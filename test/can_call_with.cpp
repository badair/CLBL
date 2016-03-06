/*

Copyright Barrett Adair 2015
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)

*/

#include <functional>
#include <memory>
#include <type_traits>
#include <cassert>
#include <CLBL/clbl.hpp>
#include <quali/quali_macros.hpp>

#ifndef CLBL_STATIC_ASSERT
#undef NDEBUG
#define CLBL_STATIC_ASSERT(...) static_assert(__VA_ARGS__, #__VA_ARGS__)
#endif //#ifndef CLBL_STATIC_ASSERT

struct foo {
    void operator()(int);
    char operator()(char);
};

int main() {

    CLBL_STATIC_ASSERT(clbl::can_call_with<foo, int>::value);
    CLBL_STATIC_ASSERT(clbl::can_call_with<foo, char>::value);
    CLBL_STATIC_ASSERT(!clbl::can_call_with<foo, foo>::value);
    CLBL_STATIC_ASSERT(!clbl::can_call_with<foo, int*>::value);

    return 0;
}
