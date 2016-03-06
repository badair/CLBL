# CLBL

![Build Status](https://travis-ci.org/badair/CLBL.svg?branch=master)

[![Join the chat at https://gitter.im/badair/CLBL](https://badges.gitter.im/badair/CLBL.svg)](https://gitter.im/badair/CLBL?utm_source=badge&utm_medium=badge&utm_campaign=pr-badge&utm_content=badge)

<!--</a> <a target="_blank" href="http://melpon.org/wandbox/permlink/TlioDiz6yYNxZFnv">![Try it online][badge.wandbox]</a>-->

Note: I will be renaming this project in the near future. Additionally, the current interface is rough around the edges and not fully adorned.



- function types
- function pointers
- function references
- "abominable" function types
- pointers to member functions
- pointers to member data
- objects with operator()
- objects with function pointer conversions
- const
- volatile
- &
- &&
- varargs
- noexcept
- calling conventions

The taxonomy of callable types in C++ can be difficult to manage, especially for generic programming purposes. For the uninitiated, have a look at the [sample implementation](http://en.cppreference.com/w/cpp/types/is_function#Possible_implementation) of `std::is_function`. This is how function types must be inspected in C++ - with walls and walls of template code. Keep in mind, that's only for normal functions. Dealing with pointer-to-member-functions are ["an absolute nightmare... the worst type\[s\] by far in the core language"](https://youtu.be/zt7ThwVfap0?t=699), according to Stephan T. Lavavej.

Various library tools exist to mitigate these obstacles. Although sufficient for most cases, I find the current library solutions to be fragmented, lacking in features, and unintuitive (e.g. `second_argument_type` instead of `arg<1>`). Skipping over the deprecated members of the `std::mem_fun` family, here's the shortlist of current library solutions:

`std::result_of`

`std::mem_fn`
  - `result_type`
  - `argument_type`
  - `first_argument_type`
  - `second_argument_type`

`std::bind`
  - `result_type`
    
`std::function`
  - `result_type`
  - `argument_type`
  - `first_argument_type`
  - `second_argumenti_type`

`boost::function_traits`
  - `arity`
  - `result_type`
  - `arg1_type`
  - `arg2_type`
  - `arg3_type`
  - etc.
  
`std::invoke`

`std::function` is somewhat of out of place in this list, but I included it for completeness. `std::invoke` is certainly useful for generic programming. And, of course, `auto`, and `decltype` go a very long way on their own. However, `boost::function_traits` offers perhaps the most complete type-level information of the tools listed above. From a metaprogramming perspective, there is so much more we can glean from a function type. If you've ever been compelled to specialize a template for a function type, then CLBL might be for you.

CLBL provides a comprehensive and uniform metaprogramming interface for every callable type in C++. The goal of CLBL's kitchen sink approach is to abstract the dirty work required to perform type-level manipulation and inspection of callable types, collect every piece of information that can be gathered, and repackage it in the form of a tidy, accessible, coherent collection of metaprogramming tools.

Specifically, CLBL provides (*read: plans to provide*) the following advantages over existing solutions:

1. assisted overload resolution
2. implicit `std::function` instantiation
3. cv-qualified `std::function` objects
4.  perfect-forwarding `std::function` objects
4. careful considerations for object lifetimes and ref-qualified overloads 
5. qualifiers-as-values: CLBL employs value-syntax manipulation of reference, const, and volatile for both types and PMF overloads
6. detection of minimum and maximum arity to account for default parameters and overloads
7. detection of C-style varargs
8. optional disabling of argument slicing
9. Testing callability with arbitrary argument types (i.e. You give us the arguments, we handle the SFINAE return `bool`)
10. argument types packaged into `std::tuple`
11. `bind`-like semantics with `std::placeholders` - CLBL retains the bind signature, which ultimately enables invisible, seamless conversion from `bind` expressions to `std::function` objects
12. Currently supporting for Clang 3.7 and later, GCC 5.2.1 and later
13. A full range of PMF type manipulations - add/remove qualifiers, swap classes, build casts programmatically
12. `std::invoke`-inspired wrapper semantics - a single function name is used to create CLBL wrappers (without type erasure), accepting the following as input:
    - function pointers
    - function references
    - member function pointers
    - member data pointers
    - `std::integral_constant`-wrapped pointers
    - callable objects
    - pointers and smart pointers to callable objects
    - reference-wrapped callable objects
    - objects with overloaded/templated `operator()`

CLBL provides an identical interface for all callable types. Onces a callable type is lifted into a CLBL wrapper, you don't need to know what it is or where it came from (but you can still find out, of course).

CLBL is not yet ready for primetime - some features are not fully implemented and/or tested; meanwhile, documentation is virtually nonexistent -- but we're getting there. For now, this readme is solely intended to guage interest and solicit feedback, rather than to encourage an audit of the code (it's not nearly ready for that!).

You might be thinking, "why on Earth do we need another lambda library?" If so, I'd like to emphasize the fact that CLBL is, first and foremost, a metaprogramming toolkit. The callable wrapper aspect was simply a natural extension of this approach. I believe the convenience of packaging the type information inside of wrappers makes them highly attractive to C++ library writers.

## License
Please see [LICENSE.md](LICENSE.md).

<!-- Links -->
[factory folder]: https://github.com/badair/CLBL/tree/master/include/CLBL/factory
[internal folder]: https://github.com/badair/CLBL/tree/master/include/CLBL/internal
[harden.h]: https://github.com/badair/CLBL/blob/master/include/CLBL/harden.h
[pmf.h]: https://github.com/badair/CLBL/blob/master/include/CLBL/pmf.h
[fwrap.h]: https://github.com/badair/CLBL/blob/master/include/CLBL/fwrap.h
[qualified_type.h]: https://github.com/badair/CLBL/blob/master/include/CLBL/qualified_type.h
[qflags.h]: https://github.com/badair/CLBL/blob/master/include/CLBL/qflags.h
[CLBL.Docs]: https://badair.github.io/CLBL/
[badge.Wandbox]: https://img.shields.io/badge/try%20it-online-blue.svg
[example.Wandbox]: http://melpon.org/wandbox/permlink/TlioDiz6yYNxZFnv
