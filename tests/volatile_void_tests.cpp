#include "test.h"
#include "CLBL/func.h"
#include "void_definitions.h"

#include <iostream>

using namespace clbl::tests;
using namespace clbl;

void volatile_void_tests() {

#ifdef CLBL_VOLATILE_VOID_TESTS
    std::cout << "running CLBL_VOLATILE_VOID_TESTS" << std::endl;

    auto g = func(&void_func);

    {
        volatile_void_struct volatile_void_object{};

        auto f = func(&volatile_void_object);
        auto h = func(&volatile_void_object, &volatile_void_struct::func);

        run_tests(
            f, test_id::volatile_void_struct_op,
            g, test_id::void_func,
            h, test_id::volatile_void_struct_func
            );
    }
    {
        volatile volatile_void_struct volatile_void_object{};

        auto f = func(&volatile_void_object);
        auto h = func(&volatile_void_object, &volatile_void_struct::func);

        run_tests(
            f, test_id::volatile_void_struct_op,
            g, test_id::void_func,
            h, test_id::volatile_void_struct_func
            );
    }

#endif
}