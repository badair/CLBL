/*

Copyright Barrett Adair 2015
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)

*/

/*
The tests in this file are mostly inferior to
the ones in reference_transparency_tests.cpp
*/

#include "test.hpp"
#include "int_char_definitions.hpp"

#include <iostream>
#include <functional>
#include <CLBL/clbl.hpp>

using namespace clbl::test;
using namespace clbl;


struct copy_counter {

    //increment value on every copy
    static int value;
    static void reset() { value = 0; }

    copy_counter() = default;
    copy_counter(const copy_counter&) { ++value; }
    copy_counter(copy_counter&&) = default;
};

/*
calling the take_copy function will increment
the copy_counter::value. This will allow us to
confirm that arguments passing through CLBL are
not copied superfluously
*/
void take_copy(copy_counter) {}

/*
we will use this to test something that forwards what
we're forwarding
*/
auto forwarder = [](auto&& t) {return take_copy(t);};

struct copy_taker {
    void take_copy(copy_counter) const {}
};

struct copy_returner {
    copy_counter return_copy() {
        copy_counter c{};
        return c;
    }
};

int copy_counter::value = 0;

int main() {

    {
        copy_counter::reset();

        auto f = fwrap(&take_copy);
        copy_counter obj{};

        f(obj);

        assert(copy_counter::value == 1);
    }

    {
        copy_counter::reset();

        auto f = fwrap(&forwarder);
        copy_counter obj{};
        f(obj);

        assert(copy_counter::value == 1);
    }
    {
        copy_counter::reset();

        auto f = fwrap(forwarder);
        copy_counter obj{};
        f(obj);

        assert(copy_counter::value == 1);
    }
    {
        copy_counter::reset();

        auto f = fwrap(std::ref(forwarder));
        copy_counter obj{};
        f(obj);

        assert(copy_counter::value == 1);
    }
    {
        copy_counter::reset();
        copy_counter obj{};

        auto f = fwrap(&take_copy);
        auto hardened = harden<void(copy_counter)>(f);

        hardened(obj);
        assert(copy_counter::value == 1);

        auto std_func = convert_to<std::function>(hardened);

        std_func(obj);
        assert(copy_counter::value == 2);
    }
    {
        copy_counter::reset();
        copy_counter obj{};

        auto f = fwrap(forwarder);

        //lamba operator()s are const qualified unless the lambda is specified as mutable.
        //Forwarding lambdas need a reference type
        auto hardened = harden<void(copy_counter&) const>(f);

        hardened(obj);
        assert(copy_counter::value == 1);

        auto std_func = convert_to<std::function>(hardened);

        std_func(obj);
        assert(copy_counter::value == 2);
    }
    {
        copy_counter::reset();
        copy_counter obj{};

        auto std_func = convert_to<std::function>(harden<void(const copy_counter&) const>(fwrap(forwarder)));

        std_func(obj);
        assert(copy_counter::value == 1);
    }
    {
        copy_counter::reset();
        copy_counter obj{};

        const auto f = fwrap(&copy_taker::take_copy, copy_taker{});
        const auto hardened = harden<void(copy_counter) const>(f);

        hardened(obj);
        assert(copy_counter::value == 1);

        auto std_func = convert_to<std::function>(hardened);

        std_func(obj);
        assert(copy_counter::value == 2);
    }
    {
        //testing presence of RVO for non-trivially copyable types
        static_assert(!std::is_trivially_copyable<copy_counter>::value, "");

        copy_counter::reset();
        auto f = fwrap(&copy_returner::return_copy, copy_returner{});
        auto hardened = harden<copy_counter()>(f);
        auto std_func = convert_to<std::function>(f);
        auto std_func2 = convert_to<std::function>(hardened);

        f();
        assert(copy_counter::value == 0);

        hardened();
        assert(copy_counter::value == 0);

        std_func();
        assert(copy_counter::value == 0);

        std_func2();
        assert(copy_counter::value == 0);
    }
    {
        //testing presence of RVO for non-trivially copyable types
        static_assert(!std::is_trivially_copyable<copy_counter>::value, "");

        copy_counter::reset();
        auto obj = copy_returner{};
        auto f = fwrap(&copy_returner::return_copy, &obj);
        auto hardened = harden<copy_counter()>(f);
        auto std_func = convert_to<std::function>(f);
        auto std_func2 = convert_to<std::function>(hardened);

        f();
        assert(copy_counter::value == 0);

        hardened();
        assert(copy_counter::value == 0);

        std_func();
        assert(copy_counter::value == 0);

        std_func2();
        assert(copy_counter::value == 0);
    }

    return 0;
}