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

    volatile const_volatile_int_char_struct const_volatile_int_char_object{};

    auto g = fwrap(&int_char_func);
    auto f = fwrap(&const_volatile_int_char_object);
    auto h = fwrap(&const_volatile_int_char_struct::func, &const_volatile_int_char_object);
    auto i = fwrap(const_volatile_int_char_object);

    run_basic_tests(g, test_id::int_char_func, 1, 'c');
    run_basic_tests(f, test_id::const_volatile_int_char_struct_op, 1, 'c');
    run_basic_tests(h, test_id::const_volatile_int_char_struct_func, 1, 'c');
    run_basic_tests(i, test_id::const_volatile_int_char_struct_op, 1, 'c');

    return 0;
}