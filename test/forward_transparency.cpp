/*

Copyright Barrett Adair 2015
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)

*/

#include <functional>
#include <iostream>
#include <cassert>
#include <memory>
#include <utility>

#include <CLBL/clbl.hpp>

using namespace clbl;

struct foo {};

enum class overload {
    lref,
    const_lref,
    rref,
};

struct ref_taker {
    overload operator()(foo&) {
        return overload::lref;
    }
    overload operator()(const foo&) {
        return overload::const_lref;
    }
    overload operator()(foo&&) {
        return overload::rref;
    }
};

template<typename Callable>
void assert_lref(Callable&& f) {
    auto my_foo = foo{};
    foo& lref = my_foo;
    assert(f(lref) == overload::lref);
}

template<typename Callable>
void assert_const_lref(Callable&& f) {
    auto my_foo = foo{};
    const foo& const_lref = my_foo;
    assert(f(const_lref) == overload::const_lref);
}

template<typename Callable>
void assert_rref(Callable&& f) {
    assert(f(foo{}) == overload::rref);
}

template<typename Callable>
void test_reference_overload_resolution(Callable&& f) {
    assert_lref(std::forward<Callable>(f));
    assert_const_lref(std::forward<Callable>(f));
    assert_rref(std::forward<Callable>(f));
}

void call_std_function_lref(std::function<overload(foo&)> f) {
    //uncommenting assert_const_lref or assert_rref 
    //should cause compiler error

    assert_lref(f);
    //assert_const_lref(f);
    //assert_rref(f);
}

void std_function_lref_control_case() {

    std::function<overload(foo&)> f =
        [](foo&) {return overload::lref;};

    //uncommenting assert_const_lref or assert_rref 
    //should cause compiler error
    
    assert_lref(f);
    //assert_const_lref(f);
    //assert_rref(f);
}


void call_std_function_const_lref(std::function<overload(const foo&)> f) {
    //assert_lref and assert_rref should compile, but should 
    //fail runtime asserts because overloads are erased

    //assert_lref(f);
    assert_const_lref(f);
    //assert_rref(f);
}

void std_function_const_lref_control_case() {

    std::function<overload(const foo&)> f =
        [](const foo&) {return overload::const_lref;};

    //assert_lref and assert_rref should compile, but should 
    //fail runtime asserts because overloads are erased

    //assert_lref(f);
    assert_const_lref(f);
    //assert_rref(f);
}

void call_std_function_rref(std::function<overload(foo&&)> f) {
    //uncommenting assert_lref or assert_const_lref 
    //should cause compiler error

    //assert_lref(f);
    //assert_const_lref(f);
    assert_rref(f);
}

void std_function_rref_control_case() {

    std::function<overload(foo&&)> f =
        [](foo&&) {return overload::rref;};

    //uncommenting assert_lref or assert_const_lref 
    //should cause compiler error

    //assert_lref(f);
    //assert_const_lref(f);
    assert_rref(f);
}

template<typename Callable>
void test_clbl_forward_transparency(Callable&& c) {
    auto my_foo = foo{};
    {
        foo& lref = my_foo;
        auto hardened = harden<overload(foo&)>(c);
        assert(hardened(lref) == overload::lref);
        auto f = convert_to<std::function>(hardened);
        assert(f(lref) == overload::lref);
        call_std_function_lref(f);
    }
    {
        const foo& const_lref = my_foo;
        auto hardened = harden<overload(const foo&)>(c);
        assert(hardened(const_lref) == overload::const_lref);
        auto f = convert_to<std::function>(hardened);
        assert(f(const_lref) == overload::const_lref);
        call_std_function_const_lref(f);
    }
    {
        auto hardened = harden<overload(foo&&)>(c);
        assert(hardened(foo{}) == overload::rref);
        auto f = convert_to<std::function>(hardened);
        assert(f(foo{}) == overload::rref);
        call_std_function_rref(f);
    }
}

int main() {

    {
        auto r = ref_taker{};
        auto f = fwrap(&r);
        test_reference_overload_resolution(f);
    }
    {
        auto r = ref_taker{};
        auto f = fwrap(r);
        test_reference_overload_resolution(f);
    }
    {
        auto r = ref_taker{};
        auto f = fwrap(std::ref(r));
        test_reference_overload_resolution(f);
    }
    {
        auto r = ref_taker{};
        auto f = fwrap(std::move(r));
        test_reference_overload_resolution(f);
    }
    {
        auto f = fwrap(std::make_shared<ref_taker>());
        test_reference_overload_resolution(f);
    }
    {
        auto f = fwrap(std::make_unique<ref_taker>());
        test_reference_overload_resolution(f);
    }

    //STD::FUNCTION ARGUMENT FORWARDING TESTS

    std_function_lref_control_case();
    std_function_const_lref_control_case();
    std_function_rref_control_case();

    {
        auto r = ref_taker{};
        auto f = fwrap(&r);
        test_clbl_forward_transparency(f);
    }
    {
        auto r = ref_taker{};
        auto f = fwrap(r);
        test_clbl_forward_transparency(f);
    }
    {
        auto r = ref_taker{};
        auto f = fwrap(std::ref(r));
        test_clbl_forward_transparency(f);
    }
    {
        auto r = ref_taker{};
        auto f = fwrap(std::move(r));
        test_clbl_forward_transparency(f);
    }
    {
        //todo - make movable version of harden and convert_to - unique_ptr will not work there
        auto f = fwrap(std::make_shared<ref_taker>());
        test_clbl_forward_transparency(f);
    }
}

