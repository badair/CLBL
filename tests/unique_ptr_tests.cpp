#include "test.h"
#include "CLBL/func.h"
#include "void_definitions.h"
#include "int_definitions.h"
#include "int_char_definitions.h"

#include <sstream>
#include <memory>
#include <iostream>
#include <string>

using namespace clbl::tests;
using namespace clbl;

namespace unique_tests {
    static constexpr auto destroyed_message = "destroyed";

    struct scope_test {
        std::stringstream& ss;

        scope_test(std::stringstream& s) : ss(s) {}

        void func(int i) { ss << i; }

        ~scope_test() {
            ss << destroyed_message;
        }
    };
}

void unique_ptr_tests() {

    using namespace unique_tests;

#ifdef CLBL_UNIQUE_PTR_TESTS
    std::cout << "running CLBL_UNIQUE_PTR_TESTS" << std::endl;

    std::stringstream ss{};
    
    {
        std::cout << "creating unique_ptr" << std::endl;

        auto ptr = std::make_unique<scope_test>(ss);

        std::cout << "creating unique_ptr func" << std::endl;
        auto f = func(std::move(ptr), &scope_test::func);

        std::cout << "calling unique_ptr func" << std::endl;
        f(1);

        std::cout << "destroying" << std::endl;
    }

    auto expected_result = std::string{ "1" } + std::string{ destroyed_message };
    TEST(ss.str() == expected_result);

#endif
}