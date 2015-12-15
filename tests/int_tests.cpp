#include "test.h"
#include "CLBL/clbl.h"
#include "int_definitions.h"

#include<iostream>

using namespace clbl::tests;
using namespace clbl;

void int_tests() {

#ifdef CLBL_INT_TESTS
    std::cout << "running CLBL_INT_TESTS" << std::endl;

    int_struct int_object{};

    auto f = fwrap(&int_object);
    auto g = fwrap(&int_func);
    auto h = fwrap(&int_object, &int_struct::func);
    auto hardened_f = harden<const char*(int)>(f);
    auto hardened_g = harden<const char*(int)>(g);
    auto hardened_h = harden<const char*(int)>(h);
    auto default_hardened_f = harden(f);
    auto default_hardened_g = harden(g);
    auto default_hardened_h = harden(h);

    auto j = fwrap(int_object);
    auto k = fwrap(&int_func);
    auto l = fwrap(int_object, &int_struct::func);
    auto hardened_j = harden<const char*(int)>(j);
    auto hardened_k = harden<const char*(int)>(k);
    auto hardened_l = harden<const char*(int)>(l);
    auto default_hardened_j = harden(j);
    auto default_hardened_k = harden(k);
    auto default_hardened_l = harden(l);

    static_assert(std::is_same<decltype(f)::type, const char*(int)>::value, "std::is_same<decltype(f)::type, const char*(int)>::value");

    run_tests(
        f, test_id::int_struct_op,
        g, test_id::int_func,
        h, test_id::int_struct_func,
        1
    );

    run_tests(
        hardened_f, test_id::int_struct_op,
        hardened_g, test_id::int_func,
        hardened_h, test_id::int_struct_func,
        1
        );

    run_tests(
        default_hardened_f, test_id::int_struct_op,
        default_hardened_g, test_id::int_func,
        default_hardened_h, test_id::int_struct_func,
        1
    );

    run_tests(
        j, test_id::int_struct_op,
        k, test_id::int_func,
        l, test_id::int_struct_func,
        1
        );

    run_tests(
        hardened_j, test_id::int_struct_op,
        hardened_k, test_id::int_func,
        hardened_l, test_id::int_struct_func,
        1
        );

    run_tests(
        default_hardened_j, test_id::int_struct_op,
        default_hardened_k, test_id::int_func,
        default_hardened_l, test_id::int_struct_func,
        1
        );

#endif
}