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

#ifdef CLBL_CONST_INT_TESTS
    std::cout << "running CLBL_CONST_INT_TESTS" << std::endl;

    const auto const_int_object = const_int_struct{};

    {
        auto f = fwrap(&const_int_object);
        auto g = fwrap(&int_func);
        auto h = fwrap(&const_int_object, &const_int_struct::func);
        auto hardened_f = harden<std::string(int)>(f);
        auto hardened_g = harden<std::string(int)>(g);
        auto hardened_h = harden<std::string(int)>(h);
        auto default_hardened_f = harden(f);
        auto default_hardened_g = harden(g);
        auto default_hardened_h = harden(h);

        run_tests(
            f, test_id::const_int_struct_op,
            g, test_id::int_func,
            h, test_id::const_int_struct_func,
            1
            );

        run_tests(
            hardened_f, test_id::const_int_struct_op,
            hardened_g, test_id::int_func,
            hardened_h, test_id::const_int_struct_func,
            1
            );

        run_tests(
            default_hardened_f, test_id::const_int_struct_op,
            default_hardened_g, test_id::int_func,
            default_hardened_h, test_id::const_int_struct_func,
            1
            );
    }
    {
        auto f = fwrap(const_int_object);
        auto g = fwrap(&int_func);
        auto h = fwrap(const_int_object, &const_int_struct::func);
        auto hardened_f = harden<auto_(int)>(f);
        auto hardened_g = harden<auto_(int)>(g);
        auto hardened_h = harden<auto_(int)>(h);
        auto default_hardened_f = harden(f);
        auto default_hardened_g = harden(g);
        auto default_hardened_h = harden(h);

        run_tests(
            f, test_id::const_int_struct_op,
            g, test_id::int_func,
            h, test_id::const_int_struct_func,
            1
            );

        run_tests(
            hardened_f, test_id::const_int_struct_op,
            hardened_g, test_id::int_func,
            hardened_h, test_id::const_int_struct_func,
            1
            );

        run_tests(
            default_hardened_f, test_id::const_int_struct_op,
            default_hardened_g, test_id::int_func,
            default_hardened_h, test_id::const_int_struct_func,
            1
            );
    }

#endif

    return 0;
}