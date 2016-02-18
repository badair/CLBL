 /*

Copyright Barrett Adair 2015
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)

*/

#include<functional>
#include<iostream>
#include<cassert>

#include <CLBL/clbl.hpp>

using namespace clbl;

void call_std_function(std::function<void(int)> f, int i) {
    f(i);
}

struct foo {

    int value;

    void add(int i) {
        value = value + i;
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

template<typename FooWrapper>
void test_foo_wrapper_indirect(FooWrapper&& f, foo& my_foo) {

    my_foo.value = 0;
    foo::copy_count = 0;
    foo::move_count = 0;

    std::cout << "Testing indirect wrapper...\n";

    auto stdf = convert_to<std::function>(f);

    f(1);
    assert(my_foo.value == 1);
    assert(foo::copy_count == 0);
    assert(foo::move_count == 0);

    stdf(2);
    assert(my_foo.value == 3);
    assert(foo::move_count == 0);

    call_std_function(stdf, 3);
    assert(my_foo.value == 6);
    assert(foo::move_count == 0);
}

template<typename FooWrapper>
void test_foo_wrapper_copy(FooWrapper&& f, foo& my_foo) {

    my_foo.value = 0;
    foo::copy_count = 0;
    foo::move_count = 0;

    std::cout << "Testing copy wrapper...\n";

    assert(foo::move_count == 0);

    auto stdf = convert_to<std::function>(f);

    assert(foo::copy_count == 1);

    f(1);
    assert(my_foo.value == 0);

    stdf(2);
    assert(my_foo.value == 0);
    assert(foo::copy_count == 1);
    call_std_function(stdf, 3);
    assert(my_foo.value == 0);
    assert(foo::copy_count == 2);
}

int main() {
    {
        auto my_foo = foo{};
        auto f = fwrap(&my_foo, &foo::add);
        test_foo_wrapper_indirect(f, my_foo);
    }

    {
        auto my_foo = foo{};
        auto f = fwrap(std::ref(my_foo), &foo::add);
        test_foo_wrapper_indirect(f, my_foo);
    }

    {
        auto my_foo = foo{};
        auto f = fwrap(my_foo, &foo::add);
        test_foo_wrapper_copy(f, my_foo);
    }
}

