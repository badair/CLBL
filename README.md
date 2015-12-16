# CLBL
CLBL is a C++14 header-only library of wrappers and tools for callable types. Function pointers, pointer-to-member functions, and callable objects, with special considerations for cv-correctness and disambiguation.  This library is intended to be useful for both template metaprogrammers and general C++ programmers, with the following goals:

1. Facilitate the creation of callable wrappers forall  callable types (similar to std::bind/std::invoke)
2. Signature deduction for unambiguous cases
3. optional, lazy, manual deduction for ambiguous cases (overloads/templates)
4. provide metaprogramming facilities for all things callable.
5. Easy conversion to type-erased wrapper implementations like std::function because of signature deduction
6. Perfect forwarding even after conversion to std::function

CLBL is a shortening of the word "callable."

CLBL has no dependencies outside the standard library; however, Clang is currently the only compiler known to work with CLBL, due to the use of generic lambdas and variable templates. CLBL is being developed using LLVM-vs2014 toolkit in Visual Studio 2015.

More features and documentation coming soon...
