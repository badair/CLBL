/*

Copyright Barrett Adair 2015
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)

*/

#include "test.h"
#include "int_definitions.h"

#include<iostream>
#include <CLBL/clbl.h>

using namespace clbl::tests;
using namespace clbl;

int main() {


    const auto const_int_object = const_int_struct{};

    
    auto f = fwrap(&const_int_object);
    auto g = fwrap(&int_func);
    auto h = fwrap(&const_int_object, &const_int_struct::func);
    auto i = fwrap(int_func);
    auto j = fwrap(const_int_object, &const_int_struct::func);
    auto hardened_f = harden<std::string(int)>(f);
    auto hardened_g = harden<std::string(int)>(g);
    auto hardened_h = harden<std::string(int)>(h);
    auto hardened_i = harden<std::string(int)>(i);
    auto hardened_j = harden<std::string(int)>(j);
    auto default_hardened_f = harden(f);
    auto default_hardened_g = harden(g);
    auto default_hardened_h = harden(h);
    auto default_hardened_i = harden(i);
    auto default_hardened_j = harden(j);
    
    run_basic_tests(f, test_id::const_int_struct_op, 1);
    run_basic_tests(g, test_id::int_func, 1);
    run_basic_tests(h, test_id::const_int_struct_func, 1);
    run_basic_tests(i, test_id::int_func, 1);
    run_basic_tests(j, test_id::const_int_struct_func, 1);
    run_basic_tests(hardened_f, test_id::const_int_struct_op, 1);
    run_basic_tests(hardened_g, test_id::int_func, 1);
    run_basic_tests(hardened_h, test_id::const_int_struct_func, 1);
    run_basic_tests(hardened_i, test_id::int_func, 1);
    run_basic_tests(hardened_j, test_id::const_int_struct_func, 1);
    run_basic_tests(default_hardened_f, test_id::const_int_struct_op, 1);
    run_basic_tests(default_hardened_g, test_id::int_func, 1);
    run_basic_tests(default_hardened_h, test_id::const_int_struct_func, 1);
    run_basic_tests(default_hardened_i, test_id::int_func, 1);
    run_basic_tests(default_hardened_j, test_id::const_int_struct_func, 1);
        
    return 0;
}