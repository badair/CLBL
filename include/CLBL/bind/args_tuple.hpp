/*!
Copyright (c) 2001-2004 Peter Dimov and Multi Media Ltd.
Copyright (c) 2001 David Abrahams
Copyright (c) 2005 Peter Dimov
Copyright (c) 2016 Barrett Adair

Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
*/

#ifndef CLBL_BIND_ARGS_TUPLE_HPP
#define CLBL_BIND_ARGS_TUPLE_HPP

#include <CLBL/bind/bind_value.hpp>

namespace clbl { namespace internal { namespace detail {

struct empty {};

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

}}} //end namespaces

#endif