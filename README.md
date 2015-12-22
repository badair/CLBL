# CLBL

[![Join the chat at https://gitter.im/badair/CLBL](https://badges.gitter.im/badair/CLBL.svg)](https://gitter.im/badair/CLBL?utm_source=badge&utm_medium=badge&utm_campaign=pr-badge&utm_content=badge)

CLBL is a C++14 header-only library of wrappers and tools for callable types: function pointers, pointer-to-member functions, callable objects (which, of course, includes lambdas), and many forms of indirection thereof. This library is intended to be useful for both template metaprogrammers and general C++ programmers, with the following goals:

1. Facilitate the creation of callable wrappers at compile-time for any callable type, with no runtime overhead. "No runtime overhead" does rely on the compiler to perform RVO. Because everything is inlined and most things are `constexpr`, RVO is relatively trivial for compilers.

2. Employ a common wrapper creation interface - `clbl::fwrap` is a heavily overloaded function thataccepts function pointers, function object lvalues/rvalues, std::reference_wrapper, and PMFs (pointer-to-member functions). This is similar in design to `std::invoke`

3. Deduce signatures for unambiguous callable types, and provide metaprogramming tools to manipulate them

4. Provide an intuitive interface for lazy disambiguation of objects with overloaded/templated `operator()` - this is done by `clbl::harden`

5. Allow for easy conversion to type-erased wrappers like `std::function` via `clbl::convert_to`, while maintaining perfect forwarding and CV overload selection determined with `clbl::harden`, without requiring the user to explicitly specify the function type (for unambiguous cases)

CLBL is a shortening of the word "callable."

CLBL has no dependencies outside the standard library. However, Clang is currently the only compiler known to work with CLBL, due to reliance on generic lambdas and variable templates. CLBL is being developed using the LLVM-vs2014 toolkit in Visual Studio 2015.

CLBL only uses 4 standard library headers:

1. `<functional>` - only used to make an interface for std::reference_wrapper. The aforementioned features regarding `std::function` are implicit,  because `clbl::convert_to` accepts it as a template template parameter

2. `<tuple>` - only used to make argument types easily accessible for metaprogrammers

3. `<type_traits>` - used pervasively for type-level compile-time computations

4. `<utility>` - for std::forward

CLBL is not yet stable, and should not yet be used in production code.

More features and documentation coming soon... For now, here's a quick, sorely incomplete rundown:

```cpp

#include <cassert>
#include <functional>
#include <memory>

#include <CLBL/clbl.h>

/*
In this example, and throughout CLBL code, "CV" refers to "const" and "volatile".

Ref-qualifiers are not officically supported by CLBL.
*/

struct cv_reporter {

    auto operator()() {
        return "no cv";
    }

    auto operator()(std::string additional_message) {
        return "no cv - " + additional_message;
    }

    auto operator()() const {
        return "const";
    }

    auto operator()() volatile {
        return "volatile";
    }

    auto operator()() const volatile {
        return "const volatile";
    }
};

int main() {

    auto obj = cv_reporter{};

    /*
    Here we create a CLBL callable object by wrapping a cv_reporter lvalue (which is copied).
    clbl::fwrap also accepts pointers, smart pointers, rvalues, and std::reference_wrapper.
    */
    auto callable = clbl::fwrap(obj);

    /*
    clbl::fwrap also accepts free function pointers and member function pointers. Documentation has not yet been written
    for these features, but there are many test cases for them.

    cv_reporter has an overloaded operator(). CV overloads stick with the CV of the 
    CLBL wrapper object.
    */

    assert(callable() == "no cv");
    assert(callable("std::string overload") == "no cv - std::string overload");

    {
        const auto const_callable = callable;
        assert(const_callable() == "const");

        /*
        The potential symbol explosion is intercepted when trying to nest CLBL wrappers. The 
        guts of the callable object here are repurposed, while still keeping any newly 
        introduced CV-qualifiers.
        */
        const auto nested_const_callable = clbl::fwrap(callable);
        assert(nested_const_callable() == "const");
    }
    /*
    The std::string overload isn't const-qualified. Trying to call it with our const-ified CLBL
    wrappers will fail to compile.

    So far, we haven't seen anything you can't do with a good old-fashioned lambda. This is
    where things start getting interesting (I hope).

    Every CLBL callable type (i.e. every type that clbl::fwrap can return) defines a "type"
    alias, that tells you the emulated function type of the callable object. Since cv_reporter's
    operator() is overloaded, there is no emulated function type - the wrapper just forwards
    whatever it gets and lets the compiler handle overload resolution. We can check for this
    programmatically:
    */
    static_assert(std::is_same<decltype(callable)::type, clbl::ambiguous_return(clbl::ambiguous_args)>::value, "");

    //the same idea, in fewer keystrokes:
    static_assert(decltype(callable)::is_ambiguous, "");

    //fewer still:
    static_assert(clbl::is_ambiguous(callable), "");

    /*
    There is a little-known (and generally useless) valid C++ type called an "abominable
    function type", which is a mutt type that looks like a normal function type (e.g.
    void(int, char) ) except that it is also cv-qualified and/or ref-qualified, like a
    member function.

    clbl::harden provides an interface to help you safely disambiguate overloads by leveraging
    the information you can store in these strange types:
    */

    {
        auto elevated_callable = clbl::harden<const char*(void) const>(callable);
        assert(elevated_callable() == "const");

        //we can always check the emulated function type with clbl::emulates
        static_assert(clbl::emulates<const char*(void)>(elevated_callable), "");
    }

    /*
    (Note: To learn more about abominable function types, check out the C++17 proposal P0172R0.
    If you think clbl::harden is a good use case for these types, let the commitee know,
    because otherwise the type might end up on C++17's chopping block.)

    We just used clbl::harden to choose the const-qualified overload over the non-const one.
    clbl::harden creates a new callable type for a single overload. You can elevate CV with
    clbl::harden, but you cannot revoke CV. CLBL goes to great lengths to preserve CV-correctness
    while still allowing these upcasts with clbl::harden.

    CV is determined by one of three things in CLBL:

    1. CV of the original, underlying object
    2. CV of the CLBL callable wrapper itself
    3. CV specified by clbl::harden

    Attentive readers probably noticed that one of cv_reporter's operator() overloads returns
    an std::string, while the others return const char*. Trying to remember return types is
    annoying, and sometimes impossible - this is why we have C++11 auto. In the same vein, 
    CLBL provides the clbl::auto_ tag type, which can be used as a placeholder for 
    clbl::harden return types:
    */

    using auto_ = clbl::auto_;

    {
        auto elevated_callable = clbl::harden<auto_(void) const>(callable);
        assert(elevated_callable() == "const");

        //getting the return type with clbl::result_of
        using return_type = clbl::result_of<decltype(elevated_callable)>;
        static_assert(std::is_same<const char*, return_type>::value, "");

        //a cleaner way to perform the above check:
        static_assert(clbl::returns<const char*>(elevated_callable), "");
    }

    /*
    clbl::harden is only useful for ambiguous cases of operator(). This will be increasingly useful
    in the wild as generic lambdas make their way into C++ codebases. To disambiguate free functions
    and member functions, you still need to static_cast before creating the CLBL wrapper, as has
    always been the case.

    ...

    Note: Neither the emulated function type nor the type requested with clbl::harden are the
    signature of the CLBL wrapper's actual operator() - in general, you don't have to worry about
    this. When you do, the clbl::forward tool smooths the seams, which will be discussed later in
    this section.

    An especially practical feature of using (unambiguous) CLBL wrappers is their ability to create
    std::function objects without requiring the programmer to specify the signature. This is possible
    because the CLBL wrapper already knows everything there is to know about a function's type.
    clbl::convert_to is used to create std::function objects.
    */

    auto sum_result = 0;

    auto sum_calculator = clbl::fwrap([](int left, int right, int& result) {
        result = left + right;
    });

    sum_calculator(2, 5, sum_result);
    assert(sum_result == 7);

    auto std_func_calculator = clbl::convert_to<std::function>(sum_calculator);
    std_func_calculator(3, 6, sum_result);
    assert(sum_result == 9);

    using forwarding_glue = clbl::forwarding_glue<decltype(sum_calculator)>;
    using expected_type = std::function<forwarding_glue>;
    static_assert(std::is_same<expected_type, decltype(std_func_calculator)>::value, "");

    /*
    "forwarding_glue" is used to trick std::function into perfect forwarding for us.
    forwarding_glue is simply the emulated function type, except each argument 
    type is wrapped in clbl::forward.
    */

    using expanded_forwarding_glue = void(clbl::forward<int>, clbl::forward<int>, clbl::forward<int&>);
    static_assert(std::is_same<forwarding_glue, expanded_forwarding_glue>::value, "");

    /*
    A clbl::forward object behaves exactly like the type in its template argument, except 
    that the underlying value is not copied when marshaling arguments all the way back to 
    the original callable argument to clbl::fwrap. The value arguments are only copied when
    passing from the CLBL wrapper to the original.
    */

    return 0;
}

```
