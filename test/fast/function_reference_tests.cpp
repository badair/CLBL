/*

Copyright Barrett Adair 2015
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)

*/

#include "test.hpp"

#include <memory>
#include <iostream>
#include <CLBL/clbl.hpp>

using namespace clbl::test;
using namespace clbl;

template<typename T>
void increment_T(T& i) {
    ++i;
}

int main() {

    {
        auto f = fwrap(increment_T<int>);

        int i = 0;
        f(i);
        assert(i == 1);
    }

    {
        auto f = fwrap<void(int&)>(increment_T);

        int i = 0;
        f(i);
        assert(i == 1);
    }

    return 0;
}