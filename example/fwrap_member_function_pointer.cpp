/*

@copyright Barrett Adair 2015
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)

*/

#include <functional>
#include <memory>
#include <cassert>

#include <CLBL/clbl.h>

struct foo {
    void add_ten(int& i) {
        i = i + 10;
    }
};

int main() {

    auto my_foo = foo{};

    //bind the object with a copy
    auto c1 = clbl::fwrap(my_foo, &foo::add_ten);

    //bind the object with a pointer
    auto c2 = clbl::fwrap(&my_foo, &foo::add_ten);

    //bind the object with smart pointers
    auto c3 = clbl::fwrap(std::make_unique<foo>(), &foo::add_ten);
    auto c4 = clbl::fwrap(std::make_shared<foo>(), &foo::add_ten);

    //bind the object with a reference wrapper
    auto c5 = clbl::fwrap(std::ref(my_foo), &foo::add_ten);

    //bind the object with a move
    auto temp = foo{};
    auto c6 = clbl::fwrap(std::move(temp), &foo::add_ten);

    /* TODO update this section - see template_pmfs.cpp for more recent example
    
    CLBL_PMFWRAP yields a wrapper that can potentially leave a
    smaller memory footprint by passing the PMF as a template argument,
    since it is known at compile time. Note that, per the C++ standard,
    a casted PMF cannot be a template value argument. As such,CLBL_PMFWRAP
    will fail to compile if you try to cast the PMF first.
    
    
    auto c7 = CLBL_PMFWRAP(my_foo, &foo::add_ten);
    auto c8 = CLBL_PMFWRAP(&my_foo, &foo::add_ten);
    auto c9 = CLBL_PMFWRAP(std::make_unique<foo>(), &foo::add_ten);
    auto c10 = CLBL_PMFWRAP(std::make_shared<foo>(), &foo::add_ten);
    auto c11 = CLBL_PMFWRAP(std::ref(my_foo), &foo::add_ten);
    */
    auto temp2 = foo{};
    auto c12 = clbl::fwrap(std::move(temp2), &foo::add_ten);

    auto i = 0;

    c1(i);
    c2(i);
    c3(i);
    c4(i);
    c5(i);
    c6(i);

    /*
    c7(i);
    c8(i);
    c9(i);
    c10(i);
    c11(i);*/
    c12(i);

    //assert(i == 10 * 12);

    return 0;
}