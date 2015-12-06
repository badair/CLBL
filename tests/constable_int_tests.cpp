#include "test.h"
#include "CLBL/func.h"
#include "int_definitions.h"

#include <iostream>

using namespace clbl::tests;
using namespace clbl;

void constable_int_tests() {

#ifdef CLBL_CONSTABLE_INT_TESTS
    std::cout << "running CLBL_CONSTABLE_INT_TESTS" << std::endl;

    auto constable_int_object = const_int_struct{};

    auto f = func(&constable_int_object);
    auto g = func(&int_func);
    auto h = func(&constable_int_object, &const_int_struct::func);

    run_tests(
        f, test_id::const_int_struct_op,
        g, test_id::int_func,
        h, test_id::const_int_struct_func,
        1);

#endif
}

