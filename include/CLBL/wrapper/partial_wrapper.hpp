/*!
@file

@copyright Louis Dionne 2013-2016
@copyright Modified Work Barrett Adair 2016
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#ifndef CLBL_PARTIAL_H
#define CLBL_PARTIAL_H

#include <cstddef>
#include <utility>
#include <CLBL/forward.hpp>

namespace clbl { namespace internal {

template <typename Tuple, int Trim, typename IndexSequence>
struct trim_left_detail;

template <typename Tuple, int Trim, std::size_t... I>
struct trim_left_detail<Tuple, Trim, std::index_sequence<I...>>
{
    using type = std::tuple<typename std::tuple_element<I + Trim, Tuple>::type...>;
};
                    
template <int Trim, typename T>
struct trim_left;
                        
template <int Trim, typename... T>
struct trim_left<Trim, std::tuple<T...>>
{
  using type = typename trim_left_detail<
    std::tuple<T...>,
    Trim,
    std::make_index_sequence<sizeof...(T) - Trim>
  >::type;
};

template<typename Return, typename Tuple>
struct unpack_as_forward_function;

template<typename Return, typename... T>
struct unpack_as_forward_function<Return, std::tuple<T...>> {
    using type = Return(forward<T>...);
};

template<typename Return, typename Tuple>
struct unpack_as_function;

template<typename Return, typename... T>
struct unpack_as_function<Return, std::tuple<T...>> {
    using type = Return(T...);
};

template <typename Indices, typename Base, typename ...X>
struct partial_wrapper;

template <std::size_t ...n, typename Base, typename ...X>
struct partial_wrapper<std::index_sequence<n...>, Base, X...> 
    : Base {
    // Not needed in theory; workaround for a bug in libstdc++'s tuple,
    // which instantiates the default constructor of elements too eagerly.
    partial_wrapper() = default;

    using arg_types = typename trim_left<sizeof...(X), typename Base::arg_types>::type;

    using clbl_tag = typename Base::clbl_tag;

    using forwarding_glue = typename unpack_as_forward_function<
        typename Base::return_type,
        arg_types
    >::type;

    using function_type = typename unpack_as_function<
        typename Base::return_type,
        arg_types
    >::type;

    template <typename ...T>
    constexpr partial_wrapper(Base const& b, T&& ...t)
        : Base(b), storage_{static_cast<T&&>(t)...}
    { }

    std::tuple<X...> storage_;

    template <typename ...Y>
    constexpr decltype(auto) operator()(Y&& ...y) const& {
        return Base::invoke(
            std::get<n>(storage_)...,
            static_cast<Y&&>(y)...
        );
    }

    template <typename ...Y>
    constexpr decltype(auto) operator()(Y&& ...y) & {
        return Base::invoke (
            std::get<n>(storage_)...,
            static_cast<Y&&>(y)...
        );
    }

    template <typename ...Y>
    constexpr decltype(auto) operator()(Y&& ...y) && {
        return Base::invoke (
            static_cast<X&&>(std::get<n>(storage_))...,
            static_cast<Y&&>(y)...
        );
    }

    template<typename Wrapper>
    static inline constexpr auto
    copy_wrapper(Wrapper const & w) {
        return w;
    }
};

}}

#endif