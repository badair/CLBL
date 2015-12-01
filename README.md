Note: the perfect-forwarding-and-value-sematics branch (currently under development) invalidates all of this documentation, so continue reading with caution.

# CLBL
CLBL is a C++14 header-only library for creating generic, statically dispatched callable objects by wrapping free functions, member functions and callable objects -- *without using type erasure* (yet still convertible to `std::function`). This library is intended to be useful for both template metaprogrammers and general C++ programmers, with the following goals:

1. Allow creation of callable wrappers with function types deduced for non-overloaded cases
2. Eliminate the semantic barriers that separate free functions, member functions, and "callable objects"
3. Improve performance over type-erased function wrappers like `std::function' *(todo - need to add argument binding features, benchmarks)*
4. provide metaprogramming facilities for all things callable

CLBL is a shortening of the word "callable."

External dependencies: Boost.Hana

Tested using LLVM-vs2014 toolkit in Visual Studio 2015. Compiler support is probably the same as Boost.Hana. So, as far as I know, Clang is the only C++14 compiler that currently supports the features used by CLBL (Novemeber 2015).

## Overview

You only need to include the `CLBL/func.h` header. `boost/hana.hpp` must be includable.

```cpp
#include "CLBL/func.h"
#include <cassert>
#include <functional>
#include <memory>
```

Let's create some things we can call...

```cpp
int add_one(int i) {
    return i + 1;
}

struct two_adder {

    int add_two(int i) {
        return i + 2;
    }

};

auto add_three = [](int i) {return i + 3;};

struct overloaded_four_adder {

    int operator()(int i) const volatile {
        return i + 4;
    }

    int operator()(int i) const {
        assert(false);
        return -1;
    }
};

