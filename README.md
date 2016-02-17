# CLBL

![Build Status](https://travis-ci.org/badair/CLBL.svg?branch=master)

[![Join the chat at https://gitter.im/badair/CLBL](https://badges.gitter.im/badair/CLBL.svg)](https://gitter.im/badair/CLBL?utm_source=badge&utm_medium=badge&utm_campaign=pr-badge&utm_content=badge)

<!--</a> <a target="_blank" href="http://melpon.org/wandbox/permlink/TlioDiz6yYNxZFnv">![Try it online][badge.wandbox]</a>-->

CLBL is a standalone C++14 header-only library for callable types, intended to supersede `std::bind`, `std::mem_fn`, `std::invoke`, `std::result_of`, and `boost::function_traits` in one fell swoop. Instead of providing drop-in replacements for these tools, CLBL intends to provide a unified, exhaustive interface for interacting with C++ callable types. This library is intended primarily for C++ template metaprogrammers and library writers.

I will not revisit documentation until the library meets its aforementioned goals and passes comprehensive test coverage. My previous documentation attempts reflect neither the current state of this repository, nor the current goals thereof. I'll answer any questions posted to the issue list or Gitter.

CLBL/include/CLBL/fwrap.h currently serves as the conceptual entry point for the library, via `clbl::fwrap` (names subject to change - I'm sticking with easy-to-grep ones for now). `pmf.h` also contains some code that is central to the library.

Projected v0.1 release: May 1, 2016

## License
Please see [LICENSE.md](LICENSE.md).


<!-- Links -->
[CLBL.Docs]: https://badair.github.io/CLBL/
[badge.Wandbox]: https://img.shields.io/badge/try%20it-online-blue.svg
[example.Wandbox]: http://melpon.org/wandbox/permlink/TlioDiz6yYNxZFnv
