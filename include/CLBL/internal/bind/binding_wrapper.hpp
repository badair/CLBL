/*!
Copyright (c) 2001-2004 Peter Dimov and Multi Media Ltd.
Copyright (c) 2001 David Abrahams
Copyright (c) 2005 Peter Dimov
Copyright (c) 2016 Barrett Adair

Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
*/

#ifndef CLBL_BIND_H
#define CLBL_BIND_H

#include <tuple>
#include <functional>

#include <CLBL/ph.hpp>

//todo - split into multiple files

namespace clbl { namespace internal {

    template<typename Callable, typename ArgsTuple>
    struct binding_wrapper;

    namespace detail {

        struct empty {};

        template<typename T>
        struct bind_value {

        public:

            bind_value(typename std::decay<T>::type const & t)
                : t_( t ) {}

            bind_value(const typename std::decay<T>::type && t)
                : t_( t ) {}

            T & get() { return t_; }

            T const & get() const { return t_; }

        private:

            T t_;
        };

        template<typename F>
        struct ref_unwrapper {

            static inline constexpr 
            F& unwrap(F& f) {
                return f;
            }

            template<typename F2>
            static inline constexpr
            F2& unwrap(std::reference_wrapper<F2> rf) {
                return rf.get();
            }
        };

        template <typename F, typename ArgsTuple, typename Tuple, std::size_t... I>
        constexpr decltype(auto)
        apply(F& f, ArgsTuple& args, Tuple&& t, std::index_sequence<I...>) {
            return f(args.apply_element(std::get<I>(static_cast<Tuple&&>(t)))...);
        }

        template<typename... Args>
        class args_tuple {

        private:

            static constexpr const auto sequence = std::make_index_sequence<sizeof...(Args)>{};
            std::tuple<Args...> data;

        public:

            //this cast forwards any rvalue references
            args_tuple(Args... args)
                : data{ static_cast<Args>(args)... } {}

            template<
                typename T,
                typename std::enable_if<std::is_placeholder<T>::value != 0, int>::type I 
                            = std::is_placeholder<T>::value
            >
            decltype(auto)
            apply_element(T &) const { return std::get<I - 1>(data); }

            template<typename T>
            T & apply_element(detail::bind_value<T> & v) const { return v.get(); }

            template<typename T>
            T const & apply_element(detail::bind_value<T> const & v) const { return v.get(); }

            template<typename T>
            T & apply_element(std::reference_wrapper<T> const & v) const { return v.get(); }

            template<typename F, typename ArgsTuple>
            auto apply_element(binding_wrapper<F, ArgsTuple> & b) const { return b.eval(*this); }

            template<typename F, typename ArgsTuple>
            auto apply_element(binding_wrapper<F, ArgsTuple> const & b) const { return b.eval(*this); }

            template<typename F, typename ArgsTuple>
            auto operator()(F& f, ArgsTuple&& args) {
                return apply(ref_unwrapper<F>::unwrap(f), args, data, sequence);
            }
        };

        template<>
        struct args_tuple<empty> {

            args_tuple() = default;

            template<typename T>
            T & apply_element(detail::bind_value<T> & v) const { return v.get(); }

            template<typename T>
            T const & apply_element(detail::bind_value<T> const & v) const { return v.get(); }

            template<typename T>
            T & apply_element(std::reference_wrapper<T> const & v) const { return v.get(); }

            template<typename F, typename ArgsTuple>
            auto apply_element(binding_wrapper<F, ArgsTuple> & b) const { return b.eval(*this); }

            template<typename F, typename ArgsTuple>
            auto apply_element(binding_wrapper<F, ArgsTuple> const & b) const { return b.eval(*this); }

            template<typename F, typename A>
            auto operator()(F& f, A&&) const { return ref_unwrapper<F>::unwrap(f)(); }
        };

        template<std::size_t OutputIndex, std::size_t InputIndex>
        struct ph_route {
            static constexpr const auto output_index = OutputIndex;
            static constexpr const auto input_index = InputIndex;
        };

        template<typename, typename> struct argument_routing {};

        template<std::size_t... I, typename Tuple>
        struct argument_routing<std::index_sequence<I...>, Tuple> {
            using type =
                std::tuple<
                    ph_route<
                        I,
                        std::is_placeholder<
                            typename std::tuple_element<I, Tuple>::type
                        >::value
                    >...
                >;
        };

        template <typename...> struct prepend;

        template <> struct prepend<> {
            using type = std::tuple<>;
        };

        template <typename T, typename... Args>
        struct prepend<T, std::tuple<Args...> > {
            using type = std::tuple<T, Args...>;
        };

        //base case
        template <typename...> struct placeholder_routes_detail;
        template <> struct placeholder_routes_detail<> {
            using type = std::tuple<>;
        };

        template <typename Head, typename ...Tail>
        struct placeholder_routes_detail<Head, Tail...> {
            //TODO - is there a faster way to do this?
            using type = typename std::conditional<
                Head::input_index == 0,
                typename placeholder_routes_detail<Tail...>::type,
                typename prepend<
                    Head,
                    typename placeholder_routes_detail<Tail...>::type
                >::type
            >::type;
        };

