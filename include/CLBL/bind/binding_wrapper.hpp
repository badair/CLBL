/*!
Copyright (c) 2001-2004 Peter Dimov and Multi Media Ltd.
Copyright (c) 2001 David Abrahams
Copyright (c) 2005 Peter Dimov
Copyright (c) 2016 Barrett Adair

Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
*/

#ifndef CLBL_BINDING_WRAPPER_HPP
#define CLBL_BINDING_WRAPPER_HPP

#include <tuple>
#include <functional>

#include <CLBL/ph.hpp>
#include <CLBL/bind/bind_value.hpp>
#include <CLBL/bind/args_tuple.hpp>
#include <CLBL/bind/sort_tuple.hpp>
#include <CLBL/bind/placeholder_routes.hpp>
#include <CLBL/bind/remove_duplicate_placeholders.hpp>

//todo - split into multiple files

namespace clbl { namespace internal { namespace detail {

template <typename, typename, typename>
struct arguments_needed_helper;

template <typename OriginalArgsTuple,  typename PhRoutesTuple, std::size_t... I>
struct arguments_needed_helper<PhRoutesTuple, OriginalArgsTuple, std::index_sequence<I...>> {

    using type = std::tuple<
        typename std::tuple_element<
            std::tuple_element<I, PhRoutesTuple>::type::output_index,
            OriginalArgsTuple
        >::type...
    >;
};

template<typename PhRouteLeft, typename PhRouteRight>
struct compare_input_index {
    static constexpr bool value = 
        PhRouteLeft::input_index < PhRouteRight::input_index;
};

template<typename PhRouteLeft, typename PhRouteRight>
struct compare_output_index {
    static constexpr bool value = 
        PhRouteLeft::output_index < PhRouteRight::output_index;
};

template <typename PhRoutesTuple, typename OriginalArgsTuple>
struct arguments_needed {
    using duplicates_removed = typename remove_duplicate_placeholders<PhRoutesTuple>::type;
    using type = typename arguments_needed_helper<
        sort_tuple<duplicates_removed, compare_input_index>,
        OriginalArgsTuple,
        std::make_index_sequence<std::tuple_size<duplicates_removed>::value>
    >::type;
};

template <typename T>
struct arguments_needed<T, std::tuple<>> {
    using type = std::tuple<>;
};

template <typename T>
struct arguments_needed<T, clbl::ambiguous_args> {
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
            sort_tuple<typename placeholder_routes<ArgsTuple>::type, compare_input_index>,
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

    template<typename... Args>
    decltype(auto) bind(Args... args) {
        using list_type = typename detail::wrapped_args_tuple<no_ref<Args>...>::type;
        return binding_wrapper<this_t, list_type>{*this, list_type{ args... }};
    }

private:

    Callable wrapper;
    ArgsTuple bound_args;
};

     
}}} // namespaces

#endif