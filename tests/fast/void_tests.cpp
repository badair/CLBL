/*

Copyright Barrett Adair 2015
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)

*/

#include "test.h"
#include "void_definitions.h"

#include <iostream>
#include <functional>
#include <CLBL/clbl.h>

using namespace clbl::tests;
using namespace clbl;

int main() {

    void_struct void_object{};

    auto f = fwrap(&void_object);
    auto g = fwrap(&void_func);
    auto h = fwrap(&void_object, &void_struct::func);
    auto i = fwrap(void_func);
    auto j = fwrap(void_object, &void_struct::func);
    auto k = fwrap(std::ref(void_object), &void_struct::func);
    auto l = fwrap(std::ref(void_object));
    
    run_basic_tests(f, test_id::void_struct_op);
    run_basic_tests(g, test_id::void_func);
    run_basic_tests(h, test_id::void_struct_func);
    run_basic_tests(i, test_id::void_func);
    run_basic_tests(j, test_id::void_struct_func);
    run_basic_tests(k, test_id::void_struct_func);
    run_basic_tests(l, test_id::void_struct_op);

    return 0;
}