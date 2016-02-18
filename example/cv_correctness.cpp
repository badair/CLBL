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

enum class overload { mutable_, const_, volatile_, const_volatile_ };

//bar is a type with CV-overloads
struct bar {
    /*
    C++ has no implicit/default volatile copy constructor, but copy
    semantics are a part of this example, so I must define these constructors
    */
    bar() = default;
    bar(const bar&) = default;
    bar(bar&&) = default;
    bar(const volatile bar&) {}

    auto operator()() { return overload::mutable_; }
    auto operator()() const { return overload::const_; }
    auto operator()() volatile { return overload::volatile_; }
    auto operator()() const volatile { return overload::const_volatile_; }
};

auto assert_mutable = [](auto&& c) { assert(c() == overload::mutable_);};
auto assert_const = [](auto&& c) { assert(c() == overload::const_);};
auto assert_volatile = [](auto&& c) { assert(c() == overload::volatile_);};
auto assert_const_volatile = [](auto&& c) { assert(c() == overload::const_volatile_);};

//In this example, there are no uncommented asserts that fail.

int main() {

    /*
    These 3 callable objects are used throughout this example. Note that
    the term "mutable" as used in this example refers to the general concept
    of mutability - not the related C++ keyword.
    */
    auto mutable_object = bar{};
    const auto const_object = bar{};
    volatile auto volatile_object = bar{};

    assert_mutable(mutable_object);
    assert_const(const_object);
    assert_volatile(volatile_object);

    //Ready?

    /*
    In C++, initializing an std::function with a callable object *reference*
    conversion preserves the object's CV-overload resolution. This is, of
    course, perfectly normal. Anything else would violate const-correctness.
    */
    {
        std::function<overload()> f1 = std::ref(mutable_object);
        assert_mutable(f1);

        std::function<overload()> f2 = std::ref(const_object);
        assert_const(f2);

        std::function<overload()> f3 = std::ref(volatile_object);
        assert_volatile(f3);

        /* pointers not allowed
        std::function<overload()> f4 = &mutable_object;
        assert_mutable(f4);
        */
    }

    /*
    Initializing an std::function with a callable object *copy* does not
    pass the CV-ness, and respective overload resolution behavior, of the
    original object along with the copy. Once again, this is perfectly
    normal behavior in C++. The ability to initialize a non-const object
    with a const object copy is pretty fundamental to the way things work
    in C++. For example, 'void my_func(const int i)' is an ambiguous overload
    of 'void my_func(int i)', because a const int is copyable to argument
    of both overloads.

    However, isn't this feature a little bit disappointing when dealing with
    CV-overloaded callable objects?
    */

    {
        std::function<overload()> f1 = mutable_object;
        assert_mutable(f1);

        /*
        std::function<overload()> f2 = const_object;
        assert_const(f2); // FAILS

        const std::function<overload()> f3 = const_object;
        assert_const(f3); // FAILS
        */
    }

    /*
    One might object, "Why would any sane programmer write logically divergent
    CV overloads, where the above scenario actually causes a problem?"

    I don't really know the answer to that question. But we do
    have to deal with the fact that STL containers' begin() and end() member
    functions are const-overloaded - const_vector_instance.begin() returns a
    const_iterator, while mutable_vector_instance.begin() returns a mutable
    iterator. With that in mind, consider the following parable (or don't, if
    you don't like parables):

    Suppose, one day at work, you naively assumed that an std::function
    intialized with a const object copy returned a const_iterator when called,
    when you actually received a non-const iterator.  Since you treated the
    return value like a const_iterator, everything worked just fine. The product
    shipped, no eyebrows were raised, and donuts were had the following morning.
    Months later, a new hire (we'll call him Jimmy) starts poking around the
    program's state to fix a latent bug. Jimmy delights in his discovery of a
    pristine trove of mutable iterators, minding their own business, blissfully
    unaware of their impending demise at Jimmy's imperative tendencies... Jimmy
    is why we have const in the first place (compiler optimizations
    notwithstanding).

    Instead of blindly forwarding to operator(), CLBL wrappers bake the original
    object's CV-ness into their type, so that even copies will resolve to the same
    CV overload:
    */
    {
        //every initialization in this section block involves a copy of a bar object

        auto clbl_mutable = clbl::fwrap(mutable_object);
        assert_mutable(clbl_mutable);
        std::function<overload()> f1 = clbl_mutable;
        assert_mutable(f1);

        auto clbl_const = clbl::fwrap(const_object);
        assert_const(clbl_const);
        std::function<overload()> f2 = clbl_const;
        assert_const(f2);

        auto clbl_volatile = clbl::fwrap(volatile_object);
        assert_volatile(clbl_volatile);
        std::function<overload()> f3 = clbl_volatile;
        assert_volatile(f3);
    }

    /*
    The CV-ness of a CLBL wrapper's underlying object is embedded in the
    wrapper's type - this is referred to as "deep CV" in CLBL. Deep CV cannot
    be rescinded without resorting to hacks.

    The CV-ness of the wrapper's own 'this' pointer is referred to as "shallow
    CV", which stacks on top of deep CV.
    */

    {
        auto clbl_mutable = clbl::fwrap(mutable_object);
        assert_mutable(clbl_mutable);

        //adding shallow const with a reference
        const auto& clbl_const = clbl_mutable;
        assert_const(clbl_const);

        //adding shallow volatile with a reference
        volatile auto& clbl_const_volatile = clbl_const;
        assert_const_volatile(clbl_const_volatile);

        /*
        clbl::harden has several uses for disambiguation, one of which locks
        in the CV-ness for ambiguous callables when no type arguments are
        specified. In other words, shallow CV becomes buried into the object's
        type. This is done without without Matryoshka doll madness.

        Note: other uses of clbl::harden are discussed elsewhere.
        */
        {
            auto still_const_volatile = clbl::harden(clbl_const_volatile);
            assert_const_volatile(still_const_volatile);
        }
        /*
        clbl::fwrap can also be used to lock in the CV-ness of a CLBL wrapper.
        Special handling is performed when this happens - the argument wrapper's
        internal data is repackaged into a new CLBL wrapper. A naive implemenation
        would treat CLBL wrappers as just another callable type. By preventing
        Matryoshka doll madness, CLBL makes compilers and debuggers happy.
        */
        {
            auto still_const_volatile = clbl::fwrap(clbl_const_volatile);
            assert_const_volatile(still_const_volatile);
        }
    }

    /*
    In case it isn't already clear, the last section of this example shows
    that CLBL stacks all CV sources to resolve the overload.
    */

    //shallow mutable, deep const - the const overload is called.
    auto clbl_const = clbl::fwrap(const_object);
    assert_const(clbl_const);

    //shallow volatile added to deep const copy - the const volatile overload is called.
    volatile auto clbl_const_volatile = clbl_const;
    assert_const_volatile(clbl_const_volatile);
}