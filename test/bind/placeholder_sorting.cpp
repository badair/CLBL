/*

Copyright Barrett Adair 2016
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)

*/

#include <cassert>
#include <type_traits>
#include <functional>
#include <iostream>
#include <sstream>
#include <memory>
#include <iostream>
#include <cstdint>
#include <CLBL/clbl.hpp>

#ifndef CLBL_RUNTIME_ASSERT
#undef NDEBUG
#define CLBL_RUNTIME_ASSERT(...) assert(__VA_ARGS__)
#endif //#ifndef CLBL_RUNTIME_ASSERT

#ifndef CLBL_STATIC_ASSERT
#undef NDEBUG
#define CLBL_STATIC_ASSERT(...) static_assert(__VA_ARGS__, #__VA_ARGS__)
#endif //#ifndef CLBL_STATIC_ASSERT

using namespace std::placeholders;

struct Letter {
    virtual operator const char*() const volatile = 0;
};

#define CLBL_DEFINE_TEST_LETTER(L) \
struct L : Letter { operator const char*() const volatile override { return #L; } }

CLBL_DEFINE_TEST_LETTER(A);
CLBL_DEFINE_TEST_LETTER(B);
CLBL_DEFINE_TEST_LETTER(C);
CLBL_DEFINE_TEST_LETTER(D);
CLBL_DEFINE_TEST_LETTER(E);
CLBL_DEFINE_TEST_LETTER(F);
CLBL_DEFINE_TEST_LETTER(G);

auto letters(
    const Letter& l1,
    const Letter& l2,
    const Letter& l3,
    const Letter& l4,
    const Letter& l5,
    const Letter& l6,
    const Letter& l7
) {
    auto ss = std::stringstream{};
    ss << l1 << l2 << l3 << l4 << l5 << l6 << l7;
    return ss.str();
}

auto ordered_letters(A a, B b, C c, D d, E e, F f, G g) {
    auto ss = std::stringstream{};
    ss << a << b << c << d << e << f << g;
    return ss.str();
}

int main() {

    auto a = A{};
    auto b = B{};
    auto c = C{};
    auto d = D{};
    auto e = E{};
    auto f = F{};
    auto g = G{};

    CLBL_RUNTIME_ASSERT(letters(a, b, c, d, e, f, g) == "ABCDEFG");
    CLBL_RUNTIME_ASSERT(ordered_letters(a, b, c, d, e, f, g) == "ABCDEFG");

    {
        auto x = clbl::fwrap(&ordered_letters).bind(_1, _2, _3, _4, _5, _6, _7);
        using expected_args = std::tuple<A, B, C, D, E, F, G>;
        static_assert(std::is_same<decltype(x)::arg_types, expected_args>::value, "");
        CLBL_RUNTIME_ASSERT(x(a, b, c, d, e, f, g) == "ABCDEFG");
    }
    {
        auto x = clbl::fwrap(&ordered_letters).bind(a, b, c, _1, _2, _3, _4);
        using expected_args = std::tuple<D, E, F, G>;
        static_assert(std::is_same<decltype(x)::arg_types, expected_args>::value, "");
        CLBL_RUNTIME_ASSERT(x(d, e, f, g) == "ABCDEFG");
    }
    {
        auto x = clbl::fwrap(&ordered_letters).bind(_7, _6, _5, _4, _3, _2, _1);
        using expected_args = std::tuple<G, F, E, D, C, B, A>;
        static_assert(std::is_same<decltype(x)::arg_types, expected_args>::value, "");
        CLBL_RUNTIME_ASSERT(x(g, f, e, d, c, b, a) == "ABCDEFG");
    }
    {
        auto x = clbl::fwrap(&ordered_letters).bind(a, b, c, _4, _3, _2, _1);
        using expected_args = std::tuple<G, F, E, D>;
        static_assert(std::is_same<decltype(x)::arg_types, expected_args>::value, "");
        CLBL_RUNTIME_ASSERT(x(g, f, e, d) == "ABCDEFG");
    }
    {
        auto x = clbl::fwrap(&ordered_letters).bind(_4, _3, _2, _1, e, f, g);
        using expected_args = std::tuple<D, C, B, A>;
        static_assert(std::is_same<decltype(x)::arg_types, expected_args>::value, "");
        CLBL_RUNTIME_ASSERT(x(d, c, b, a) == "ABCDEFG");
    }

    {
        //We can't actually call these, because they aren't valid
        //binds (which the compiler doesn't know until we try to call it,
        //in the current implementation). Here we are testing the
        //correctness of the arg_types alias in binding_wrapper.
        {
            auto x = clbl::fwrap(&ordered_letters).bind(_1, _1, _3, _3, _2, _1, _2);
            using expected_args = std::tuple<A, E, C>;
            static_assert(std::is_same<decltype(x)::arg_types, expected_args>::value, "");
        }
        {
            auto x = clbl::fwrap(&ordered_letters).bind(_4, _1, _2, _3, _1, _2, _4);
            using expected_args = std::tuple<B, C, D, A>;
            static_assert(std::is_same<decltype(x)::arg_types, expected_args>::value, "");
        }
    }

    {
        auto x = clbl::fwrap(&letters).bind(_1, _1, _3, _3, _2, a, b);
        CLBL_RUNTIME_ASSERT(x(a, e, c) == "AACCEAB");
        using expected_args = std::tuple<const Letter&, const Letter&, const Letter&>;
        static_assert(std::is_same<decltype(x)::arg_types, expected_args>::value, "");
    }

    return 0;
}