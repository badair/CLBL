#include "test.h"
#include "CLBL/clbl.h"
#include "int_char_definitions.h"

#include <iostream>

using namespace clbl::tests;
using namespace clbl;

void const_volatile_int_char_tests() {

#ifdef CLBL_CONST_VOLATILE_INT_CHAR_TESTS
    std::cout << "running CLBL_CONST_VOLATILE_INT_CHAR_TESTS" << std::endl;

    auto g = fwrap(&int_char_func);

    {
        volatile const_volatile_int_char_struct const_volatile_int_char_object{};

        auto f = fwrap(&const_volatile_int_char_object);
        auto h = fwrap(&const_volatile_int_char_object, &const_volatile_int_char_struct::func);

        run_tests(
            f, test_id::const_volatile_int_char_struct_op,
            g, test_id::int_char_func,
            h, test_id::const_volatile_int_char_struct_func,
            1, 'c');
    }
    {
        const volatile const_volatile_int_char_struct const_volatile_int_char_object{};

        auto f = fwrap(&const_volatile_int_char_object);
        auto h = fwrap(&const_volatile_int_char_object, &const_volatile_int_char_struct::func);

        run_tests(
            f, test_id::const_volatile_int_char_struct_op,
            g, test_id::int_char_func,
            h, test_id::const_volatile_int_char_struct_func,
            1, 'c');
    }

#endif
}