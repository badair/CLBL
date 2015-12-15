#ifndef INT_CHAR_DEFINITIONS_H
#define INT_CHAR_DEFINITIONS_H

namespace clbl { namespace tests {

    namespace test_id { constexpr auto int_char_func = "int_char_func(int, char)"; }
    extern inline const char* int_char_func(int, char) { return test_id::int_char_func; }

    namespace test_id { constexpr auto int_char_struct_func = "int_char_struct::func(int, char)"; }
    namespace test_id { constexpr auto int_char_struct_op = "int_char_struct::operator()(int, char)"; }
    struct int_char_struct {
        inline const char* func(int, char) { return test_id::int_char_struct_func; }
        inline const char* operator()(int, char) { return test_id::int_char_struct_op; }
    };

    namespace test_id { constexpr auto const_int_char_struct_func = "const_int_char_struct::func(int, char) const"; }
    namespace test_id { constexpr auto const_int_char_struct_op = "const_int_char_struct::operator()(int, char) const"; }
    struct const_int_char_struct {
        inline const char* func(int, char) const { return test_id::const_int_char_struct_func; }
        inline const char* operator()(int, char) const { return test_id::const_int_char_struct_op;}
    };

    namespace test_id { constexpr auto volatile_int_char_struct_func = "volatile_int_char_struct::func(int, char) volatile"; }
    namespace test_id { constexpr auto volatile_int_char_struct_op = "volatile_int_char_struct::operator()(int, char) volatile"; }
    struct volatile_int_char_struct {
        inline const char* func(int, char) volatile { return test_id::volatile_int_char_struct_func; }
        inline const char* operator()(int, char) volatile { return test_id::volatile_int_char_struct_op; }
    };

    namespace test_id { constexpr auto const_volatile_int_char_struct_func = "const_volatile_int_char_struct::func(int, char) const volatile"; }
    namespace test_id { constexpr auto const_volatile_int_char_struct_op = "const_volatile_int_char_struct::operator()(int, char) const volatile"; }
    struct const_volatile_int_char_struct {
        inline const char* func(int, char) const volatile { return test_id::const_volatile_int_char_struct_func; }
        inline const char* operator()(int, char) const volatile { return test_id::const_volatile_int_char_struct_op; }
    };

    namespace test_id { constexpr auto overloaded_int_char_struct_op = "overloaded_int_char_struct::operator()(int, char)"; }
    namespace test_id { constexpr auto overloaded_int_char_struct_op_c = "overloaded_int_char_struct::operator()(int, char) const"; }
    namespace test_id { constexpr auto overloaded_int_char_struct_op_v = "overloaded_int_char_struct::operator()(int, char) volatile"; }
    namespace test_id { constexpr auto overloaded_int_char_struct_op_cv = "overloaded_int_char_struct::operator()(int, char) const volatile"; }

    struct overloaded_int_char_struct {
        inline const char* operator()(int, char) { return test_id::overloaded_int_char_struct_op; }
        inline const char* operator()(int, char) const { return test_id::overloaded_int_char_struct_op_c; }
        inline const char* operator()(int, char) volatile { return test_id::overloaded_int_char_struct_op_v; }
        inline const char* operator()(int, char) const volatile { return test_id::overloaded_int_char_struct_op_cv; }

        overloaded_int_char_struct() = default;
        overloaded_int_char_struct(const overloaded_int_char_struct&) = default;
        //overloaded_int_char_struct(volatile overloaded_int_char_struct&) {}
        overloaded_int_char_struct(const volatile overloaded_int_char_struct&) {}
    };
}}

#endif