/*!
@file
Defines `clbl::is_ambiguous`.

@copyright Barrett Adair 2015
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)

*/

#ifndef CLBL_IS_AMBIGUOUS_H
#define CLBL_IS_AMBIGUOUS_H

#include <type_traits>
#include <CLBL/tags.hpp>
#include <CLBL/type_traits.hpp>
#include <CLBL/is_clbl.hpp>
#include <CLBL/constraints.hpp>

namespace clbl {

    template<
        typename Callable, 
    	CLBL_REQUIRES_(is_clbl<no_ref<Callable> >::value)
    >
    static inline constexpr auto
    is_ambiguous(Callable&&) {
        return no_ref<Callable>::is_ambiguous;
    }

    template<
        typename T,
    	CLBL_REQUIRES_(!is_clbl<no_ref<T> >::value)
    >
    static inline constexpr auto
    is_ambiguous(T&&) {
        static_assert(sizeof(T) < 0,
            "You didn't pass a CLBL callable wrapper to clbl::is_ambiguous.");
        return false;
    }
}

#endif