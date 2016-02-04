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

#define CONSTANT_PTR(pmf) decltype(pmf), pmf

int main() {

    auto my_foo = foo{};

    auto c1 = clbl::fwrap<CONSTANT_PTR(&foo::add_ten)>(std::ref(my_foo));
    auto c2 = clbl::fwrap<CONSTANT_PTR(&foo::add_ten)>(my_foo);
    auto c3 = clbl::fwrap<CONSTANT_PTR(&foo::add_ten)>(&my_foo);
    auto c4 = clbl::fwrap<CONSTANT_PTR(&foo::add_ten)>(std::make_unique<foo>());
	auto c5 = clbl::fwrap<CONSTANT_PTR(&foo::add_ten)>(std::make_shared<foo>());
    auto c6 = clbl::fwrap<CONSTANT_PTR(&foo::add_ten)>();
    
	auto i = 0;

	c1(i);
	c2(i);
	c3(i);
	c4(i);
	c5(i);
    
    c6(std::ref(my_foo), i);
    c6(my_foo, i);
    c6(&my_foo, i);
    c6(std::make_unique<foo>(), i);
    c6(std::make_shared<foo>(), i);
    
	assert(i == 10 * 10);

    return 0;
}