#include "test.h"
#include <CLBL/clbl.h>
#include "int_char_definitions.h"

#include <iostream>

using namespace clbl::tests;
using namespace clbl;

void volatile_int_char_tests() {

#ifdef CLBL_VOLATILE_INT_CHAR_TESTS
    std::cout << "running CLBL_VOLATILE_INT_CHAR_TESTS" << std::endl;

    auto g = fwrap(&int_char_func);

    {
        volatile volatile_int_char_struct volatile_int_char_object{};

        auto f = fwrap(&volatile_int_char_object);
        auto h = fwrap(&volatile_int_char_object, &volatile_int_char_struct::func);

        run_tests(
            f, test_id::volatile_int_char_struct_op,
            g, test_id::int_char_func,
            h, test_id::volatile_int_char_struct_func,
            1, 'c');
    }
    {
        //testing object that could be volatile but isn't
        volatile_int_char_struct volatilable_int_char_object{};

        auto f = fwrap(&volatilable_int_char_object);
        auto h = fwrap(&volatilable_int_char_object, &volatile_int_char_struct::func);

        run_tests(
            f, test_id::volatile_int_char_struct_op,
            g, test_id::int_char_func,
            h, test_id::volatile_int_char_struct_func,
        1, 'c');
    }

#endif
}