#ifndef TEST_H
#define TEST_H

#include <string>
#include <cstring>
#include <functional>
#include <cassert>

#include <CLBL/clbl.h>
#include "void_definitions.h"
#include "int_definitions.h"
#include "int_char_definitions.h"

namespace clbl { namespace tests {
        struct some_type {};
        auto some_callable_object = [](some_type) {};
        extern inline void some_function(some_type) {}
}}

#define STATIC_TEST(expr) static_assert(expr, "")
#define TEST(expr) assert(expr)

#define CLBL_REFERENCE_ARG_TESTS
#define CLBL_VALUE_TESTS
#define CLBL_FORWARDING_TESTS
#define CLBL_CV_TESTS
#define CLBL_SHARED_PTR_TESTS
#define CLBL_UNIQUE_PTR_TESTS
#define CLBL_OVERLOAD_TESTS
#define CLBL_INT_CHAR_TESTS
#define CLBL_INT_TESTS
#define CLBL_VOID_TESTS
#define CLBL_CONST_INT_CHAR_TESTS
#define CLBL_CONST_INT_TESTS
#define CLBL_CONST_VOID_TESTS
#define CLBL_CONST_VOLATILE_INT_CHAR_TESTS
#define CLBL_CONSTABLE_INT_CHAR_TESTS
#define CLBL_CONSTABLE_INT_TESTS
#define CLBL_CONSTABLE_VOID_TESTS
#define CLBL_VOLATILE_INT_CHAR_TESTS
#define CLBL_VOLATILE_VOID_TESTS

template<typename T>
struct start_of_type_name {
    static const char* end_of_type_name() {
        return __PRETTY_FUNCTION__;
    }
};

#define CLBL_PRINT_NAME_AND_TYPE(x) \
std::cout << "(line " << __LINE__ << ")" << \
#x << ": " << start_of_type_name<decltype(x)>::end_of_type_name() << \
std::endl << std::endl

template<typename F, typename G, typename H, typename... Args>
void run_tests(
    F& f, const char* f_result,
    G& g, const char* g_result,
    H& h, const char* h_result,
    Args... args) {

    TEST(f(args...) == f_result);
    TEST(g(args...) == g_result);
    TEST(h(args...) == h_result);

    static_assert(std::is_same<typename F::return_type, typename G::return_type>::value, "std::is_same<F::return_type, G::return_type>::value");
    static_assert(std::is_same<typename G::return_type, typename H::return_type>::value, "std::is_same<G::return_type, H::return_type>::value");
    static_assert(std::is_same<typename H::return_type, typename F::return_type>::value, "std::is_same<H::return_type, F::return_type>::value");

    static_assert(std::is_same<typename F::arg_types, typename G::arg_types>::value, "std::is_same<F::arg_types, G::arg_types>::value");
    static_assert(std::is_same<typename G::arg_types, typename H::arg_types>::value, "std::is_same<G::arg_types, H::arg_types>::value");
    static_assert(std::is_same<typename H::arg_types, typename F::arg_types>::value, "std::is_same<H::arg_types, F::arg_types>::value");

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

    static_assert(std::is_same<typename F::return_type, typename G::return_type>::value, "std::is_same<F::return_type, G::return_type>::value");
    static_assert(std::is_same<typename G::return_type, typename H::return_type>::value, "std::is_same<G::return_type, H::return_type>::value");
    static_assert(std::is_same<typename H::return_type, typename F::return_type>::value, "std::is_same<H::return_type, F::return_type>::value");

    static_assert(std::is_same<typename F::arg_types, typename G::arg_types>::value, "std::is_same<F::arg_types, G::arg_types>::value");
    static_assert(std::is_same<typename G::arg_types, typename H::arg_types>::value, "std::is_same<G::arg_types, H::arg_types>::value");
    static_assert(std::is_same<typename H::arg_types, typename F::arg_types>::value, "std::is_same<H::arg_types, F::arg_types>::value");

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