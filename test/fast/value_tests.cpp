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

#ifdef CLBL_VALUE_TESTS
    std::cout << "running CLBL_VALUE_TESTS" << std::endl;

    {
        //decays to function pointer
        auto f = fwrap([](int& i) {++i;});
        auto i = 0;
        f(i);
        assert(i == 1);
    }
    {
        //passing ref-capturing lambda by value
        auto i = 0;
        auto f = fwrap([&i]{++i;});
        
        f();
        assert(i == 1);
    }
    {
        //passing by reference wrapper
        auto i = 0;
        auto lam = [&i]{++i;};
        auto f = fwrap(std::ref(lam));

        f();
        assert(i == 1);
    }
    {
        //passing generic lambda by value
        auto i = 0;
        auto f = fwrap([](auto&& i) {increment_T(i);});

        f(i);
        assert(i == 1);
    }
    {
        //passing generic lambda object by reference wrapper
        auto i = 0;
        auto lam = [](auto&& i) {increment_T(i);};
        auto f = fwrap(std::ref(lam));

        f(i);
        assert(i == 1);
    }
    
#endif

    return 0;
}

