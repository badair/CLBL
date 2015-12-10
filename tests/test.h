#ifndef TEST_H
#define TEST_H

#include <string>
#include <functional>

#include "CLBL/clbl.h"
#include "void_definitions.h"
#include "int_definitions.h"
#include "int_char_definitions.h"

namespace clbl { namespace tests {
        struct some_type {};
        auto some_callable_object = [](some_type) {};
        extern inline void some_function(some_type) {}
}}

#define STATIC_TEST BOOST_HANA_CONSTANT_CHECK
#define TEST BOOST_HANA_CHECK

//#define CLBL_REFERENCE_ARG_TESTS
//#define CLBL_VALUE_TESTS
//#define CLBL_FORWARDING_TESTS
#define CLBL_CONVERSION_TESTS
//#define CLBL_SHARED_PTR_TESTS
//#define CLBL_UNIQUE_PTR_TESTS
//
//#define CLBL_OVERLOAD_TESTS
//
//#define CLBL_INT_CHAR_TESTS
//#define CLBL_INT_TESTS
//#define CLBL_VOID_TESTS
//#define CLBL_CONST_INT_CHAR_TESTS
//#define CLBL_CONST_INT_TESTS
//#define CLBL_CONST_VOID_TESTS
//#define CLBL_CONST_VOLATILE_INT_CHAR_TESTS
//#define CLBL_CONSTABLE_INT_CHAR_TESTS
//#define CLBL_CONSTABLE_INT_TESTS
//#define CLBL_CONSTABLE_VOID_TESTS
//#define CLBL_VOLATILE_INT_CHAR_TESTS
//#define CLBL_VOLATILE_VOID_TESTS

#define CLBL_STATIC_ASSERT_DEEP_CONST(x) static_assert(std::remove_reference_t<decltype(x)>::clbl_is_deep_const, "")
#define CLBL_STATIC_ASSERT_DEEP_VOLATILE(x) static_assert(std::remove_reference_t<decltype(x)>::clbl_is_deep_volatile, "")
#define CLBL_STATIC_ASSERT_NOT_DEEP_CONST(x) static_assert(!std::remove_reference_t<decltype(x)>::clbl_is_deep_const, "")
#define CLBL_STATIC_ASSERT_NOT_DEEP_VOLATILE(x) static_assert(!std::remove_reference_t<decltype(x)>::clbl_is_deep_volatile, "")

template<typename F, typename G, typename H, typename... Args>
void run_tests(
    F& f, const char* f_result,
    G& g, const char* g_result,
    H& h, const char* h_result,
    Args... args) {

    TEST(f(args...) == f_result);
    TEST(g(args...) == g_result);
    TEST(h(args...) == h_result);

    static_assert(std::is_same<typename F::return_t, typename G::return_t>::value, "std::is_same<F::return_t, G::return_t>::value");
    static_assert(std::is_same<typename G::return_t, typename H::return_t>::value, "std::is_same<G::return_t, H::return_t>::value");
    static_assert(std::is_same<typename H::return_t, typename F::return_t>::value, "std::is_same<H::return_t, F::return_t>::value");

    static_assert(std::is_same<typename F::args_t, typename G::args_t>::value, "std::is_same<F::args_t, G::args_t>::value");
    static_assert(std::is_same<typename G::args_t, typename H::args_t>::value, "std::is_same<G::args_t, H::args_t>::value");
    static_assert(std::is_same<typename H::args_t, typename F::args_t>::value, "std::is_same<H::args_t, F::args_t>::value");

    auto f_std_func = convert_to<std::function>(f);
    auto g_std_func = convert_to<std::function>(g);
    auto h_std_func = convert_to<std::function>(h);

    TEST(f_std_func(args...) == f_result);
    TEST(g_std_func(args...) == g_result);
    TEST(h_std_func(args...) == h_result);
}

template<typename F, typename G, typename H>
void run_tests(
    F& f, std::string f_result,
    G& g, std::string g_result,
    H& h, std::string h_result) {

    TEST(f() == f_result);
    TEST(g() == g_result);
    TEST(h() == h_result);

    static_assert(std::is_same<typename F::return_t, typename G::return_t>::value, "std::is_same<F::return_t, G::return_t>::value");
    static_assert(std::is_same<typename G::return_t, typename H::return_t>::value, "std::is_same<G::return_t, H::return_t>::value");
    static_assert(std::is_same<typename H::return_t, typename F::return_t>::value, "std::is_same<H::return_t, F::return_t>::value");

    static_assert(std::is_same<typename F::args_t, typename G::args_t>::value, "std::is_same<F::args_t, G::args_t>::value");
    static_assert(std::is_same<typename G::args_t, typename H::args_t>::value, "std::is_same<G::args_t, H::args_t>::value");
    static_assert(std::is_same<typename H::args_t, typename F::args_t>::value, "std::is_same<H::args_t, F::args_t>::value");

    auto f_std_func = convert_to<std::function>(f);
    auto g_std_func = convert_to<std::function>(g);
    auto h_std_func = convert_to<std::function>(h);

    static_assert(std::is_same<std::function<const char*(void)>, decltype(f_std_func)>::value, "convert_to<std::function>(f)");
    static_assert(std::is_same<std::function<const char*(void)>, decltype(g_std_func)>::value, "convert_to<std::function>(g)");
    static_assert(std::is_same<std::function<const char*(void)>, decltype(h_std_func)>::value, "convert_to<std::function>(h)");

    TEST(f_std_func(args...) == f_result);
    TEST(g_std_func(args...) == g_result);
    TEST(h_std_func(args...) == h_result);
}

#endif