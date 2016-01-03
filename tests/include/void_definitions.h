/*

Copyright Barrett Adair 2015
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)

*/

#ifndef VOID_DEFINITIONS_H
#define VOID_DEFINITIONS_H

namespace clbl { namespace tests {

    namespace test_id { std::string void_func = "void_func()"; }

    extern inline std::string void_func() { return test_id::void_func; }

    namespace test_id { std::string void_struct_func = "void_struct::func()"; }
    namespace test_id { std::string void_struct_op = "void_struct::operator()()"; }

    struct void_struct {
        inline std::string func() { return test_id::void_struct_func; };
        inline std::string operator()() { return test_id::void_struct_op; };
    };

    namespace test_id { std::string const_void_struct_func = "const_void_struct::func() const"; }
    namespace test_id { std::string const_void_struct_op = "const_void_struct::operator()() const"; }

    struct const_void_struct {
        inline std::string func() const { return test_id::const_void_struct_func; };
        inline std::string operator()() const { return test_id::const_void_struct_op; };
    };

    namespace test_id { std::string volatile_void_struct_func = "volatile_void_struct::func() volatile"; }
    namespace test_id { std::string volatile_void_struct_op = "volatile_void_struct::operator()() volatile"; }

    struct volatile_void_struct {
        inline volatile_void_struct() = default;
        inline volatile_void_struct(const volatile_void_struct&) = default;
        inline volatile_void_struct(volatile_void_struct&&) = default;
        inline volatile_void_struct(const volatile volatile_void_struct&) {}
        inline std::string func() volatile { return test_id::volatile_void_struct_func; };
        inline std::string operator()() volatile { return test_id::volatile_void_struct_op; };
    };
}}

#endif