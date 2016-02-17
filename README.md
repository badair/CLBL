# CLBL

![Build Status](https://travis-ci.org/badair/CLBL.svg?branch=master)

[![Join the chat at https://gitter.im/badair/CLBL](https://badges.gitter.im/badair/CLBL.svg)](https://gitter.im/badair/CLBL?utm_source=badge&utm_medium=badge&utm_campaign=pr-badge&utm_content=badge)

<!--</a> <a target="_blank" href="http://melpon.org/wandbox/permlink/TlioDiz6yYNxZFnv">![Try it online][badge.wandbox]</a>-->

CLBL is a standalone C++14 header-only library for callable types, intended to supersede `std::bind`, `std::mem_fn`, `std::invoke`, `std::result_of`, and `boost::function_traits` in one fell swoop. Instead of providing drop-in replacements for these tools, CLBL intends to provide a unified, exhaustive interface for interacting with C++ callable types. This library is intended primarily for C++ template metaprogrammers and library writers.

I will not revisit documentation until the library meets its aforementioned goals and passes comprehensive test coverage. My previous documentation attempts reflect neither the current state of this repository, nor the current goals thereof. I'll answer any questions posted to the issue list or Gitter.

All test cases pass for Clang 3.6 and later. There is partial support for MSVC. I have not yet tested on GCC. I intend for this library to ultimately work with all 3 compilers.

Projected v0.1 release: May 1, 2016

Code notes:

- [fwrap.h] and [pmf.h] contain perhaps the important code in the library.

- [harden.h] contains the code used to perform overload resolution.

- The [factory folder] contains classes that `clbl::fwrap` uses to create callable wrappers.

- The [internal folder] contains the actual wrapper class templates. A call to `clbl::fwrap` always returns an instance of one of these wrappers, depending on the argument types. The type aliases in these wrappers are intended to be uniform, and play a large role in the intended usefulness of this library.

- The [qflags.h] file contains bit flags that represent C++ value categories and qualifiers, which are integral to CLBL's implementation (see also [qualified_type.h]).

I ultimately plan to propose this project as an addition to Boost. Please contact me if you are interested in contributing to this project -- I'll be more than happy to catch you up to speed. I'm eager to hear any feedback you might have as well. 

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
