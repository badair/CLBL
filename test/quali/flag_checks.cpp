/*!
@file

@copyright Barrett Adair 2015
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)

*/

#include <quali/quali_macros.hpp>
#include <cassert>
#include <string>

using namespace quali;

#ifndef QUALI_RUNTIME_ASSERT
#undef NDEBUG
#define QUALI_RUNTIME_ASSERT(...) assert(__VA_ARGS__)
#endif

#if CLBL_TEST_PART == 1
#define QUALI_TEST_CASE QUALI_DEFAULT
#define QUALI_EXPECTED_CV QUALI_DEFAULT
#define QUALI_EXPECTED_REF QUALI_DEFAULT
#endif

#if CLBL_TEST_PART == 2
#define QUALI_TEST_CASE const
#define QUALI_EXPECTED_CV const
#define QUALI_EXPECTED_REF QUALI_DEFAULT
#endif

#if CLBL_TEST_PART == 3
#define QUALI_TEST_CASE volatile 
#define QUALI_EXPECTED_CV volatile
#define QUALI_EXPECTED_REF QUALI_DEFAULT
#endif

#if CLBL_TEST_PART == 4
#define QUALI_TEST_CASE const, volatile
#define QUALI_EXPECTED_CV const, volatile
#define QUALI_EXPECTED_REF QUALI_DEFAULT
#endif

#if CLBL_TEST_PART == 5
#define QUALI_TEST_CASE &
#define QUALI_EXPECTED_CV QUALI_DEFAULT
#define QUALI_EXPECTED_REF &
#endif

#if CLBL_TEST_PART == 6
#define QUALI_TEST_CASE &&
#define QUALI_EXPECTED_CV QUALI_DEFAULT
#define QUALI_EXPECTED_REF &&
#endif

#if CLBL_TEST_PART == 7
#define QUALI_TEST_CASE const, &
#define QUALI_EXPECTED_CV const
#define QUALI_EXPECTED_REF &
#endif

#if CLBL_TEST_PART == 8
#define QUALI_TEST_CASE const, &&
#define QUALI_EXPECTED_CV const
#define QUALI_EXPECTED_REF &&
#endif

#if CLBL_TEST_PART == 9
#define QUALI_TEST_CASE , volatile, &
#define QUALI_EXPECTED_CV , volatile
#define QUALI_EXPECTED_REF , &
#endif

#if CLBL_TEST_PART == 10
#define QUALI_TEST_CASE volatile, &&
#define QUALI_EXPECTED_CV volatile
#define QUALI_EXPECTED_REF &&
#endif

#if CLBL_TEST_PART == 11
#define QUALI_TEST_CASE const, volatile, &
#define QUALI_EXPECTED_CV const, volatile
#define QUALI_EXPECTED_REF &
#endif

#if CLBL_TEST_PART == 12
#define QUALI_TEST_CASE const, volatile, &&
#define QUALI_EXPECTED_CV const, volatile
#define QUALI_EXPECTED_REF &&
#endif

constexpr auto expected_cv = QUALI(FLAGS, QUALI_EXPECTED_CV);
constexpr auto expected_ref = QUALI(FLAGS, QUALI_EXPECTED_REF);

using test_type = qualified_type<int, QUALI(FLAGS, QUALI_TEST_CASE)>;

constexpr auto cv_flags = cv_of<test_type>::value;
constexpr auto ref_flags = ref_of<test_type>::value;

static_assert(cv_flags == expected_cv, "");
static_assert(ref_flags == expected_ref, "");
static_assert((cv_flags | ref_flags) == (expected_cv | expected_ref), "");

int main() { return 0; }