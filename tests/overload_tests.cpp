#include "test.h"
#include "CLBL/clbl.h"
#include "int_char_definitions.h"

using namespace clbl::tests;
using namespace clbl;

#include<iostream>
#include<type_traits>

void overload_tests() {

#ifdef CLBL_OVERLOAD_TESTS
    std::cout << "running CLBL_OVERLOAD_TESTS" << std::endl;

    overloaded_int_char_struct overloaded_object{};

    //must be const because the fwrap returns a prvalue which binds to a const reference during perfect forwarding
    auto h = harden<const char*(int, char) const>(fwrap(&overloaded_object));

    TEST(h(1, 'c') == test_id::overloaded_int_char_struct_op_c);

    //calling clbl::fwrap with a result from clbl::fwrap
    ////must be const because the fwrap returns a prvalue which binds to a const reference during perfect forwarding
    auto identity_func = harden<const char*(int, char) const>(fwrap(&h));

    TEST(identity_func(1, 'c') == h(1, 'c'));

#endif
}