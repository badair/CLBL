#include "test.h"
#include "CLBL/func.h"
#include "int_char_definitions.h"

using namespace clbl::tests;
using namespace clbl;

#include<iostream>
#include<type_traits>

void overload_tests() {

#ifdef CLBL_OVERLOAD_TESTS
    std::cout << "running CLBL_OVERLOAD_TESTS" << std::endl;

    auto overloaded_object = overloaded_int_char_struct{};

    auto h = harden<const char*(int, char)>(func(&overloaded_object));

    TEST(h(1, 'c') == test_id::overloaded_int_char_struct_op);

    //calling clbl::func with a result from clbl::func
    auto identity_func = harden<const char*(int, char)>(func(&h));

    TEST(identity_func(1, 'c') == h(1, 'c'));

#endif
}