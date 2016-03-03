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
#define QUALI_TEST_TOKEN_STR ""
#endif

#if CLBL_TEST_PART == 2
#define QUALI_TEST_CASE const
#define QUALI_TEST_TOKEN_STR "const"
#endif

#if CLBL_TEST_PART == 3
#define QUALI_TEST_CASE volatile
#define QUALI_TEST_TOKEN_STR "volatile"
#endif

#if CLBL_TEST_PART == 4
#define QUALI_TEST_CASE const, volatile
#define QUALI_TEST_TOKEN_STR "const volatile"
#endif

#if CLBL_TEST_PART == 5
#define QUALI_TEST_CASE &
#define QUALI_TEST_TOKEN_STR "&"
#endif

#if CLBL_TEST_PART == 6
#define QUALI_TEST_CASE &&
#define QUALI_TEST_TOKEN_STR "&&"
#endif

#if CLBL_TEST_PART == 7
#define QUALI_TEST_CASE const, &
#define QUALI_TEST_TOKEN_STR "const &"
#endif

#if CLBL_TEST_PART == 8
#define QUALI_TEST_CASE const, &&
#define QUALI_TEST_TOKEN_STR "const &&"
#endif

#if CLBL_TEST_PART == 9
#define QUALI_TEST_CASE volatile, &
#define QUALI_TEST_TOKEN_STR "volatile &"
#endif

#if CLBL_TEST_PART == 10
#define QUALI_TEST_CASE volatile, &&
#define QUALI_TEST_TOKEN_STR "volatile &&"
#endif

#if CLBL_TEST_PART == 11
#define QUALI_TEST_CASE const, volatile, &
#define QUALI_TEST_TOKEN_STR "const volatile &"
#endif

#if CLBL_TEST_PART == 12
#define QUALI_TEST_CASE const, volatile, &&
#define QUALI_TEST_TOKEN_STR "const volatile &&"
#endif

int main() {

    static_assert(
        std::is_same<
        int QUALI(TOKENS, QUALI_TEST_CASE),
        qualified_type<int, QUALI(FLAGS, QUALI_TEST_CASE)>
        >::value,
        ""
        );

    QUALI_RUNTIME_ASSERT(
        std::string{ QUALI_STR(TOKENS, QUALI_TEST_CASE) }
        == std::string{ QUALI_TEST_TOKEN_STR }
    );

    return 0;
}