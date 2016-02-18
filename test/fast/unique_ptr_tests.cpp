/*

Copyright Barrett Adair 2015
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)

*/

#include "test.hpp"
#include "void_definitions.hpp"
#include "int_definitions.hpp"
#include "int_char_definitions.hpp"

#include <sstream>
#include <memory>
#include <iostream>
#include <string>
#include <CLBL/clbl.hpp>

using namespace clbl::test;
using namespace clbl;

static constexpr auto destroyed_message = "destroyed";

struct scope_test {
    std::stringstream& ss;

    scope_test(std::stringstream& s) : ss(s) {}

    void func(int i) { ss << i; }

    ~scope_test() {
        ss << destroyed_message;
    }
};

int main() {

#ifdef CLBL_UNIQUE_PTR_TESTS
    std::cout << "running CLBL_UNIQUE_PTR_TESTS" << std::endl;

    {
        std::stringstream ss{};

        {
            auto ptr = std::make_unique<scope_test>(ss);
            auto f = fwrap(std::move(ptr), &scope_test::func);
            f(1);
        }

        auto expected_result = std::string{ "1" } +std::string{ destroyed_message };
        assert(ss.str() == expected_result);
    }

#endif

    return 0;
}