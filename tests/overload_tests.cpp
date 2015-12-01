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

    auto normal = force<const char*(int, char)>::func(&overloaded_object);
    auto c = force<const char*(int, char) const>::func(&overloaded_object);
    auto v = force<const char*(int, char) volatile>::func(&overloaded_object);
    auto cv = force<const char*(int, char) const volatile>::func(&overloaded_object);

    TEST(normal(1, 'c') == test_id::overloaded_int_char_struct_op);
    TEST(c(1, 'c') == test_id::overloaded_int_char_struct_op_c);
    TEST(v(1, 'c') == test_id::overloaded_int_char_struct_op_v);
    TEST(cv(1, 'c') == test_id::overloaded_int_char_struct_op_cv);

    STATIC_TEST(c.matches(v) && v.matches(cv) && cv.matches(normal) && normal.matches(c));

    //calling clbl::func with a result from clbl::func
    auto identity_func = force<const char*(int, char)>::func(&normal);

    TEST(identity_func(1, 'c') == normal(1, 'c'));
    //this test fails, due to perfect forwarding: STATIC_TEST(normal.matches(identity_func) && identity_func.matches(normal));

    //creating a func from a func
    auto identity_func_decayed = force<const char*(int, char)>::func(&c);
    TEST(identity_func_decayed(1, 'c') == c(1, 'c'));
    //this test fails, due to perfect forwarding: STATIC_TEST(normal.matches(identity_func_decayed) && identity_func_decayed.matches(c));

#endif
}