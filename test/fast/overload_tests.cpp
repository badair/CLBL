/*

Copyright Barrett Adair 2015
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)

*/

#include "test.hpp"
#include "int_char_definitions.hpp"

#include<iostream>
#include<type_traits>

#include <CLBL/clbl.hpp>

using namespace clbl::test;
using namespace clbl;

struct return_types {
    template<typename T>
    auto operator()(T t) {
        return t;
    }

    template<typename T>
    auto operator()(T) const {
        return std::string{"const"};
    }
};

int main() {

    {
        overloaded_int_char_struct overloaded_object{};

        //must be const because the fwrap returns a prvalue which binds to a const reference during perfect forwarding
        auto h = harden<std::string(int, char) const>(fwrap(&overloaded_object));

        assert(h(1, 'c') == test_id::overloaded_int_char_struct_op_c);

        //calling clbl::fwrap with a result from clbl::fwrap
        ////must be const because the fwrap returns a prvalue which binds to a const reference during perfect forwarding
        auto identity_func = harden<std::string(int, char) const>(fwrap(&h));

        assert(identity_func(1, 'c') == h(1, 'c'));
    }
    {
        //testing automatic return type deduction
        auto f = fwrap(return_types{});

        {
            auto hardened = harden<auto_(int)>(f);
            static_assert(std::is_same<decltype(hardened)::return_type, int>::value, "");
            auto stdf = convert_to<std::function>(hardened);
            assert(stdf(1) == 1);
            
        }
        {
            auto hardened = harden<auto_(int) const>(f);
            static_assert(std::is_same<decltype(hardened)::return_type, std::string>::value, "");
            auto stdf = convert_to<std::function>(hardened);
            assert(stdf(1) == "const");
        }
    }

    return 0;
}