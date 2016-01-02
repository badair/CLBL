/*!

@copyright Barrett Adair 2015
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)

@mainpage User Manual

@tableofcontents

@section tutorial-terminology Terminology

1. CV - [const/volatile-qualified][C++.CV]
2. ambiguous case - In the context of CLBL, this refers to a member/function template or 
overload whose address cannot be resolved by name alone
3. "Matryoshka madness" - Excessive type recursion resembling the [traditional Russian toy][Doll]

@section tutorial-cv-correctness CV Correctness

`bar` represents some type with a CV-overloaded `operator()`.

@code{cpp}
struct bar {
    auto operator()() { return overload::mutable_; }
    auto operator()() const { return overload::const_; }
    auto operator()() volatile { return overload::volatile_; }
    auto operator()() const volatile { return overload::const_volatile_; }
};

auto mutable_object = bar{};
const auto const_object = bar{};
volatile auto volatile_object = bar{};

@endcode

In C++, initializing an `std::function` with a callable object *reference*
conversion preserves the object's CV-overload resolution. This is, of
course, perfectly normal. Anything else would violate const-correctness.

@code{cpp}
std::function<overload()> f1 = std::ref(mutable_object);
assert_mutable(f1);

std::function<overload()> f2 = std::ref(const_object);
assert_const(f2);

std::function<overload()> f3 = std::ref(volatile_object);
assert_volatile(f3);
@endcode

Initializing an `std::function` with a callable object *copy* does not
pass the CV-ness, and respective overload resolution behavior, of the
original object along with the copy. Once again, this is perfectly
normal behavior in C++. The ability to initialize a non-const object
with a const object copy is pretty fundamental to the way things work
in C++. For example, `void my_func(const int i)` is an ambiguous overload
of `void my_func(int i)`, because a `const int` is copyable to the argument
of both overloads.

However, isn't this feature a bit disappointing when considering the
behavior of CV-overloaded callables?
    
@code{cpp}
std::function<overload()> f = const_object;
assert_const(f); //failed assertion!
@endcode

Making `f` in the above code `const` doesn't change things, either.

One might object, "Why would any sane programmer write logically divergent
CV overloads, where the above scenario actually causes a problem?"

I don't really know the answer to that question. But we do
have to deal with the fact that STL containers' `begin()` and `end()` member
functions are const-overloaded - `const_vector_instance.begin()` returns a
`const_iterator`, while `mutable_vector_instance.begin()` returns a mutable
`iterator`. With that in mind, consider the following parable (or don't, if
you don't like parables):

Suppose, one day at work, you naively assumed that an `std::function`
intialized with a const object copy returned a `const_iterator` when called,
when it actually returned an `iterator`.  Since you treated the
return value like a `const_iterator`, everything worked just fine. The product
shipped, no eyebrows were raised, and donuts were had the following morning.
Months later, a new hire (we'll call him Jimmy) starts poking around the
program's state to fix a latent bug. Jimmy delights in his discovery of a
pristine trove of mutable `iterator`s, minding their own business, blissfully
unaware of their impending demise at Jimmy's imperative tendencies... Jimmy
is why we have `const` in the first place (compiler optimizations
notwithstanding).

Instead of blindly forwarding to `operator()`, CLBL wrappers bake the original
object's CV-ness into their type, so that even copies will resolve to the same
CV overload.

Every initialization in the following snippet involves a copy of a `bar` object:

@code{cpp}
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
@endcode
 
The CV-ness of a CLBL wrapper's underlying object is embedded in the
wrapper's type - this is referred to as "deep CV" in CLBL. Deep CV cannot
be rescinded without resorting to hacks.

The CV-ness of the wrapper's own `this` pointer is referred to as "shallow
CV", which stacks on top of deep CV.
    
Here we add shallow const to a reference: 

@code{cpp}
const auto& clbl_const = clbl_mutable;
assert_const(clbl_const);
@endcode

Then we add shallow volatile:

@code{cpp}
volatile auto& clbl_const_volatile = clbl_const;
assert_const_volatile(clbl_const_volatile);
@endcode

Perhaps disappointingly, shallow CV is removed in
copy initialization of CLBL wrappers:

@code{cpp}
auto volatile_copy = clbl_const_volatile;
assert_mutable(volatile_copy);
@endcode

This could have been avoided in CLBL by implementing custom copy
constructors, but the runtime benefits of trivial copy construction 
outweigh those of making shallow CV copyable, in my opinion.
        
`clbl::harden` is CLBL's disambiguation tool. One of the things it can
do is "bury" shallow CV into the type of a CLBL wrapper, becoming deep CV.

@code{cpp}
auto still_const_volatile = clbl::harden(clbl_const_volatile);
assert_const_volatile(still_const_volatile);
@endcode
         
The implementation of `clbl::harden` does not "Matryoshka madness", which
in turn helps keep your compiler and debugger happy. Other uses of `clbl::harden`
will be discussed later.

In a similar fashion, `clbl::fwrap` can be used to lock in the CV-ness of a CLBL
wrapper. 

@code{cpp}
auto still_const_volatile = clbl::fwrap(clbl_const_volatile);
assert_const_volatile(still_const_volatile);
@endcode

Special handling is performed when this happens to prevent Matryoshka
madness - the argument CLBL wrapper's internal data is repackaged as CLBL wrapper, which is often a different type entirely. (A naive implemenation
would treat CLBL wrappers as just another callable type.)

-----------

In summary:

Shallow mutable, deep const - the const overload is called:
@code{cpp}
auto clbl_const = clbl::fwrap(const_object);
assert_const(clbl_const);
@endcode

Shallow volatile added to deep const copy - the const volatile overload is called:
@code{cpp}
volatile auto clbl_const_volatile = clbl_const;
assert_const_volatile(clbl_const_volatile);
@endcode

Shallow volatile lost in copy, deep const remains - the const overload is called:
@code{cpp}
std::function<overload()> still_const = clbl_const_volatile;
assert_const(still_const);
@endcode


------------------------------------------------------------------------------

TODO - description

@section tutorial-installation Installation

------------------------------------------------------------------------------

- [<b>Headers</b>](files.html)\n
  A list of all the headers provided by the library.


TODO


<!-- Links -->
[C++.CV]: http://en.cppreference.com/w/cpp/language/cv
[Doll]: https://en.wikipedia.org/wiki/Matryoshka_doll
[C++14.auto_rt]: http://en.wikipedia.org/wiki/C%2B%2B14#Function_return_type_deduction
[C++14.gconstexpr]: http://en.wikipedia.org/wiki/C%2B%2B11#constexpr_.E2.80.93_Generalized_constant_expressions
[C++14.glambda]: http://en.wikipedia.org/wiki/C%2B%2B14#Generic_lambdas
[C++14.ice]: http://en.cppreference.com/w/cpp/types/integral_constant
[C++14.udl]: http://en.wikipedia.org/wiki/C%2B%2B11#User-defined_literals
[C++14.vtemplate]: http://en.wikipedia.org/wiki/C%2B%2B14#Variable_templates
[C++14]: http://en.wikipedia.org/wiki/C%2B%2B14
[C++17.clite]: http://www.open-std.org/jtc1/sc22/wg21/docs/papers/2013/n3580.pdf
[CMake]: http://www.cmake.org
[constexpr_throw]: http://stackoverflow.com/a/8626450/627587
[CopyConstructible]: http://en.cppreference.com/w/cpp/concept/CopyConstructible
[Homebrew.devel-only]: https://github.com/Homebrew/homebrew-devel-only
[Homebrew]: http://brew.sh
[lie-to-children]: http://en.wikipedia.org/wiki/Lie-to-children
[N4487]: https://isocpp.org/files/papers/N4487.pdf
[POD]: http://en.cppreference.com/w/cpp/concept/PODType
[SO.sfinae]: http://stackoverflow.com/a/257382/627587
[Sprout]: https://github.com/bolero-MURAKAMI/Sprout

*/