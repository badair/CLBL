/*

Copyright Barrett Adair 2015
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)

*/

#include "test.h"
#include "void_definitions.h"

#include<iostream>
#include <CLBL/clbl.h>

using namespace clbl::tests;
using namespace clbl;

int main() {

#ifdef CLBL_VOID_TESTS
    std::cout << "running CLBL_VOID_TESTS" << std::endl;

    void_struct void_object{};

    auto f = fwrap(&void_object);
    auto g = fwrap(&void_func);
    auto h = fwrap(&void_object, &void_struct::func);

    /*todo make this work (also try_call)
    static_assert(!can_call(f, 1), "");
    static_assert(can_call(f), "");*/

    run_tests(
        f, test_id::void_struct_op,
        g, test_id::void_func,
        h, test_id::void_struct_func
        );

#endif

    return 0;
}