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
#include <CLBL/tags.h>
#include <CLBL/type_traits.h>
#include <CLBL/is_clbl.h>

namespace clbl {
    namespace detail {
        template<typename Callable>
        static constexpr auto is_ambiguous_t = no_ref<Callable>::is_ambiguous;
    }

    template<typename Callable, 
    	std::enable_if_t<is_clbl<no_ref<Callable> >, dummy>* = nullptr>
    static inline constexpr auto
    is_ambiguous(Callable&&) {
        return no_ref<Callable>::is_ambiguous;
    }

    template<typename T,
    	std::enable_if_t<!is_clbl<no_ref<T> >, dummy>* = nullptr>
    static inline constexpr auto
    is_ambiguous(T&&) {
        static_assert(sizeof(T) < 0,
            "You didn't pass a CLBL callable wrapper to clbl::is_ambiguous.");
        return false;
    }
}

#endif