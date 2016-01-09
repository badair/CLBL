/*

Copyright Barrett Adair 2015
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)

*/

#include "test.h"
#include "void_definitions.h"
#include "int_definitions.h"
#include "int_char_definitions.h"

#include <sstream>
#include <memory>
#include <iostream>

#include <CLBL/clbl.h>

using namespace clbl::tests;
using namespace clbl;

static constexpr auto destroyed_message = "destroyed";

struct scope_test {
    std::stringstream& ss;

    scope_test(std::stringstream& s) : ss(s) {}

    void fwrap(int i) { ss << i; }

    ~scope_test() {
        ss << destroyed_message;
    }
};

int main() {

#ifdef CLBL_SHARED_PTR_TESTS
    std::cout << "running CLBL_SHARED_PTR_TESTS" << std::endl;

    std::stringstream ss{};

    {
        std::shared_ptr<scope_test> second_ptr;

        {
            auto first_ptr = std::make_shared<scope_test>(ss);
            second_ptr = first_ptr;
            auto f = fwrap(first_ptr, &scope_test::fwrap);
            f(1);
        }

        assert(ss.str() == "1");
        ss.str("");
    }

    assert(ss.str() == destroyed_message);

#endif

    return 0;
}