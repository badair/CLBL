# CLBL
CLBL is a C++14 header-only library of wrappers and tools for callable types: function pointers, pointer-to-member functions, callable objects (which, of course, includes lambdas). This library is intended to be useful for both template metaprogrammers and general C++ programmers, with the following goals:

1. Facilitate the creation of callable wrappers for all callable types (a la std::bind, std::invoke) with zero runtime overhead (assuming RVO is performed by the compiler)
2. Employ a common wrapper creation interface (very similar to std::invoke, but without the required argument binding)
3. Signature deduction for unambiguous cases
4. optional, lazy disambiguation for overloaded/templated operator()
5. Easy conversion to type-erased wrappers such as std::function, while maintaining perfect forwarding and overload selection (see the clbl::forward type)
6. provide metaprogramming facilities for all things callable
7. enable the programmer to choose an object lifetime strategy by accepting arguments by-value, by std::reference_wrapper, by pointer, and by smart pointer

CLBL is a shortening of the word "callable."

CLBL has no dependencies outside the standard library. However, Clang is currently the only compiler known to work with CLBL, due to reliance on generic lambdas and variable templates. CLBL is being developed using the LLVM-vs2014 toolkit in Visual Studio 2015.

More documentation and features coming soon... For now, here's a partial rundown - I'll improve formatting, etc. as time permits:

```cpp
#include <cassert>
#include <functional>
#include <memory>

#include "CLBL/clbl.h"

/*
In this example, and throughout CLBL code, "CV" refers to "const" and "volatile"
qualifiers. 

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

void calculate_sum(int left, int right, int& result) {
    result = left + right;
}

int main() {

    auto obj = cv_reporter{};

    /*
    Here we create a CLBL callable object by wrapping a cv_reporter lvalue (which is copied).
    clbl::fwrap also accepts pointers, smart pointers, rvalues (to copy), and std::reference_wrapper.
    */
    auto callable = clbl::fwrap(obj);

    /*
    clbl::fwrap also accepts free function pointers and member function pointers. More on those later.

    cv_reporter has an overloaded operator(). When we can still call it, overload resolution behaves 
    normally.
    */

    assert(callable() == "no cv");
    assert(callable("std::string overload") == "no cv - std::string overload");

    {
        const auto const_callable = callable;
        assert(const_callable() == "const");

        /*
        you can nest CLBL wrappers ad-nauseum - but your compiler will not be pleased.
        */
        const auto nested_const_callable = clbl::fwrap(callable);
        assert(const_callable() == "const");
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

    /*
    There is a little-known (and generally useless) valid C++ type called an "abominable 
    function type", which is a mutt type that looks like a normal function type (e.g. 
    void(int, char) ) except that it is also cv-qualified and/or ref-qualified, like a 
    member function.
    
    clbl::harden provides an interface to help you safely disambiguate overloads by leveraging
    the information you can store in these types:
    */

    {
        auto elevated_callable = clbl::harden<const char*(void) const>(callable);
        assert(elevated_callable() == "const");

        using emulated_function_type = decltype(elevated_callable)::type;
        static_assert(std::is_same<const char*(void), emulated_function_type>::value, "");
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
    annoying, and sometimes impossible - Do you know the return type of std::bind? clbl:::fwrap? 
    Functions like these might as well return Egyptian hieroglyphs - thankfully, C++ has the 
    auto keyword. In the same vein, CLBL provides the clbl::auto_ tag type, which can be used
    as a placeholder for clbl::harden return types:
    */

    using auto_ = clbl::auto_;

    {
        auto elevated_callable = clbl::harden<auto_(void) const>(callable);
        assert(elevated_callable() == "const");

        using return_type = decltype(elevated_callable)::return_t;
        static_assert(std::is_same<const char*, return_type>::value, "");
    }

    /*
    As you can see, CLBL wrappers define a return_t alias for the return type, which, in this case,
    is deduced to be const char*.

    clbl::harden is only useful for ambiguous cases of operator(). To disambiguate free functions
    and member functions, you still need to static_cast before creating the CLBL wrapper, as has
    always been the case. This will be increasingly useful in the wild as generic lambdas make
    their way into C++ codebases. 

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

    auto sum_calculator = clbl::fwrap(&calculate_sum);
    sum_calculator(2, 5, sum_result);
    assert(sum_result == 7);

    auto std_func_calculator = clbl::convert_to<std::function>(sum_calculator);
    std_func_calculator(3, 6, sum_result);
    assert(sum_result == 9);

    using expected_type = std::function<decltype(sum_calculator)::forwarding_glue>;
    static_assert(std::is_same<expected_type, decltype(std_func_calculator)>::value, "");

    /*
    What the heck is decltype(sum_calculator)::forwarding_glue, you ask? In order to
    perfectly forward value types, CLBL wraps each argument in a clbl::forward object:
    */
    using full_std_func_signature = std::function<void(clbl::forward<int>, clbl::forward<int>, clbl::forward<int&>)>;
    static_assert(std::is_same<full_std_func_signature, expected_type>::value, "");
    
    /*
    A clbl::forward object behaves exactly like the type in its template argument, except that
    the underlying value is not copied in transport:
        std::function -> CLBL wrapper -> orginal

    The value arguments are only copied when passing from the CLBL wrapper to the original.
    */

    return 0;
}
```
