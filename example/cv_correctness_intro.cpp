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

#include <CLBL/clbl.h>

//bear with me while we set the stage...

struct foo {};

enum class overload { mutable_, const_, volatile_, const_volatile_ };

struct bar {
    overload operator()() { return overload::mutable_; }
    overload operator()() const { return overload::const_; }
    overload operator()() volatile { return overload::volatile_; }
    overload operator()() const volatile { return overload::const_volatile_; }

    // C++ has no implicit/default volatile constructor,
    // but copy semantics are a part of this example,
    // so I must do this...
    bar() = default;
    bar(const bar&) = default;
    bar(bar&&) = default;
    bar(const volatile bar&) {}
};

template<typename CallableType>
void assert_mutable(CallableType&& c) { assert(c() == overload::mutable_); }

template<typename CallableType>
void assert_const(CallableType&& c) { assert(c() == overload::const_); }

template<typename CallableType>
void assert_volatile(CallableType&& c) { assert(c() == overload::volatile_); }

template<typename CallableType>
void assert_const_volatile(CallableType&& c) { assert(c() == overload::const_volatile_); }

int main() {
    auto mbar = bar{};
    assert_mutable(mbar); //passes

    const auto cbar = bar{};
    assert_const(cbar); //passes

    volatile auto vbar = bar{};
    assert_volatile(vbar); //passes

    using std_func = std::function<overload()>;

    //Ready?

    {
        /*
        Passing a callable object to an std::function by REFERENCE
        preserves cv overload resoultion, because it's using a reference
        to the original object.
        */

        std_func mutable_f = std::ref(mbar);
        assert_mutable(mutable_f); //passes

        std_func const_f = std::ref(cbar);
        assert_const(const_f); //passes

        std_func volatile_f = std::ref(vbar);
        assert_volatile(volatile_f); //passes
    }


    /*
    Initializing an std::function with a callable object by
    VALUE does not preserve the cv-ness of the original object.
    Of course, that's normal behavior in C++. Constness
    isn't determined until after object construction.

    However, isn't the following scenario a bit disappointing?
    */

    {
        std_func mutable_f = mbar;
        assert_mutable(mutable_f); //passes

        /*
        const std_func const_f = cbar;
        assert_const(const_f); // **FAILS**
        */
    }

    /*
    Just a little bit? I mean, of course it's unwise to write
    conceptually divergent CV overloads of member functions, especially
    to the extent where the above scenario comes back to haunt you.
    However, consider the following parable (or don't, if you don't like parables):

    Suppose you naively assumed that an std::function intialized by-value from a
    const object returned a const_iterator, when you actually received a non-const
    iterator. (This scenario isn't too far-fetched, considering that's how the STL's
    begin() and end() overloads are designed.) Since you treated the return value
    like a const_iterator, everything worked just fine. The product shipped, eyebrows
    were un-raised, and donuts were had the following morning. Months later,
    a new hire named Jimmy starts poking around the program's state to fix a latent
    bug. Jimmy delights in his discovery of a cluster of pristine non-const iterators,
    minding their own business, blissfully unaware of their impending demise at the
    Jimmy's behest. You see, Jimmy is why we have const in the first place (compiler
    optimizations notwithstanding).

    That's a potential problem that CLBL attempts to address:
    */
    {
        //these are all copies
        auto clbl_mutable = clbl::fwrap(mbar);
        assert_mutable(clbl_mutable); //passes
        std_func mutable_f = clbl_mutable;
        assert_mutable(mutable_f); //passes

        auto clbl_const = clbl::fwrap(cbar);
        assert_const(clbl_const); //passes
        std_func const_f = clbl_const;
        assert_const(const_f); //passes

        auto clbl_volatile = clbl::fwrap(vbar);
        assert_volatile(clbl_volatile); //passes
        std_func volatile_f = clbl_volatile;
        assert_volatile(volatile_f); //passes
    }

    /*
    The CV-ness of a CLBL wrapper's underlying object is embedded
    in the wrapper's type - this is referred to as "deep CV" in
    CLBL. The CV-ness of the wrapper's own 'this' pointer is
    referred to as "shallow CV", which stacks on top of deep CV.
    */

    {
        auto clbl_mutable = clbl::fwrap(mbar);
        assert_mutable(clbl_mutable);

        //adding shallow const with a reference
        const auto& clbl_const = clbl_mutable;
        assert_const(clbl_const);

        //adding shallow volatile with a reference
        volatile auto& clbl_const_volatile = clbl_const;
        assert_const_volatile(clbl_const_volatile);

        //shallow CV removed in copy initialization
        auto elevated_copy = clbl_const_volatile;
        assert_mutable(elevated_copy);

        /*
        shallow CV becomes deep CV (special case is handled
        for "nested" CLBL wrappers to prevent the wrapper types
        from becoming compile-time Matryoshka dolls)
        */
        auto still_elevated = clbl::fwrap(clbl_const_volatile);
        assert_const_volatile(still_elevated);
    }

    /*
    In case it isn't already clear, this last example shows
    that CLBL stacks all CV sources to resolve the overload:
    */
    {
        //no shallow CV, only deep const
        auto clbl_const = clbl::fwrap(cbar);
        assert_const(clbl_const);

        //shallow volatile, deep const, copy initialized
        volatile auto clbl_const_volatile = clbl_const;
        assert_const_volatile(clbl_const_volatile);

        //shallow volatile lost in copy, deep const remains
        std_func still_const = clbl_const_volatile;
        assert_const(still_const);

        /*
        clbl::harden locks in the constness for ambiguous callables
        when no type arguments are specified
        */
        auto still_const_volatile = clbl::harden(clbl_const_volatile);
        assert_const_volatile(still_const_volatile);
    }
}