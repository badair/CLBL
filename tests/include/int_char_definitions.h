/*

Copyright Barrett Adair 2015
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)

*/

#ifndef INT_CHAR_DEFINITIONS_H
#define INT_CHAR_DEFINITIONS_H

#include <string>

namespace clbl { namespace tests {

    namespace test_id { std::string int_char_func = "int_char_func(int, char)"; }
    extern inline std::string int_char_func(int, char) { return test_id::int_char_func; }

    namespace test_id { std::string int_char_struct_func = "int_char_struct::func(int, char)"; }
    namespace test_id { std::string int_char_struct_op = "int_char_struct::operator()(int, char)"; }
    struct int_char_struct {
        inline std::string func(int, char) { return test_id::int_char_struct_func; }
        inline std::string operator()(int, char) { return test_id::int_char_struct_op; }
    };

    namespace test_id { std::string const_int_char_struct_func = "const_int_char_struct::func(int, char) const"; }
    namespace test_id { std::string const_int_char_struct_op = "const_int_char_struct::operator()(int, char) const"; }
    struct const_int_char_struct {
        inline std::string func(int, char) const { return test_id::const_int_char_struct_func; }
        inline std::string operator()(int, char) const { return test_id::const_int_char_struct_op;}
    };

    namespace test_id { std::string volatile_int_char_struct_func = "volatile_int_char_struct::func(int, char) volatile"; }
    namespace test_id { std::string volatile_int_char_struct_op = "volatile_int_char_struct::operator()(int, char) volatile"; }
    struct volatile_int_char_struct {
        inline std::string func(int, char) volatile { return test_id::volatile_int_char_struct_func; }
        inline std::string operator()(int, char) volatile { return test_id::volatile_int_char_struct_op; }
    };

    namespace test_id { std::string const_volatile_int_char_struct_func = "const_volatile_int_char_struct::func(int, char) const volatile"; }
    namespace test_id { std::string const_volatile_int_char_struct_op = "const_volatile_int_char_struct::operator()(int, char) const volatile"; }
    struct const_volatile_int_char_struct {
        inline std::string func(int, char) const volatile { return test_id::const_volatile_int_char_struct_func; }
        inline std::string operator()(int, char) const volatile { return test_id::const_volatile_int_char_struct_op; }
    };

    namespace test_id { std::string overloaded_int_char_struct_op = "overloaded_int_char_struct::operator()(int, char)"; }
    namespace test_id { std::string overloaded_int_char_struct_op_c = "overloaded_int_char_struct::operator()(int, char) const"; }
    namespace test_id { std::string overloaded_int_char_struct_op_v = "overloaded_int_char_struct::operator()(int, char) volatile"; }
    namespace test_id { std::string overloaded_int_char_struct_op_cv = "overloaded_int_char_struct::operator()(int, char) const volatile"; }

    struct overloaded_int_char_struct {
        inline std::string operator()(int, char) { return test_id::overloaded_int_char_struct_op; }
        inline std::string operator()(int, char) const { return test_id::overloaded_int_char_struct_op_c; }
        inline std::string operator()(int, char) volatile { return test_id::overloaded_int_char_struct_op_v; }
        inline std::string operator()(int, char) const volatile { return test_id::overloaded_int_char_struct_op_cv; }

        overloaded_int_char_struct() = default;
        overloaded_int_char_struct(const overloaded_int_char_struct&) = default;
        overloaded_int_char_struct(const volatile overloaded_int_char_struct&) {}
    };
}}

#endif