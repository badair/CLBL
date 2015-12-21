#include "test.h"
#include <CLBL/clbl.h>
#include "int_char_definitions.h"

using namespace clbl::tests;
using namespace clbl;

#include<iostream>
#include<type_traits>

namespace ol_tests {
    struct return_types {
        template<typename T>
        auto operator()(T t) {
            return t;
        }

        template<typename T>
        auto operator()(T t) const {
            return "const";
        }
    };
}

void overload_tests() {
    using namespace ol_tests;

#ifdef CLBL_OVERLOAD_TESTS
    std::cout << "running CLBL_OVERLOAD_TESTS" << std::endl;

    {
        overloaded_int_char_struct overloaded_object{};

        //must be const because the fwrap returns a prvalue which binds to a const reference during perfect forwarding
        auto h = harden<const char*(int, char) const>(fwrap(&overloaded_object));

        TEST(h(1, 'c') == test_id::overloaded_int_char_struct_op_c);

        //calling clbl::fwrap with a result from clbl::fwrap
        ////must be const because the fwrap returns a prvalue which binds to a const reference during perfect forwarding
        auto identity_func = harden<const char*(int, char) const>(fwrap(&h));

        TEST(identity_func(1, 'c') == h(1, 'c'));
    }
    {
        //testing automatic return type deduction
        auto f = fwrap(return_types{});

        {
            auto hardened = harden<auto_(int)>(f);
            static_assert(std::is_same<decltype(hardened)::return_type, int>::value, "");
            auto stdf = convert_to<std::function>(hardened);
            TEST(stdf(1) == 1);
            
        }
        {
            auto hardened = harden<auto_(int) const>(f);
            static_assert(std::is_same<decltype(hardened)::return_type, const char*>::value, "");
            auto stdf = convert_to<std::function>(hardened);
            TEST(stdf(1) == "const");
        }
    }

#endif
}