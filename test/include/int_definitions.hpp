/*

Copyright Barrett Adair 2015
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)

*/

#ifndef INT_DEFINITIONS_H
#define INT_DEFINITIONS_H

#include <string>

namespace clbl { namespace test {

    namespace test_id { std::string int_func = "int_func(int i)"; }
    extern inline std::string int_func(int) { return test_id::int_func; }

    namespace test_id { std::string int_struct_func = "int_struct::func(int)"; }
    namespace test_id { std::string int_struct_op = "int_struct::operator()(int)"; }
    struct int_struct {
        inline std::string func(int) { return test_id::int_struct_func; }
        inline std::string operator()(int) { return test_id::int_struct_op; }
    };

    namespace test_id { std::string const_int_struct_func = "const_int_struct::func(int) const"; }
    namespace test_id { std::string const_int_struct_op = "const_int_struct::operator()(int) const"; }
    struct const_int_struct {
        inline std::string func(int) const { return test_id::const_int_struct_func; }
        inline std::string operator()(int) const { return test_id::const_int_struct_op; }
    };
}}

#endif