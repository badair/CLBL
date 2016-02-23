/*

Copyright Barrett Adair 2015
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)

*/

#include "test.hpp"
#include <CLBL/clbl.hpp>
#include "int_definitions.hpp"

#include <iostream>

using namespace clbl::test;
using namespace clbl;

int main() {

    auto constable_int_char_object = const_int_char_struct{};

    auto f = fwrap(&constable_int_char_object);
    auto g = fwrap(&int_char_func);
    auto h = fwrap(&const_int_char_struct::func, &constable_int_char_object);
    auto i = fwrap(int_char_func);

    run_basic_tests(f, test_id::const_int_char_struct_op, 1, 'c');
    run_basic_tests(g, test_id::int_char_func, 1, 'c');
    run_basic_tests(h, test_id::const_int_char_struct_func, 1, 'c');
    run_basic_tests(i, test_id::int_char_func, 1, 'c');

    return 0;
}