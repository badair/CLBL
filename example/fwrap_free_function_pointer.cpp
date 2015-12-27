#include <cassert>
#include <CLBL/clbl.h>

auto func(int i, int j, int k){
	return i + j + k;
}

 int main() {
 	auto callable = clbl::fwrap(&func);
 	assert(callable(1,2,3) == 6);
 }