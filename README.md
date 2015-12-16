# CLBL
CLBL is a C++14 header-only library of wrappers and tools for callable types: function pointers, pointer-to-member functions, callable objects (which, of course, includes lambdas). This library is intended to be useful for both template metaprogrammers and general C++ programmers, with the following goals:

1. Facilitate the creation of callable wrappers for all callable types (a la std::bind, std::invoke)
2. Employ a common wrapper creation interface (very similar to std::invoke, but without the required argument binding)
3. Signature deduction for unambiguous cases
4. optional, lazy, manual disambiguation for overloaded/templated operator()
5. Easy conversion to type-erased wrappers such as std::function, while maintaining perfect forwarding and overload selection (see the clbl::forward type)
6. provide metaprogramming facilities for all things callable
7. enable the programmer to choose an object lifetime strategy by accepting object copies, std::reference_wrapper, pointers, and smart pointers

CLBL is a shortening of the word "callable."

CLBL has no dependencies outside the standard library. However, Clang is currently the only compiler known to work with CLBL, due to reliance on generic lambdas and variable templates. CLBL is being developed using the LLVM-vs2014 toolkit in Visual Studio 2015.

Documentation and more features coming soon...
