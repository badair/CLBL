/*

Copyright Barrett Adair 2015
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)

*/

#include "test.h"
#include "int_char_definitions.h"

#include <iostream>
#include <CLBL/clbl.h>

using namespace clbl::tests;
using namespace clbl;

int main() {

#ifdef CLBL_CONST_VOLATILE_INT_CHAR_TESTS
    std::cout << "running CLBL_CONST_VOLATILE_INT_CHAR_TESTS" << std::endl;

    auto g = fwrap(&int_char_func);

    {
        volatile const_volatile_int_char_struct const_volatile_int_char_object{};

        auto f = fwrap(&const_volatile_int_char_object);
        auto h = fwrap(&const_volatile_int_char_object, &const_volatile_int_char_struct::func);

        run_tests(
            f, test_id::const_volatile_int_char_struct_op,
            g, test_id::int_char_func,
            h, test_id::const_volatile_int_char_struct_func,
            1, 'c');
    }
    {
        const volatile const_volatile_int_char_struct const_volatile_int_char_object{};

        auto f = fwrap(&const_volatile_int_char_object);
        auto h = fwrap(&const_volatile_int_char_object, &const_volatile_int_char_struct::func);

        run_tests(
            f, test_id::const_volatile_int_char_struct_op,
            g, test_id::int_char_func,
            h, test_id::const_volatile_int_char_struct_func,
            1, 'c');
    }

#endif

    return 0;
}