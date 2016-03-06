/*

Copyright Barrett Adair 2015
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)

*/

#include <iostream>
#include <functional>
#include <memory>
#include <type_traits>
#include <cassert>
#include <CLBL/clbl.hpp>
#include <quali/quali_macros.hpp>

#ifndef CLBL_RUNTIME_ASSERT
#undef NDEBUG
#define CLBL_RUNTIME_ASSERT(...) assert(__VA_ARGS__)
#endif //#ifndef CLBL_RUNTIME_ASSERT

#ifndef CLBL_STATIC_ASSERT
#undef NDEBUG
#define CLBL_STATIC_ASSERT(...) static_assert(__VA_ARGS__, #__VA_ARGS__)
#endif //#ifndef CLBL_STATIC_ASSERT

struct foo {

    static int copy_count;
    static int move_count;

    mutable bool was_moved;
    mutable bool was_copied;

    static void reset() {
        copy_count = 0;
        move_count = 0;
    }

    foo() = default;

    foo(const foo& f) {
        ++copy_count; f.was_copied = true;
    }

    foo(const volatile foo& f) {
        ++copy_count; f.was_copied = true;
    }

    foo(foo&& f) {
        ++move_count; f.was_moved = true;
    }

    std::integral_constant<quali::flags, QUALI(FLAGS, &)> operator()() & { return{}; }
    std::integral_constant<quali::flags, QUALI(FLAGS, &&)> operator()() && { return{}; }
    std::integral_constant<quali::flags, QUALI(FLAGS, const, &)> operator()() const & { return{}; }
    std::integral_constant<quali::flags, QUALI(FLAGS, const, &&)> operator()() const && { return{}; }
    std::integral_constant<quali::flags, QUALI(FLAGS, volatile, &)> operator()() volatile & { return{}; }
    std::integral_constant<quali::flags, QUALI(FLAGS, volatile, &&)> operator()() volatile && { return{}; }
    std::integral_constant<quali::flags, QUALI(FLAGS, const, volatile, &)> operator()() const volatile & { return{}; }
    std::integral_constant<quali::flags, QUALI(FLAGS, const, volatile, &&)> operator()() const volatile && { return{}; }
};

int foo::copy_count = 0;
int foo::move_count = 0;

int main() {

    //fwrap by value
#if CLBL_TEST_PART == 1
{
    auto f = foo{};
    auto g = clbl::fwrap(f);
    auto result = g();

    CLBL_STATIC_ASSERT(std::is_same<decltype(f()), decltype(g())>::value);
    CLBL_STATIC_ASSERT(decltype(result)::value == QUALI(FLAGS, &));
    CLBL_RUNTIME_ASSERT(f.was_copied);
    CLBL_RUNTIME_ASSERT(!f.was_moved);
}
#endif

//fwrap by pointer
#if CLBL_TEST_PART == 2
{
    auto f = foo{};
    auto g = clbl::fwrap(&f);
    auto result = g();

    CLBL_STATIC_ASSERT(std::is_same<decltype(f()), decltype(g())>::value);
    CLBL_STATIC_ASSERT(decltype(result)::value == QUALI(FLAGS, &));
    CLBL_RUNTIME_ASSERT(!f.was_copied);
    CLBL_RUNTIME_ASSERT(!f.was_moved);
    CLBL_RUNTIME_ASSERT(foo::copy_count == 0);
    CLBL_RUNTIME_ASSERT(foo::move_count == 0);
}
#endif

//fwrap by reference
#if CLBL_TEST_PART == 3
{
    auto f = foo{};
    auto g = clbl::fwrap(std::ref(f));
    auto result = g();

    CLBL_STATIC_ASSERT(std::is_same<decltype(f()), decltype(g())>::value);
    CLBL_STATIC_ASSERT(decltype(result)::value == QUALI(FLAGS, &));
    CLBL_RUNTIME_ASSERT(!f.was_copied);
    CLBL_RUNTIME_ASSERT(!f.was_moved);
    CLBL_RUNTIME_ASSERT(foo::copy_count == 0);
    CLBL_RUNTIME_ASSERT(foo::move_count == 0);
}
#endif

//fwrap by const reference
#if CLBL_TEST_PART == 4
{
    auto f = foo{};
    auto g = clbl::fwrap(std::cref(f));
    auto result = g();

    CLBL_STATIC_ASSERT(decltype(result)::value == QUALI(FLAGS, const, &));
    CLBL_RUNTIME_ASSERT(!f.was_copied);
    CLBL_RUNTIME_ASSERT(!f.was_moved);
    CLBL_RUNTIME_ASSERT(foo::copy_count == 0);
    CLBL_RUNTIME_ASSERT(foo::move_count == 0);
}
#endif

//fwrap by prvalue
#if CLBL_TEST_PART == 5
{
    auto g = clbl::fwrap(foo{});
    auto result = g();

    //lvalue-reference-qualified overload is called because we don't want to re-move. 
    CLBL_STATIC_ASSERT(decltype(result)::value == QUALI(FLAGS, &));

#ifndef CLBL_GCC_5_REF_OVERLOAD_WORKAROUND
    //You can move the callable if you want to call the rvalue-reference-qualified overload.
    auto move_result = static_cast<decltype(g)&&>(g)();
    CLBL_STATIC_ASSERT(decltype(move_result)::value == QUALI(FLAGS, &&));
#endif

}
#endif

//fwrap by xvalue
#if CLBL_TEST_PART == 6
{
    auto f = foo{};
    auto g = clbl::fwrap(std::move(f));

    CLBL_RUNTIME_ASSERT(f.was_moved);

    auto result = g();

    //lvalue-reference-qualified overload is called because we don't want to re-move. 
    CLBL_STATIC_ASSERT(decltype(result)::value == QUALI(FLAGS, &));
 
#ifndef CLBL_GCC_5_REF_OVERLOAD_WORKAROUND

    auto move_result = std::move(g)();
    CLBL_STATIC_ASSERT(decltype(move_result)::value == QUALI(FLAGS, &&));

#endif

}
#endif

return 0;
}
