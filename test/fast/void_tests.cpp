/*

Copyright Barrett Adair 2015
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)

*/

#include "test.hpp"
#include "void_definitions.hpp"

#include <iostream>
#include <functional>
#include <CLBL/clbl.hpp>

using namespace clbl::test;
using namespace clbl;

int main() {

    void_struct void_object{};

    auto f = fwrap(&void_object);
    auto g = fwrap(&void_func);
    auto h = fwrap(&void_struct::func, &void_object);
    auto i = fwrap(void_func);
    auto j = fwrap(&void_struct::func, void_object);
    auto k = fwrap(&void_struct::func, std::ref(void_object));
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