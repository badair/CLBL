#include "test.h"
#include "CLBL/func.h"
#include "void_definitions.h"

#include <iostream>

using namespace clbl::tests;
using namespace clbl;

void const_void_tests() {

#ifdef CLBL_CONST_VOID_TESTS
    std::cout << "running CLBL_CONST_VOID_TESTS" << std::endl;

    const auto const_void_object = const_void_struct{};

    auto f = func(&const_void_object);
    auto g = func(&void_func);
    auto h = func(&const_void_object, &const_void_struct::func);

    run_tests(
        f, test_id::const_void_struct_op,
        g, test_id::void_func,
        h, test_id::const_void_struct_func
    );

#endif

}