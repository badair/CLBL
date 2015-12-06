#include "test.h"
#include "CLBL/func.h"
#include "void_definitions.h"

#include<iostream>

using namespace clbl::tests;
using namespace clbl;

void void_tests() {

#ifdef CLBL_VOID_TESTS
    std::cout << "running CLBL_VOID_TESTS" << std::endl;

    void_struct void_object{};

    auto f = func(&void_object);
    auto g = func(&void_func);
    auto h = func(&void_object, &void_struct::func);

    run_tests(
        f, test_id::void_struct_op,
        g, test_id::void_func,
        h, test_id::void_struct_func
        );

#endif
}