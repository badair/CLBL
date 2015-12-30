/*

Copyright Barrett Adair 2015
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)

*/

#include <cassert>
#include <string>
#include <CLBL/clbl.h>

struct foo {

    auto operator()() {
        return std::string{ "foo::operator()()" };
    }

    auto operator()(int, char = 'a') {
        return std::string{ "foo::operator()(int, char)" };
    }

    auto operator()() const {
        return std::string{ "foo::operator()() const" };
    }

    auto operator()(int, char) const {
        return std::string{ "foo::operator()(int, char) const" };
    }
};

int main() {

    auto obj = foo{};
    auto callable = clbl::fwrap(obj);

    assert(callable() == "foo::operator()()");

    /*
    There is a little-known (and generally useless) valid C++ function type called an 
    "abominable function type", which looks looks like a normal function
    type (e.g. void(int, char) ) except that it is also cv-qualified and/or 
    ref-qualified, like a member function. An abominable function type cannot be
    instantiated, but CLBL makes use of the information you can store in these
    types. Specifically, CLBL lets you disambiguate operator() by passing an
    abominable function type to clbl::harden.

    Here, we use clbl::harden to choose the const-qualified overload over the non-const one:
    */

    {
        auto hardened_callable = clbl::harden<std::string(void) const>(callable);
        assert(hardened_callable() == "foo::operator()() const");

        //we can always check the emulated function type with clbl::emulates
        static_assert(clbl::emulates<std::string(void)>(hardened_callable), "");

        //compiler error - can't call foo::operator()(int, char) const because
        //we selected the std::string(void) const overload
        //assert(hardened_callable(1, 'a') == "foo::operator()(int, char) const");
    }

    /*
    clbl::harden creates a new callable type for a single overload. You can elevate CV
    with clbl::harden, but you cannot revoke CV. CLBL goes to great lengths to preserve
    CV-correctness while still allowing these upcasts with clbl::harden.

    CV in CLBL is determined by a combination of the following:

    1. CV of the original, underlying object
    2. CV of the CLBL callable wrapper itself (i.e. the CV on the 'this' pointer)
    3. CV qualifiers requested in a template argument to clbl::harden

    ...

	You can also use the clbl::auto_ tag as a placeholder for clbl::harden return types,
	since (generally speaking) return types don't participate in overload resolution:
    */

    using auto_ = clbl::auto_;

    {
        auto hardened_callable = clbl::harden<auto_(int, char) const>(callable);
        assert(hardened_callable(1, 'a') == "foo::operator()(int, char) const");
    }

    /*

    As you might have guessed, clbl::auto_ simply triggers a decltype to determine
    the return instead of what you would normally pass.

    clbl::harden is only useful for ambiguous cases of operator(). This will be
    increasingly useful in the wild as generic lambdas make their way into C++
    codebases. To disambiguate free functions and member functions, you still need
    to static_cast before creating the CLBL wrapper, as has always been the case.
	*/

    return 0;
}