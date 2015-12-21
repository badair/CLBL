#include "test.h"
#include <CLBL/clbl.h>
#include "int_definitions.h"

#include <iostream>

using namespace clbl::tests;
using namespace clbl;

void constable_int_tests() {

#ifdef CLBL_CONSTABLE_INT_TESTS
    std::cout << "running CLBL_CONSTABLE_INT_TESTS" << std::endl;

    auto constable_int_object = const_int_struct{};

    {
        auto f = fwrap(&constable_int_object);
        auto g = fwrap(&int_func);
        auto h = fwrap(&constable_int_object, &const_int_struct::func);
        auto hardened_f = harden<const char*(int)>(f);
        auto hardened_g = harden<const char*(int)>(g);
        auto hardened_h = harden<const char*(int)>(h);
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
        auto f = fwrap(constable_int_object);
        auto g = fwrap(&int_func);
        auto h = fwrap(constable_int_object, &const_int_struct::func);
        auto hardened_f = harden<const char*(int)>(f);
        auto hardened_g = harden<const char*(int)>(g);
        auto hardened_h = harden<const char*(int)>(h);
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
}

