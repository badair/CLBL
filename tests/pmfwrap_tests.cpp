/*
Copyright Barrett Adair 2015
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
*/

#include "test.h"
#include "void_definitions.h"
#include "int_char_definitions.h"

#include<iostream>
#include<functional>
#include <CLBL/clbl.h>

using namespace clbl::tests;
using namespace clbl;

/*
CLBL_PMFWRAP creates a wrapper with a PMF passed as a template argument to reduce
memory footprint of the callable wrapper
*/

int main() {

#ifdef CLBL_VOID_TESTS

#if CLBL_TEST_PART == 1
{
    void_struct void_object{};

    auto f = CLBL_PMFWRAP(std::ref(void_object), &void_struct::func);
    auto g = CLBL_PMFWRAP(void_object,           &void_struct::func);
    auto h = CLBL_PMFWRAP(&void_object,          &void_struct::func);

    run_tests(
        f, test_id::void_struct_func,
        g, test_id::void_struct_func,
        h, test_id::void_struct_func
    );
}
#endif

#if CLBL_TEST_PART == 2
{
    int_char_struct int_char_object{};

    auto f = CLBL_PMFWRAP(std::ref(int_char_object), &int_char_struct::func); 
    auto g = CLBL_PMFWRAP(int_char_object,           &int_char_struct::func);
    auto h = CLBL_PMFWRAP(&int_char_object,          &int_char_struct::func);

    run_tests(
        f, test_id::int_char_struct_func,
        g, test_id::int_char_struct_func,
        h, test_id::int_char_struct_func,
        1, 'c'
    );
}

#endif

#endif

    return 0;
}