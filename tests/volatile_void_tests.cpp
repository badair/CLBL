#include "test.h"
#include "CLBL/func.h"
#include "void_definitions.h"

#include <iostream>

using namespace clbl::tests;
using namespace clbl;

void volatile_void_tests() {

#ifdef CLBL_VOLATILE_VOID_TESTS
    std::cout << "running CLBL_VOLATILE_VOID_TESTS" << std::endl;

    volatile_void_struct volatile_void_object{};

    auto f = func(&volatile_void_object);
    TEST(f() == test_id::volatile_void_struct_op);

    auto g = func(&void_func);
    TEST(g() == test_id::void_func);

    auto h = func(&volatile_void_object, &decltype(volatile_void_object)::func);
    TEST(h() == test_id::volatile_void_struct_func);

    STATIC_TEST(f.return_type == g.return_type);
    STATIC_TEST(h.return_type == g.return_type);
    STATIC_TEST(f.return_type == hana::type_c<const char*>);

    STATIC_TEST(f.args_tuple == g.args_tuple);
    STATIC_TEST(h.args_tuple == g.args_tuple);
    STATIC_TEST(f.args_tuple == hana::make_tuple());

    STATIC_TEST(f.arity == g.arity);
    STATIC_TEST(h.arity == g.arity);
    STATIC_TEST(h.arity == 0_c);

#ifdef CLBL_TEST_CAN_CALL
    STATIC_TEST(!f.can_call(1));
    STATIC_TEST(!g.can_call(1));
    STATIC_TEST(!h.can_call(1));
    STATIC_TEST(f.can_call());
    STATIC_TEST(g.can_call());
    STATIC_TEST(h.can_call());
#endif

    STATIC_TEST(h.matches(g));
    STATIC_TEST(g.matches(f));
    STATIC_TEST(!g.matches(func(&int_char_func)));

    auto int_object = int_struct{};
    STATIC_TEST(!f.matches(func(&int_object)));

#endif
}