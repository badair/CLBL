/*!
@file
Defines `clbl::has_normal_call_operator` and `clbl::ptr_has_normal_call_operator`.

@copyright Barrett Adair 2015
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)

*/

#ifndef CLBL_HAS_NORMAL_CALL_OPERATOR_H
#define CLBL_HAS_NORMAL_CALL_OPERATOR_H

#include <utility>

#include <CLBL/no_ref.h>
#include <CLBL/is_valid.h>

namespace clbl {

    /*
    clbl::fwrap is a heavily overloaded function that can be used to create CLBL 
    wrappers for anything that is callable.
    */

    namespace detail {
        auto has_normal_call_operator_impl = is_valid(
            [](auto&& arg)->decltype(&no_ref<decltype(arg)>::operator()) {}
        );

        auto ptr_has_normal_call_operator_impl = is_valid(
            [](auto&& arg)->decltype(&no_ref<decltype(*arg)>::operator()) {}
        );
    }

    template<typename T>
    constexpr bool has_normal_call_operator = 
    	decltype(detail::has_normal_call_operator_impl(std::declval<T>()))::value;

    template<typename T>
    constexpr bool ptr_has_normal_call_operator =
        decltype(detail::ptr_has_normal_call_operator_impl(std::declval<T>()))::value;

}

#endif