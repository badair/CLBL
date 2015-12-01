#ifndef TEST_H
#define TEST_H

#include "CLBL/func.h"
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

//todo need to rework can_call implementation (or review failing test cases)
//#define CLBL_TEST_CAN_CALL

#define CLBL_REFERENCE_ARG_TESTS
#define CLBL_VALUE_TESTS
#define CLBL_FORWARDING_TESTS
#define CLBL_CONVERSION_TESTS
#define CLBL_SHARED_PTR_TESTS

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

#endif