#include "test.h"
#include "CLBL/func.h"
#include "int_definitions.h"

#include<iostream>

using namespace clbl::tests;
using namespace clbl;

void const_int_tests() {

#ifdef CLBL_CONST_INT_TESTS
    std::cout << "running CLBL_CONST_INT_TESTS" << std::endl;

    const auto const_int_object = const_int_struct{};

    auto f = func(&const_int_object);
    auto g = func(&int_func);
    auto h = func(&const_int_object, &const_int_struct::func);

    run_tests(
        f, test_id::const_int_struct_op,
        g, test_id::int_func,
        h, test_id::const_int_struct_func,
        1);
#endif
}