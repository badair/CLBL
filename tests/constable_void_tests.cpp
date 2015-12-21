#include "test.h"
#include <CLBL/clbl.h>
#include "void_definitions.h"

#include<iostream>

using namespace clbl::tests;
using namespace clbl;

void constable_void_tests() {

#ifdef CLBL_CONSTABLE_VOID_TESTS
    std::cout << "running CLBL_CONSTABLE_VOID_TESTS" << std::endl;

    auto constable_void_object = const_void_struct{};

    auto f = fwrap(&constable_void_object);
    auto g = fwrap(&void_func);
    auto h = fwrap(&constable_void_object, &const_void_struct::func);

    run_tests(
        f, test_id::const_void_struct_op,
        g, test_id::void_func,
        h, test_id::const_void_struct_func
    );

#endif
}