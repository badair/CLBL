#include "test.h"
#include "CLBL/func.h"
#include "int_char_definitions.h"

#include<iostream>

using namespace clbl::tests;
using namespace clbl;

void const_int_char_tests() {

#ifdef CLBL_CONST_INT_CHAR_TESTS
    std::cout << "running CLBL_CONST_INT_CHAR_TESTS" << std::endl;

    const auto const_int_char_object = const_int_char_struct{};

    auto f = func(&const_int_char_object);
    TEST(f(1, 'c') == test_id::const_int_char_struct_op);

    auto g = func(&int_char_func);
    TEST(g(1, 'c') == test_id::int_char_func);

    auto h = func(&const_int_char_object, &decltype(const_int_char_object)::func);
    TEST(h(1, 'c') == test_id::const_int_char_struct_func);

    STATIC_TEST(f.return_type == g.return_type);
    STATIC_TEST(h.return_type == g.return_type);
    STATIC_TEST(f.return_type == hana::type_c<const char*>);

    STATIC_TEST(f.args_tuple == g.args_tuple);
    STATIC_TEST(h.args_tuple == g.args_tuple);
    STATIC_TEST(f.args_tuple == hana::tuple_t<int, char>);

    STATIC_TEST(f.arity == g.arity);
    STATIC_TEST(h.arity == g.arity);
    STATIC_TEST(h.arity == 2_c);

#ifdef CLBL_TEST_CAN_CALL
    STATIC_TEST(f.can_call(1, 'c'));
    STATIC_TEST(g.can_call(1, 'c'));
    STATIC_TEST(h.can_call(1, 'c'));
    STATIC_TEST(!f.can_call());
    STATIC_TEST(!g.can_call(1));
    STATIC_TEST(!h.can_call(1, some_type{}));
#endif

    STATIC_TEST(h.matches(g));
    STATIC_TEST(g.matches(f));
    STATIC_TEST(!g.matches(func(&some_function)));
    STATIC_TEST(!f.matches(func(&some_callable_object)));

#endif
}
