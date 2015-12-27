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

    auto c1 = clbl::fwrap(std::ref(my_foo), &foo::add_ten);
    auto c2 = clbl::fwrap(my_foo, &foo::add_ten);
    auto c3 = clbl::fwrap(&my_foo, &foo::add_ten);
    auto c4 = clbl::fwrap(std::make_unique<foo>(), &foo::add_ten);
	auto c5 = clbl::fwrap(std::make_shared<foo>(), &foo::add_ten);

	auto i = 0;

	c1(i);
	c2(i);
	c3(i);
	c4(i);
	c5(i);

	assert(i == 10 * 5);

    return 0;
}