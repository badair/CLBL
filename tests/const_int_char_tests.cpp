#include "test.h"
#include <CLBL/clbl.h>
#include "int_char_definitions.h"

#include<iostream>
#include<memory>

using namespace clbl::tests;
using namespace clbl;

void const_int_char_tests() {

#ifdef CLBL_CONST_INT_CHAR_TESTS
    std::cout << "running CLBL_CONST_INT_CHAR_TESTS" << std::endl;

    const auto const_int_char_object = const_int_char_struct{};

    auto g = fwrap(&int_char_func);

    {
        auto f = fwrap(&const_int_char_object);
        auto h = fwrap(&const_int_char_object, &const_int_char_struct::func);

        run_tests(
            f, test_id::const_int_char_struct_op,
            g, test_id::int_char_func,
            h, test_id::const_int_char_struct_func,
            1, 'c');
    }
    {
        auto f = fwrap(const_int_char_object);
        auto h = fwrap(const_int_char_object, &const_int_char_struct::func);

        run_tests(
            f, test_id::const_int_char_struct_op,
            g, test_id::int_char_func,
            h, test_id::const_int_char_struct_func,
            1, 'c');
    }

    {
        auto f = fwrap(std::ref(const_int_char_object));
        auto h = fwrap(std::ref(const_int_char_object), &const_int_char_struct::func);

        run_tests(
            f, test_id::const_int_char_struct_op,
            g, test_id::int_char_func,
            h, test_id::const_int_char_struct_func,
            1, 'c');
    }
    {
        auto shared_obj = std::make_shared<const_int_char_struct>();
        auto f = fwrap(shared_obj);
        auto h = fwrap(shared_obj, &const_int_char_struct::func);

        run_tests(
            f, test_id::const_int_char_struct_op,
            g, test_id::int_char_func,
            h, test_id::const_int_char_struct_func,
            1, 'c');
    }

#endif
}
