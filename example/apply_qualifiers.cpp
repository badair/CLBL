/*

@copyright Barrett Adair 2015
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)

*/

#include <type_traits>
#include <quali/quali.hpp>

using namespace quali;

int main() {
	using cv_int = qualify<int, const_ | volatile_>;
	static_assert(std::is_same<cv_int, const volatile int>::value, "");
	return 0;
}