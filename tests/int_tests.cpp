#include "test.h"
#include "CLBL/func.h"
#include "int_definitions.h"

#include<iostream>

using namespace clbl::tests;
using namespace clbl;

void int_tests() {

#ifdef CLBL_INT_TESTS
    std::cout << "running CLBL_INT_TESTS" << std::endl;

    int_struct int_object{};
    int_char_struct int_char_object{};

    auto f = func(&int_object);
    TEST(f(1) == test_id::int_struct_op);

    auto g = func(&int_func);
    TEST(g(1) == test_id::int_func);

    auto h = func(&int_object, &decltype(int_object)::func);
    TEST(h(1) == test_id::int_struct_func);

    STATIC_TEST(f.return_type == g.return_type);
    STATIC_TEST(h.return_type == g.return_type);
    STATIC_TEST(f.return_type == hana::type_c<const char*>);

    STATIC_TEST(f.args_tuple == g.args_tuple);
    STATIC_TEST(h.args_tuple == g.args_tuple);
    STATIC_TEST(f.args_tuple == hana::tuple_t<int>);

    STATIC_TEST(f.arity == g.arity);
    STATIC_TEST(h.arity == g.arity);
    STATIC_TEST(h.arity == 1_c);

#ifdef CLBL_TEST_CAN_CALL
    STATIC_TEST(f.can_call(1));
    STATIC_TEST(g.can_call(1));
    STATIC_TEST(h.can_call(1));
    STATIC_TEST(!f.can_call(some_type{}));
    STATIC_TEST(!g.can_call(some_type{}));
    STATIC_TEST(!h.can_call(some_type{}));
#endif

    STATIC_TEST(h.matches(g));
    STATIC_TEST(g.matches(f));
    STATIC_TEST(!g.matches(func(&void_func)));
    STATIC_TEST(!f.matches(func(&int_char_object)));

#endif
}