/*

@copyright Barrett Adair 2015
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)

*/

#include <cassert>
#include <CLBL/clbl.h>

auto func(int i, int j, int k){
	return i + j + k;
}

 int main() {
 	auto callable = clbl::fwrap(&func);
 	assert(callable(1,2,3) == 6);
 }