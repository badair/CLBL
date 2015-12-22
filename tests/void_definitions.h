/*

Copyright Barrett Adair 2015
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)

*/

#ifndef VOID_DEFINITIONS_H
#define VOID_DEFINITIONS_H

namespace clbl { namespace tests {

    namespace test_id { constexpr auto void_func = "void_func()"; }

    extern inline const char* void_func() { return test_id::void_func; }

    namespace test_id { constexpr auto void_struct_func = "void_struct::func()"; }
    namespace test_id { constexpr auto void_struct_op = "void_struct::operator()()"; }

    struct void_struct {
        inline const char* func() { return test_id::void_struct_func; };
        inline const char* operator()() { return test_id::void_struct_op; };
    };

    namespace test_id { constexpr auto const_void_struct_func = "const_void_struct::func() const"; }
    namespace test_id { constexpr auto const_void_struct_op = "const_void_struct::operator()() const"; }

    struct const_void_struct {
        inline const char* func() const { return test_id::const_void_struct_func; };
        inline const char* operator()() const { return test_id::const_void_struct_op; };
    };

    namespace test_id { constexpr auto volatile_void_struct_func = "volatile_void_struct::func() volatile"; }
    namespace test_id { constexpr auto volatile_void_struct_op = "volatile_void_struct::operator()() volatile"; }

    struct volatile_void_struct {
        inline volatile_void_struct() {}
        inline volatile_void_struct(const volatile_void_struct&) {}
        inline const char* func() volatile { return test_id::volatile_void_struct_func; };
        inline const char* operator()() volatile { return test_id::volatile_void_struct_op; };
    };
}}

#endif