        template <typename... Args>
        struct placeholder_routes_detail<std::tuple<Args...> > {
            using type = typename placeholder_routes_detail<Args...>::type;
        };

        template <typename...> struct placeholder_routes;
        template <> struct placeholder_routes<> { using type = std::tuple<>; };

        template<std::size_t Target, typename...>
        struct ph_sort;

        //base case
        template<std::size_t Target>
        struct ph_sort<Target>{
            using type = std::tuple<>;
        };

        // sorting the placeholders to get the argument index in the order of the
        // original signature. This is a horribly slow recursive selection sort
        // at O(n) best case and O(n^2) worst case, but it will do for now...
        // TODO optimize this sort
        // TODO handle duplicate placeholders
        // TODO this is not a real sort
        template<std::size_t Target, std::size_t OutputIndex, typename... Tail>
        struct ph_sort<Target, ph_route<OutputIndex, Target>, Tail...>{
            using type = typename prepend<
                std::integral_constant<
                    std::size_t,ph_route<
                    OutputIndex, Target
                >::output_index>,
                typename ph_sort<Target + 1, Tail...>::type
            >::type;
        };

        //pushing the head to the back because it isn't the argrument we are looking for
        template<std::size_t Target, typename Head, typename... Tail>
        struct ph_sort<Target, Head, Tail...>{
            using type = typename ph_sort<Target, Tail..., Head>::type;
        };

        //entry point to ph_sort
        template<std::size_t Target, typename... Args>
        struct ph_sort<Target, std::tuple<Args...> >{
            using type = typename ph_sort<Target, Args...>::type;
        };

        template <typename... Args>
        struct placeholder_routes<clbl::internal::detail::args_tuple<Args...> >
        {
            using type = typename ph_sort<
                1,
                typename placeholder_routes_detail<
                    typename argument_routing<
                        std::make_index_sequence<sizeof...(Args)>,
                        std::tuple<Args...>
                    >::type
                >::type
            >::type;
        };

        template<typename, typename> struct arguments_needed {
            using type = std::tuple<>;
        };

        template < std::size_t... I, typename OriginalArgsTuple>
        struct arguments_needed<std::tuple<std::integral_constant<std::size_t, I>...>, OriginalArgsTuple> {
            using type = std::tuple<typename std::tuple_element<I, OriginalArgsTuple>::type...>;
        };

        template <std::size_t... I>
        struct arguments_needed<std::tuple<std::integral_constant<std::size_t, I>...>, std::tuple<>> {
            using type = std::tuple<>;
        };

        template < std::size_t... I>
        struct arguments_needed<std::tuple<std::integral_constant<std::size_t, I>...>, clbl::ambiguous_args> {
            using type = ambiguous_args;
        };

        template<typename T, int I>
        struct add_value_2 {
            using type = clbl::ph<I>;
        };

        template< typename T >
        struct add_value_2< T, 0 > {
            using type = detail::bind_value< T >;
        };

        template<typename T>
        struct add_value {
            using type = typename add_value_2< T, std::is_placeholder< T >::value >::type;
        };

        template<typename T>
        struct add_value< detail::bind_value<T> > {
            using type = detail::bind_value<T>;
        };

        template<typename T>
        struct add_value< std::reference_wrapper<T> > {
            using type = std::reference_wrapper<T>;
        };

        template<int I>
        struct add_value< clbl::ph<I> > {
            using type = clbl::ph<I>;
        };

        template<typename Callable, typename ArgsTuple>
        struct add_value< binding_wrapper<Callable, ArgsTuple> > {
            using type = binding_wrapper<Callable, ArgsTuple>;
        };

        template<typename... Args>
        struct wrapped_args_tuple {
            using type = args_tuple<typename add_value<Args>::type...>;
        };

        template<typename Callable, typename ArgsTuple>
        struct binding_wrapper {
        
        public:

            using this_t = binding_wrapper<Callable, ArgsTuple>;

            static constexpr const auto is_ambiguous = Callable::is_ambiguous;

            using arg_types = 
                typename arguments_needed<
                    typename placeholder_routes<ArgsTuple>::type,
                    typename Callable::arg_types
                >::type;

            using return_type = typename Callable::return_type;

            binding_wrapper(Callable f, ArgsTuple const & l) : wrapper(f), bound_args(l) {}

            auto operator()() { return bound_args(wrapper, args_tuple<empty>{}); }

            auto operator()() const { return bound_args(wrapper, args_tuple<empty>{}); }

            //perfect forwarding on invocation - we're not going to mess with lazy bind expressions
            template<typename... Args>
            auto operator()(Args&&... args) { 
                return bound_args(wrapper, args_tuple<Args&&...>{static_cast<Args&&>(args)...});
            }

            template<typename A>
            auto eval(A & a) { return bound_args(wrapper, a); }

            template<typename A>
            auto eval(A & a) const { return bound_args(wrapper, a); }

            template<typename... Fargs>
            decltype(auto) bind(Fargs... args) {
                using list_type = typename detail::wrapped_args_tuple<no_ref<Fargs>...>::type;
                return binding_wrapper<this_t, list_type>{*this, list_type{ args... }};
            }

        private:

            Callable wrapper;
            ArgsTuple bound_args;
        };

    } // namespace detail
}}
    
#endif