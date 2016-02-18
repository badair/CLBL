/*

Copyright Barrett Adair 2015
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)

*/

#ifndef TEST_H
#define TEST_H

/*
currently, successful compilation of the test cases
depends on the existence of the asserts, because otherwise
the parameter packs are not expanded
*/
#undef NDEBUG

#include <string>
#include <cstring>
#include <functional>
#include <cassert>

#include <CLBL/clbl.hpp>
#include "void_definitions.hpp"
#include "int_definitions.hpp"
#include "int_char_definitions.hpp"

namespace clbl { namespace test {
        struct some_type {};
        auto some_callable_object = [](some_type) {};
        extern inline void some_function(some_type) {}
}}

using namespace clbl;

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
    static std::string end_of_type_name() {
        return __PRETTY_FUNCTION__;
    }
};

#define CLBL_PRINT_NAME_AND_TYPE(x) \
std::cout << "(line " << __LINE__ << ")" << \
#x << ": " << start_of_type_name<decltype(x)>::end_of_type_name() << \
std::endl << std::endl

using test_result_type = std::string;


template<typename F, typename... Args>
void run_basic_tests(F& f, test_result_type expected_result, Args... test_args) {
    assert(f(test_args...) == expected_result);
    static_assert(std::is_same<typename F::return_type , test_result_type>::value, "");
    static_assert(std::is_same<result_of<F> , test_result_type>::value, "");
    static_assert(std::is_same<typename F::arg_types , std::tuple<Args...> >::value, "");
    static_assert(std::is_same<args<F> , std::tuple<Args...> >::value, "");
    auto f_std_func = convert_to<std::function>(f);
    assert(f_std_func(test_args...) == expected_result);
}

template<typename F>
void run_basic_tests(F& f, test_result_type expected_result) {
    assert(f() == expected_result);
    static_assert(std::is_same<typename F::return_type , test_result_type>::value, "");
    static_assert(std::is_same<result_of<F> , test_result_type>::value, "");
    static_assert(std::is_same<typename F::arg_types , std::tuple<> >::value, "");
    static_assert(std::is_same<args<F> , std::tuple<> >::value, "");
    auto f_std_func = convert_to<std::function>(f);
    assert(f_std_func() == expected_result);
}

#endif