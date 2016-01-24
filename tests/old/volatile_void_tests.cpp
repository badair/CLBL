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
    volatile_void_struct volatile_void_object{};
    
    auto f = fwrap(&volatile_void_object);
    auto g = fwrap(&volatile_void_object, &volatile_void_struct::func);
    auto h = fwrap(volatile_void_object);
    auto i = fwrap(volatile_void_object, &volatile_void_struct::func);
    
    run_basic_tests(f, test_id::volatile_void_struct_op);
    run_basic_tests(g, test_id::volatile_void_struct_func);
    run_basic_tests(h, test_id::volatile_void_struct_op);
    run_basic_tests(i, test_id::volatile_void_struct_func);

    return 0;
}