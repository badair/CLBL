/*

Copyright Barrett Adair 2015
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)

*/

#include "test.h"
#include <CLBL/clbl.h>
#include "int_char_definitions.h"

#include<iostream>

using namespace clbl::tests;
using namespace clbl;

int main() {

#ifdef CLBL_INT_CHAR_TESTS
    std::cout << "running CLBL_INT_CHAR_TESTS" << std::endl;

    int_char_struct int_char_object{};

    auto f = fwrap(&int_char_object);
    auto g = fwrap(&int_char_func);
    auto h = fwrap(&int_char_object, &int_char_struct::func);

    run_tests(
        f, test_id::int_char_struct_op,
        g, test_id::int_char_func,
        h, test_id::int_char_struct_func,
        1, 'c');

#endif

    return 0;
}