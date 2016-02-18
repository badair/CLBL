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
#include <CLBL/clbl.hpp>

using namespace clbl;
using namespace clbl::qflags;

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
    auto func(T&& f) {
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

class constructor_checker {

private:

    void reset_counts() {
        bar::copy_count = 0;
        bar::move_count = 0;
        foo::copy_count = 0;
        foo::move_count = 0;
    }

public:

    constructor_checker() {
        reset_counts();
    }

    template<typename T>
    void assert_copied(T&&, int count = 1) {
        assert(std::remove_reference_t<T>::copy_count == count);
    }

    template<typename T>
    void assert_moved(T&&, int count = 1) {
        assert(std::remove_reference_t<T>::move_count == count);
    }

    template<typename T>
    void assert_not_moved(T&&) {
        assert(std::remove_reference_t<T>::move_count == 0);
    }

    template<typename T>
    void assert_not_copied(T&&) {
        assert(std::remove_reference_t<T>::copy_count == 0);
    }
};
*/
int main() {

    /*
    auto my_bar = bar{};
    auto my_foo = foo{};

    {
        auto checker = constructor_checker{};
        auto f = fwrap(static_cast<overload(bar::*)(foo&)>(&bar::func));
        assert(f(my_bar, my_foo) == overload::lref);
        checker.assert_not_moved(my_foo);
        checker.assert_not_moved(my_bar);
        checker.assert_not_copied(my_foo);
        checker.assert_not_copied(my_bar);
        static_assert(std::is_same<first_arg<decltype(f)>, universal_reference<bar> >::value, "");
    }
    {
        auto checker = constructor_checker{};
        auto f = fwrap<default_ | lvalue_reference_>(static_cast<overload(bar::*)(foo&)>(&bar::func));
        assert(f(my_bar, my_foo) == overload::lref);
        checker.assert_not_moved(my_foo);
        checker.assert_not_moved(my_bar);
        checker.assert_not_copied(my_foo);
        checker.assert_not_copied(my_bar);
        static_assert(std::is_same<first_arg<decltype(f)>, bar&>::value, "");
    }
    {
        //take the unbound object as a reference
        auto checker = constructor_checker{};
        auto f = fwrap<default_>(static_cast<overload(bar::*)(foo&)>(&bar::func));
        assert(f(my_bar, my_foo) == overload::lref);
        checker.assert_not_moved(my_foo);
        checker.assert_not_moved(my_bar);
        checker.assert_not_copied(my_foo);
        checker.assert_not_copied(my_bar);
        static_assert(std::is_same<first_arg<decltype(f)>, universal_reference<bar> >::value, "");
    }

    {
        //take the unbound object as a copy
        auto checker = constructor_checker{};
        auto f = fwrap<copy_>(static_cast<overload(bar::*)(const foo&)>(&bar::func));
        assert(f(my_bar, my_foo) == overload::const_lref);
        checker.assert_not_moved(my_foo);
        checker.assert_not_moved(my_bar);
        checker.assert_not_copied(my_foo);
        checker.assert_copied(my_bar);
        static_assert(std::is_same<first_arg<decltype(f)>, bar>::value, "");
    }
    {
        //calling rvalue reference overload of foo with an rvalue reference of bar
        auto checker = constructor_checker{};
        auto f = fwrap<rvalue_reference_>(static_cast<overload(bar::*)(foo&&)>(&bar::func));
        assert(f(bar{}, foo{}) == overload::rref);
        checker.assert_not_moved(my_foo);
        checker.assert_not_moved(my_bar);
        checker.assert_not_copied(my_foo);
        checker.assert_not_copied(my_bar);
        static_assert(std::is_same<first_arg<decltype(f)>, bar&&>::value, "");
    }
    */
    return 0;
}