/*!
@file

@copyright Barrett Adair 2015
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)

*/

// testing header include behavior - quali_macros_undef.hpp #undefs 
// all the macros in quali_macros.hpp. Both headers #undef each other's
// header guards

#include <quali/quali_macros.hpp>
#include <quali/quali_macros_undef.hpp>
#include <quali/quali_macros.hpp>
#include <quali/quali_macros.hpp>
#include <quali/quali_macros_undef.hpp>
#include <quali/quali_macros.hpp>

#ifndef CLBL_STATIC_ASSERT
#define CLBL_STATIC_ASSERT(...) static_assert(__VA_ARGS__, #__VA_ARGS__)
#endif

int main() {
    CLBL_STATIC_ASSERT(QUALI_INTC(const){} == QUALI_FLAGS(const));
}