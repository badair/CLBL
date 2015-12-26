/*

Copyright Barrett Adair 2015
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)

*/

#ifndef CLBL_EMULATES_H
#define CLBL_EMULATES_H

#include <type_traits>

#include <CLBL/tags.h>
#include <CLBL/no_ref.h>
#include <CLBL/is_clbl.h>

namespace clbl {

    template<typename FunctionType, typename Callable, 
    	std::enable_if_t<is_clbl<no_ref<Callable> >, dummy>* = nullptr>
    static inline constexpr auto
    emulates(Callable&&) {
        return std::is_same<typename no_ref<Callable>::type, FunctionType>::value;
    }

    template<typename FunctionType, typename T,
    	std::enable_if_t<!is_clbl<no_ref<T> >, dummy>* = nullptr>
    static constexpr auto
    emulates(T&&) {
        static_assert(sizeof(T) < 0, 
            "You didn't pass a CLBL callable wrapper to clbl::emulates.");
        return false;
    }
}

#endif