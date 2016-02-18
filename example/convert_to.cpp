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

    foo(int start) : value(start) {}

    void add(int i) {
        value = value + i;
    }

    static int copy_count;
    static int move_count;
    foo(foo&&) {++move_count;}
    foo(const foo&) { ++copy_count; }
    foo(foo&) { ++copy_count; }
};

int foo::copy_count = 0;
int foo::move_count = 0;

int main() {

    auto my_foo = foo{ 0 };
    auto f = fwrap(&my_foo, &foo::add);
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