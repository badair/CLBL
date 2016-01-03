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

    auto constable_void_object = const_void_struct{};

    auto f = fwrap(&constable_void_object);
    auto g = fwrap(&void_func);
    auto h = fwrap(&constable_void_object, &const_void_struct::func);
    auto i = fwrap(void_func);
    auto j = fwrap(constable_void_object, &const_void_struct::func);
    run_basic_tests(f, test_id::const_void_struct_op);
    run_basic_tests(g, test_id::void_func);
    run_basic_tests(h, test_id::const_void_struct_func);
    run_basic_tests(i, test_id::void_func);
    run_basic_tests(j, test_id::const_void_struct_func);
    return 0;
}