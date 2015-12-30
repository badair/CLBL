/*

@copyright Barrett Adair 2015
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)

*/

#include <memory>
#include <cassert>
#include <cassert>

#include <CLBL/clbl.h>

struct foo {
    auto& operator()(int& i) {
        i = i + 10;
        return i;
    }
};

int main() {
    auto f = foo{};

    auto c1 = clbl::fwrap(f);
    auto c2 = clbl::fwrap(&f);
    auto c3 = clbl::fwrap(std::ref(f));
    auto c4 = clbl::fwrap(std::make_shared<foo>());
    auto c5 = clbl::fwrap(
        [](auto& i) {
            i = i + 10;
            return std::ref(i);
        }
    );

    auto i = 0;
    assert(c1(c2(c3(c4(c5(i))))) == 10 * 5);
    assert(i == 10 * 5);
}