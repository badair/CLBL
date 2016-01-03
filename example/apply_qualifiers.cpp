/*

@copyright Barrett Adair 2015
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)

*/

#include <type_traits>
#include <CLBL/clbl.h>

int main(){
	using cv_int = clbl::apply_qualifiers<int, clbl::const_ | clbl::volatile_>;
	static_assert(std::is_same<cv_int, const volatile int>::value, "");
	return 0;
}