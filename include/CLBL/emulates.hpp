/*!
@file
Defines `clbl::emulates`.

@copyright Barrett Adair 2015
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)

*/

#ifndef CLBL_EMULATES_H
#define CLBL_EMULATES_H

#include <type_traits>

#include <CLBL/tags.hpp>
#include <CLBL/type_traits.hpp>
#include <CLBL/is_clbl.hpp>

namespace clbl {

    template<
        typename FunctionType,
        typename Callable, 
    	CLBL_REQUIRES_(is_clbl<no_ref<Callable> >::value)
    >
    static inline constexpr auto
    emulates(Callable&&) {
        return std::is_same<typename no_ref<Callable>::type, FunctionType>::value;
    }

    template<
        typename FunctionType,
        typename T,
    	CLBL_REQUIRES_(!is_clbl<no_ref<T> >::value)
    >
    static constexpr auto
    emulates(T&&) {
        static_assert(sizeof(T) < 0, 
            "You didn't pass a CLBL callable wrapper to clbl::emulates.");
        return false;
    }
}

#endif