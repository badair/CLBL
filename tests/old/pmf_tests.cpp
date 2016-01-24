/*


Copyright Barrett Adair 2015
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)

*/

/*
#include <iostream>
#include <tuple>
#include <functional>
#include <memory>
#include <iostream>
#include <cassert>
#include <CLBL/pmf.h>

using namespace clbl::qflags;
namespace pmf = clbl::pmf;

enum class overload {
    lref,
    const_lref,
    rref,
};

struct foo {
    overload func() & {
        return overload::lref;
    }

    overload func() && {
        return overload::rref;
    }

    overload func() const & {
        return overload::const_lref;
    }

    static int copy_count;
    static int move_count;

    foo() = default;
    foo(foo&&) { ++move_count; }
    foo(const foo&) { ++copy_count; }
    foo(foo&) { ++copy_count; }
};

int foo::copy_count = 0;
int foo::move_count = 0;


struct bar {

    template<typename T>
    overload func(T&& f) & {
        return std::forward<T>(f).func();
    }

    template<typename T>
    overload func(T&& f) const & {
        return std::forward<T>(f).func();
    }

    template<typename T>
    overload func(T&& f) const && {
        return std::forward<T>(f).func();
    }

    static int copy_count;
    static int move_count;

    bar() = default;
    bar(bar&&) { ++move_count; }
    bar(const bar&) { ++copy_count; }
    bar(foo&) { ++copy_count; }
};

int bar::copy_count = 0;
int bar::move_count = 0;

struct bar2 {

    overload func(const foo& f) const &{
        return f.func();
    }

    overload func(const foo& f) const && {
        return f.func();
    }

    static int copy_count;
    static int move_count;

    bar2() = default;
    bar2(bar2&&) { ++move_count; }
    bar2(const bar2&) { ++copy_count; }
    bar2(foo&) { ++copy_count; }
};

int bar2::copy_count = 0;
int bar2::move_count = 0;

template<typename T, typename U>
constexpr auto is_same = std::is_same<T, U>::value;

template<typename From, typename To>
constexpr auto can_convert = std::is_convertible<From, To>::value;
*/

int main() {

/*
    auto my_bar = bar{};
    auto my_bar2 = bar2{};
    {
        auto f = fwrap<overload(bar::*)(const foo&) &>(&bar::func);
        assert(f(bar{}, foo{}) == overload::const_lref);
    }
    {
        auto f = pmf::define<overload(const foo&) const &>::wrap<bar>(&bar::func);
        assert(f.invoke(my_bar, foo{}) == overload::const_lref);
        using wrapper = decltype(f);
        static_assert(is_same<pmf::args<wrapper>, std::tuple<const foo&> >, "");
        static_assert(pmf::qualifiers<wrapper> == (const_ | lvalue_reference_), "");
        static_assert(is_same<pmf::return_type<wrapper>, overload>, "");
        static_assert(is_same<pmf::abominable<wrapper>, overload(const foo&) const &>, "");
        static_assert(is_same<pmf::decay<wrapper>, overload(bar::*)(const foo&)>, "");
        static_assert(is_same<pmf::type<wrapper>, overload(bar::*)(const foo&) const &>, "");
        auto g = f.reseat(&bar2::func);
        static_assert(is_same<decltype(g), decltype(pmf::define<overload(const foo&)const &>::wrap(&bar2::func))>, "");
        assert(g.invoke(my_bar2, foo{}) == overload::const_lref);
    }
    {
        auto f = pmf::define<overload(const foo&)const &&>::wrap(&bar2::func);
        assert(f.invoke(bar2{}, foo{}) == overload::const_lref);
        using wrapper = decltype(f);
        static_assert(is_same<pmf::args<wrapper>, std::tuple<const foo&> >, "");
        static_assert(pmf::qualifiers<wrapper> == (const_ | rvalue_reference_), "");
        static_assert(is_same<pmf::abominable<wrapper>, overload(const foo&) const &&>, "");
        static_assert(is_same<pmf::decay<wrapper>, overload(bar2::*)(const foo&)>, "");
        static_assert(is_same<pmf::type<wrapper>, overload(bar2::*)(const foo&) const &&>, "");
    }
*/
    
    return 0;
}
