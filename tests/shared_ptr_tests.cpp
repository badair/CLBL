#include "test.h"
#include "CLBL/func.h"
#include "void_definitions.h"
#include "int_definitions.h"
#include "int_char_definitions.h"

#include <sstream>
#include <memory>
#include <iostream>

using namespace clbl::tests;
using namespace clbl;

namespace shared_tests {
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

void shared_ptr_tests() {
    using namespace shared_tests;

#ifdef CLBL_SHARED_PTR_TESTS
    std::cout << "running CLBL_SHARED_PTR_TESTS" << std::endl;

    std::stringstream ss{};

    {
        std::shared_ptr<scope_test> second_ptr;

        {
            auto first_ptr = std::make_shared<scope_test>(ss);
            second_ptr = first_ptr;
            auto f = func(first_ptr, &scope_test::func);
            f(1);
        }

        TEST(ss.str() == "1");
        ss.str("");
    }

    TEST(ss.str() == destroyed_message);

#endif
}