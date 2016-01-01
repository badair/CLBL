# CLBL

![Build Status](https://travis-ci.org/badair/CLBL.svg?branch=master)

[![Join the chat at https://gitter.im/badair/CLBL](https://badges.gitter.im/badair/CLBL.svg)](https://gitter.im/badair/CLBL?utm_source=badge&utm_medium=badge&utm_campaign=pr-badge&utm_content=badge)

</a> <a target="_blank" href="http://melpon.org/wandbox/permlink/CGFpYjm7071gC7JC">![Try it online][badge.wandbox]</a>

CLBL is a generic C++14 header-only library for callable types that provides a uniform interface for wrapping callable types, manual overload resolution tools for operator(), and perfectly-forwarded conversions to std::function and similar type-erased delegates (including implicit for unambiguous calls, which means you don't have to specify the signature). This library is intended to be useful for both template metaprogrammers and general C++ programmers, with the following goals:

1. Provide callable wrappers for callable types with minimal runtime overhead (if any), in a fashion similar to 'std::bind'. Note: "Minimal runtime overhead" relies on the compiler to perform <a href="https://en.wikipedia.org/wiki/Return_value_optimization">RVO</a>. Because everything is inlined and most things are `constexpr`, RVO is relatively trivial for compilers. (Benchmarks coming soon)

2. Use a uniform interface for wrapping anything callable  - `clbl::fwrap` is a heavily overloaded function that accepts function pointers, function object lvalues/rvalues, std::reference_wrapper, and PMFs (pointer-to-member functions).

3. Deduce signatures for unambiguous callable types, and provide metaprogramming tools for manipulating them

4. Provide intuitive methods for for lazy disambiguation of objects with ambiguous `operator()` (see `clbl::harden`)

5. Allow for easy conversion to type-erased delegates (like `std::function`) with `clbl::convert_to', where
        - conversion of unambiguous calls does not require an explicitly specified signature
        - ambiguous callable types that have been disambiguated with `clbl::harden` maintain CV-overload selection when converted to a
            type-erased delegate (even in copy initialization)
        - perfect forwarding to the original callable is preserved

CLBL is a shortening of the word "callable."

CLBL has no dependencies outside the standard library. However, Clang is currently the only compiler known to work with CLBL, due to reliance on generic lambdas and variable templates. CLBL is being developed using the LLVM-vs2014 toolkit in Visual Studio 2015.

CLBL only includes 4 standard library headers:

1. `<functional>` - only used to make an interface for std::reference_wrapper. The aforementioned features regarding `std::function` are implicit,  because `clbl::convert_to` accepts a template template parameter

2. `<tuple>` - only used to make argument types easily accessible for metaprogrammers

3. `<type_traits>` - used pervasively for type-level compile-time computations

4. `<utility>` - for std::forward

## [Quick example][example.Wandbox] (build and run online)

There are more documented examples in the CLBL/examples folder, and more are being added regularly.

CLBL is not yet stable, and should not yet be used in production code. More features are coming soon, and work on Doxygen-generatetd documentation is currently underway. In the meantime, 

## License
Please see [LICENSE.md](LICENSE.md).


<!-- Links -->
[badge.Wandbox]: https://img.shields.io/badge/try%20it-online-blue.svg
[example.Wandbox]: http://melpon.org/wandbox/permlink/CGFpYjm7071gC7JC
