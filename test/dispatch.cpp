/*

Copyright Barrett Adair 2015
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)

*/

#include <iostream>
#include <functional>
#include <memory>
#include <type_traits>
#include <cassert>
#include <CLBL/clbl.hpp>
#include <quali/quali_macros.hpp>

#ifndef CLBL_RUNTIME_ASSERT
#undef NDEBUG
#define CLBL_RUNTIME_ASSERT(...) assert(__VA_ARGS__)
#endif //#ifndef CLBL_RUNTIME_ASSERT

#ifndef CLBL_STATIC_ASSERT
#undef NDEBUG
#define CLBL_STATIC_ASSERT(...) static_assert(__VA_ARGS__, #__VA_ARGS__)
#endif //#ifndef CLBL_STATIC_ASSERT

void foo() {}

template<typename T>
typename clbl::dispatch<T&&>::dispatch_type
dispatch(T&&) { return{}; }

int main() {

    {
        using dispatch_type = decltype(dispatch(&foo));

        CLBL_STATIC_ASSERT(
            std::is_same<dispatch_type,
            clbl::function_ptr<void(*)(void) >> ::value
            );
    }
    {
        auto x = []{};
        using dispatch_type = decltype(dispatch(x));
        CLBL_STATIC_ASSERT(
            std::is_same<dispatch_type,
            clbl::function_object<clbl::generalized_object<decltype(x)&>>>::value
            );
    }
    {
        auto x = [] {};
        using dispatch_type = decltype(dispatch(std::ref(x)));

        CLBL_STATIC_ASSERT(
            std::is_same<dispatch_type,
            clbl::function_object<clbl::generalized_object<decltype(std::ref(x))&&>>>::value
            );
    }
    {
        struct X { void foo() {} };

        using dispatch_type = decltype(dispatch(&X::foo));

        CLBL_STATIC_ASSERT(
            std::is_same<dispatch_type,
            clbl::pmf<decltype(&X::foo) >> ::value
            );
    }
    return 0;
}
