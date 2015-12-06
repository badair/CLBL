#include "test.h"
#include "CLBL/func.h"
#include "int_char_definitions.h"

#include<iostream>

using namespace clbl::tests;
using namespace clbl;

void int_char_tests() {

#ifdef CLBL_INT_CHAR_TESTS
    std::cout << "running CLBL_INT_CHAR_TESTS" << std::endl;

    int_char_struct int_char_object{};

    auto f = func(&int_char_object);
    auto g = func(&int_char_func);
    auto h = func(&int_char_object, &int_char_struct::func);

    run_tests(
        f, test_id::int_char_struct_op,
        g, test_id::int_char_func,
        h, test_id::int_char_struct_func,
        1, 'c');

#endif
}