int main() {
```

Calling `clbl::func` creates a CLBL callable object, without having to specify any type (for non-overloaded cases). Type deduction was the initial motivator for creating this library, since there is no straightforward way (that I know) to create `std::function`s without explicitly specifying types. CLBL employs the non-straightforward way to achieve this, and can be used to create an std::function using entirely deduced types. However, it is possible to do without `std::function`, and may be desirable to do so, especially if you are given free reign with template usage.

The first argument to `clbl::func` can be a free function pointer. This creates a callable object that forwards to the function:

```cpp
    auto add1 = clbl::func(&add_one);
    assert(add1(1) == 2);
```

The first argument can also be a pointer to an object. You can supply a member function as the second argument:

```cpp
    auto add2_obj = two_adder{};
    auto add2 = clbl::func(&add2_obj, &two_adder::add_two);
    assert(add2(1) == 3);
```

If the first argument points to an object, and the second argument is not supplied, the object's `operator()` will be used. This includes (non-generic) lambdas types:

```cpp
    auto add3 = clbl::func(&add_three);
    assert(add3(1) == 4);
```

The decision to make `clbl::func` accept an object pointer instead of a reference was a conscious one - accepting a pointer makes it more obvious (in my opinion) that the resulting CLBL callable object is only valid for the lifetime of the object.

To make things easier, `clbl::func` also accepts `std::shared_ptr`:

```cpp
    auto add2_shared_ptr = std::make_shared<two_adder>();
    auto add2_shared = clbl::func(add2_shared_ptr, &two_adder::add_two);
    assert(add2_shared(1) == 3);
```

`clbl::func` does not accept lambda rvalues. To use lambdas with CLBL, you must assign the lambda to a variable, then pass it's address to `clbl::func`. This makes the lifetime of the lambda, and the programmer's responsibility thereof, more obvious.

Since C++ does not allow us to take the address of an overloaded function without casting it first, CLBL provides the `clbl::force` template to allow the programmer to manually perform overload resolution on the object's `operator()` by passing an "abominable function type." An "abominable function type" is an obscure (and generally useless) C++ type that is conceptually equivalent to a member function pointer type with the underlying type removed. Alternatively, you can think of an abominable function type as a plain function type that can have "unnecessary" CV-qualifiers and ref-qualifiers attached. We leverage this oddity to provide an interface for forced overload resolution on `operator()`, `clbl::force`. For instance, in the case below, if we had expected a member function pointer type instead of an abominable function type, you would have needed to type this:

```cpp
    //clbl::force<int(overloaded_four_adder::*)(int) const volatile>...
```
    
...which is hideously long-winded. Instead, you can do this:

```cpp
    auto four_adder_obj = overloaded_four_adder{};
    auto add4 = clbl::force<int(int) const volatile>::func(&four_adder_obj);
    assert(add4(1) == 5);
```

`int(int) const volatile` is the "abominable function type" in this case. This type is used to build the desired member function type before `static_cast`ing the object's `operator()` to it, which resolves the overload ambiguity. 

For free function overloads and other member function overloads, you must `static_cast` the pointer yourself. This is a language restriction, since we don't (yet) have built-in reflection in C++.

You can convert a CLBL callable object to an `std::function` with `.as<std::function>()`, like this:

```cpp
    auto std_function_add4 = add4.as<std::function>();
    static_assert(std::is_same<std::function<int(int)>, decltype(std_function_add4)>::value, "");
    assert(std_function_add4(1) == add4(1));
```

Converting to std::function is useful in non-templated code, since the type of `decltype(add4)` is not something you will ever want to take as a pararmeter in a non-templated function.

Presumably, `boost::function` works as well, although I have not yet tested this.

A few template metaprogramming features are provided on a CLBL callable type.

```cpp
    using type1 = decltype(add1);
    using type4 = decltype(add4);
```

`::type` always returns a plain function type:

```cpp
    static_assert(std::is_same<type1::type, int(int)>::value, "");
    static_assert(std::is_same<type4::type, int(int)>::value, "");
```

You can use the `matches` constexpr function to compare the `::type` of CLBL callables. This returns a `boost::hana` boolean integral constant:

```cpp
    static_assert(decltype(add1.matches(add4))::value, "");
    static_assert(decltype(add4.matches(add1))::value, "");
```

The constexpr member `arity` returns the number of args the callable will accept, as a `boost::hana` integral constant:

```cpp
    static_assert(add1.arity == 1, "");
    static_assert(add4.arity == 1, "");
```

The `can_call` constexpr function returns false when the arguments would fail to compile:
   
```cpp
    struct some_invalid_argument_type {};
    static_assert(!add4.can_call(some_invalid_argument_type{}), "");
    static_assert(add4.can_call(1), "");
    
    return 0;
}
```

other TMP features:
- `::args_t` is a `boost::hana::tuple<>` of the argument types

- `::return_t` is the return type

- The `.args_tuple` member is a constexpr `boost::hana::tuple<>` of the argument types, with each argument type converted to a `boost::hana::type<>`.

- The `.return_type` member is constexpr shorthand for `boost::hana::type_c<>` of the `return_t` type


## The Future

Some things I'd like to see as development continues:

- early partial binding of arguments
- composability
- support of other FP fundamentals
- improve consistency of metaprogramming features
- improve compile-time performance
- benchmarks against `boost::function`, `std::function`, etc
- templated functions - is there anything we can even do here? Need to experiment
- macro for `static_cast`ing overloaded member functions? Not sure - I like that macros are currently not used
- either remove Boost.Hana dependency, or fully embrace it - currently, only a few features are used
- cmake support for compiling the tests
- split tests up into multiple builds (like Boost.Hana's tests) - this would also allow tests to check for `static_assert`s that are *supposed* to occur, and allow faster development (so we don't always have to build all the tests)

## Feedback
- What would you like to see added?
- What would you like to see changed?
- Would you like this to be a part of your library?
- If this looks useful to anyone, let me know and I'll add a license
- All suggestions, comments, and criticisms are highly appreciated.
