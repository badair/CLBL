#include "test.h"
#include "CLBL/clbl.h"
#include "int_definitions.h"

#include <iostream>

using namespace clbl::tests;
using namespace clbl;

void constable_int_char_tests() {

#ifdef CLBL_CONSTABLE_INT_CHAR_TESTS
    std::cout << "running CLBL_CONSTABLE_INT_CHAR_TESTS" << std::endl;

    auto constable_int_char_object = const_int_char_struct{};

    auto f = fwrap(&constable_int_char_object);
    auto g = fwrap(&int_char_func);
    auto h = fwrap(&constable_int_char_object, &const_int_char_struct::func);

    run_tests(
        f, test_id::const_int_char_struct_op,
        g, test_id::int_char_func,
        h, test_id::const_int_char_struct_func,
        1, 'c');


#endif
}