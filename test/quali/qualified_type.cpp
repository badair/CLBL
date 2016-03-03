/*!
@file

@copyright Barrett Adair 2015
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)

*/

#include <quali/quali.hpp>

using namespace quali;

#define QUALI_ASSERT_SAME(...) \
    static_assert(std::is_same<__VA_ARGS__>::value, \
        "QUALI_ASSERT_SAME("#__VA_ARGS__");")

struct foo {};

int main() {
    QUALI_ASSERT_SAME(int, qualified_type<int, default_>);
    QUALI_ASSERT_SAME(int const, qualified_type<int, const_>);
    QUALI_ASSERT_SAME(int volatile, qualified_type<int, volatile_>);
    QUALI_ASSERT_SAME(int const volatile, qualified_type<int, const_ | volatile_>);

    QUALI_ASSERT_SAME(int &, qualified_type<int, lref_>);
    QUALI_ASSERT_SAME(int const &, qualified_type<int, clref_>);
    QUALI_ASSERT_SAME(int volatile &, qualified_type<int, lref_ | volatile_>);
    QUALI_ASSERT_SAME(int const volatile &, qualified_type<int, clref_ | volatile_>);

    QUALI_ASSERT_SAME(int &&, qualified_type<int, rref_>);
    QUALI_ASSERT_SAME(int const &&, qualified_type<int, rref_ | const_>);
    QUALI_ASSERT_SAME(int volatile &&, qualified_type<int, rref_ | volatile_>);
    QUALI_ASSERT_SAME(int const volatile &&, qualified_type<int, rref_ | const_ | volatile_>);

    return 0;
}