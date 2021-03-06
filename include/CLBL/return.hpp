/*!
@file
Defines `clbl::result_of` and clbl::has_return.

@copyright Barrett Adair 2015
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)

*/

#ifndef CLBL_RETURN_H
#define CLBL_RETURN_H

#include <CLBL/constraints.hpp>

namespace clbl {

    template<typename Callable>
    using result_of = typename no_ref<Callable>::return_type;

    template<
        typename ReturnType,
        typename Callable,
        CLBL_REQUIRES_(is_clbl<no_ref<Callable> >::value)
    >
    static inline constexpr auto
    has_return(Callable&&) {
        return std::is_same<typename no_ref<Callable>::return_type, ReturnType>::value;
    }

    template<
        typename ReturnType,
        typename T,
        CLBL_REQUIRES_(!is_clbl<no_ref<T> >::value)
    >
    static constexpr auto
    has_return(T&&) {
        static_assert(sizeof(T) < 0,
            "You didn't pass a CLBL callable wrapper to clbl::returns.");
        return false;
    }
}

#endif