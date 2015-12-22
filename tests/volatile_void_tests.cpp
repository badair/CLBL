/*

Copyright Barrett Adair 2015
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)

*/

#include "test.h"
#include "void_definitions.h"

#include <iostream>
#include <CLBL/clbl.h>

using namespace clbl::tests;
using namespace clbl;

int main() {

#ifdef CLBL_VOLATILE_VOID_TESTS
    std::cout << "running CLBL_VOLATILE_VOID_TESTS" << std::endl;

    auto g = fwrap(&void_func);

    {
        volatile_void_struct volatile_void_object{};

        auto f = fwrap(&volatile_void_object);
        auto h = fwrap(&volatile_void_object, &volatile_void_struct::func);

        run_tests(
            f, test_id::volatile_void_struct_op,
            g, test_id::void_func,
            h, test_id::volatile_void_struct_func
            );
    }
    {
        volatile volatile_void_struct volatile_void_object{};

        auto f = fwrap(&volatile_void_object);
        auto h = fwrap(&volatile_void_object, &volatile_void_struct::func);

        run_tests(
            f, test_id::volatile_void_struct_op,
            g, test_id::void_func,
            h, test_id::volatile_void_struct_func
            );
    }

#endif

    return 0;
}