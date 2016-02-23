/*

Copyright Barrett Adair 2015
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)

*/

#include "test.hpp"
#include "int_char_definitions.hpp"

#include <iostream>
#include <CLBL/clbl.hpp>

using namespace clbl::test;
using namespace clbl;

int main() {

    //testing volatile object

    volatile volatile_int_char_struct volatile_int_char_object{};

    auto f = fwrap(&int_char_func);
    auto g = fwrap(&volatile_int_char_object);
    auto h = fwrap(&volatile_int_char_struct::func, &volatile_int_char_object);
    auto i = fwrap(volatile_int_char_object);
    auto j = fwrap(&volatile_int_char_struct::func, volatile_int_char_object);

    run_basic_tests(f, test_id::int_char_func, 1, 'c');
    run_basic_tests(g, test_id::volatile_int_char_struct_op, 1, 'c');
    run_basic_tests(h, test_id::volatile_int_char_struct_func, 1, 'c');
    run_basic_tests(i, test_id::volatile_int_char_struct_op, 1, 'c');
    run_basic_tests(j, test_id::volatile_int_char_struct_func, 1, 'c');

    return 0;
}