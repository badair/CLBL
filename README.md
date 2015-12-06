# CLBL
CLBL is a C++14 header-only library for creating generic, statically dispatched callable objects by wrapping free functions, member functions and function objects, without using type erasure (but still convertible to `std::function`). This library is intended to be useful for both template metaprogrammers and general C++ programmers, with the following goals:

1. Allow creation of callable wrappers with function types deduced for non-overloaded cases
2. Eliminate the semantic barriers that separate free functions, member functions, and function objects
3. Improve performance over type-erased function wrappers like `std::function`.
4. provide metaprogramming facilities for all things callable.

CLBL is a shortening of the word "callable."

CLBL overlaps considerably with std::bind, std::invoke, and boost::function_traits.

External dependencies: Boost.Hana

Tested using LLVM-vs2014 toolkit in Visual Studio 2015. Compiler support is probably the same as Boost.Hana. So, as far as I know, Clang is the only C++14 compiler that currently supports the features used by CLBL (Novemeber 2015).

*The old documentation has been removed, because it was entirely outdated. New docs will be rewritten in the near future, when the current rewrite is finished. In the meantime, please refer to the test cases. *
