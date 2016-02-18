/*

Copyright Barrett Adair 2015
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)

*/
#include "test.hpp"
#include <CLBL/clbl.hpp>
#include "int_definitions.hpp"

#include <iostream>
#include <functional>

using namespace clbl::test;
using namespace clbl;

int main() {

    int_struct int_object{};

    auto f = fwrap(&int_object);
    auto g = fwrap(&int_func);
    auto h = fwrap(&int_object, &int_struct::func);
    auto i = fwrap(int_func);
    auto j = fwrap(int_object, &int_struct::func);
    auto k = fwrap(int_object);
    auto l = fwrap(std::ref(int_object));
    auto m = fwrap(std::ref(int_object), &int_struct::func);

    auto hardened_f = harden<auto_(int)>(f);
    auto hardened_g = harden<auto_(int)>(g);
    auto hardened_h = harden<auto_(int)>(h);
    auto hardened_i = harden<auto_(int)>(i);
    auto hardened_j = harden<auto_(int)>(j);
    auto hardened_k = harden<auto_(int)>(k);
    auto hardened_l = harden<auto_(int)>(l);
    auto hardened_m = harden<auto_(int)>(m);

    auto default_hardened_f = harden(f);
    auto default_hardened_g = harden(g);
    auto default_hardened_h = harden(h);
    auto default_hardened_i = harden(i);
    auto default_hardened_j = harden(j);
    auto default_hardened_k = harden(k);
    auto default_hardened_l = harden(l);
    auto default_hardened_m = harden(m);

    run_basic_tests(f, test_id::int_struct_op, 1);
    run_basic_tests(hardened_f, test_id::int_struct_op, 1);
    run_basic_tests(default_hardened_f, test_id::int_struct_op, 1);

    run_basic_tests(g, test_id::int_func, 1);
    run_basic_tests(hardened_g, test_id::int_func, 1);
    run_basic_tests(default_hardened_g, test_id::int_func, 1);

    run_basic_tests(h, test_id::int_struct_func, 1);
    run_basic_tests(hardened_h, test_id::int_struct_func, 1);
    run_basic_tests(default_hardened_h, test_id::int_struct_func, 1);

    run_basic_tests(i, test_id::int_func, 1);
    run_basic_tests(hardened_i, test_id::int_func, 1);
    run_basic_tests(default_hardened_i, test_id::int_func, 1);

    run_basic_tests(j, test_id::int_struct_func, 1);
    run_basic_tests(hardened_j, test_id::int_struct_func, 1);
    run_basic_tests(default_hardened_j, test_id::int_struct_func, 1);

    run_basic_tests(k, test_id::int_struct_op, 1);
    run_basic_tests(hardened_k, test_id::int_struct_op, 1);
    run_basic_tests(default_hardened_k, test_id::int_struct_op, 1);

    run_basic_tests(l, test_id::int_struct_op, 1);
    run_basic_tests(hardened_l, test_id::int_struct_op, 1);
    run_basic_tests(default_hardened_l, test_id::int_struct_op, 1);

    run_basic_tests(m, test_id::int_struct_func, 1);
    run_basic_tests(hardened_m, test_id::int_struct_func, 1);
    run_basic_tests(default_hardened_m, test_id::int_struct_func, 1);

    return 0;
}