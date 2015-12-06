#include "test.h"
#include "CLBL/func.h"
#include "int_char_definitions.h"

#include<functional>
#include<iostream>

#include<iostream>

using namespace clbl::tests;
using namespace clbl;

auto int_char_std_function_result(std::function<const char*(int, char)> f) {
    return f(1, 'c');
}

void call_std_function(std::function<void()> f) {
    f();
}

void conversion_tests() {

#ifdef CLBL_CONVERSION_TESTS
    std::cout << "running CLBL_CONVERSION_TESTS" << std::endl;

    struct mutable_struct {
        int value;
        mutable_struct(int i) : value(i) {}

        void increment() {
            ++value;
        }
    };
    {
        //testing object reference preservation when converted to std::function
        auto mutable_object = mutable_struct{ 0 };
        auto f = func(&mutable_object, &mutable_struct::increment);
        auto stdf = convert_to<std::function>(f);

        f();
        stdf();
        call_std_function(stdf);

        TEST(mutable_object.value == 3);
    }

    {
        //testing cv-correctness of clbl callables converted to std::function
        auto overloaded_object = overloaded_int_char_struct{};

        auto normal = func(&overloaded_object);
        auto c = func(&overloaded_object);
        auto v = func(&overloaded_object);
        auto cv = func(&overloaded_object);

        auto hnormal = harden<const char*(int, char)>(normal);
        auto hc = harden<const char*(int, char) const>(c);
        auto hv = harden<const char*(int, char) volatile>(v);
        auto hcv = harden<const char*(int, char) const volatile>(cv);

        STATIC_ASSERT_NOT_CLBL_CONST(hnormal);
        STATIC_ASSERT_NOT_CLBL_CONST(hc);
        STATIC_ASSERT_NOT_CLBL_CONST(hv);
        STATIC_ASSERT_NOT_CLBL_CONST(hcv);
        STATIC_ASSERT_NOT_CLBL_VOLATILE(hnormal);
        STATIC_ASSERT_NOT_CLBL_VOLATILE(hc);
        STATIC_ASSERT_NOT_CLBL_VOLATILE(hv);
        STATIC_ASSERT_NOT_CLBL_VOLATILE(hcv);

        auto stdn = convert_to<std::function>(hnormal);
        auto stdc = convert_to<std::function>(hc);
        auto stdv = convert_to<std::function>(hv);
        auto stdcv = convert_to<std::function>(hcv);

        STATIC_ASSERT_NOT_CLBL_CONST(hnormal);
        STATIC_ASSERT_NOT_CLBL_CONST(hc);
        STATIC_ASSERT_NOT_CLBL_CONST(hv);
        STATIC_ASSERT_NOT_CLBL_CONST(hcv);
        STATIC_ASSERT_NOT_CLBL_VOLATILE(hnormal);
        STATIC_ASSERT_NOT_CLBL_VOLATILE(hc);
        STATIC_ASSERT_NOT_CLBL_VOLATILE(hv);
        STATIC_ASSERT_NOT_CLBL_VOLATILE(hcv);

        TEST(stdn(1, 'c') == test_id::overloaded_int_char_struct_op);
        TEST(stdc(1, 'c') == test_id::overloaded_int_char_struct_op_c);
        TEST(stdv(1, 'c') == test_id::overloaded_int_char_struct_op_v);
        TEST(stdcv(1, 'c') == test_id::overloaded_int_char_struct_op_cv);

        TEST(int_char_std_function_result(stdn) == test_id::overloaded_int_char_struct_op);
        TEST(int_char_std_function_result(stdc) == test_id::overloaded_int_char_struct_op_c);
        TEST(int_char_std_function_result(stdv) == test_id::overloaded_int_char_struct_op_v);
        TEST(int_char_std_function_result(stdcv) == test_id::overloaded_int_char_struct_op_cv);
    }
    {
        auto overloaded_object = overloaded_int_char_struct{};

        auto normal = func(&overloaded_object);
        auto c = func(&overloaded_object);
        auto v = func(&overloaded_object);
        auto cv = func(&overloaded_object);

        STATIC_ASSERT_NOT_CLBL_CONST(normal);
        STATIC_ASSERT_NOT_CLBL_CONST(c);
        STATIC_ASSERT_NOT_CLBL_CONST(v);
        STATIC_ASSERT_NOT_CLBL_CONST(cv);
        STATIC_ASSERT_NOT_CLBL_VOLATILE(normal);
        STATIC_ASSERT_NOT_CLBL_VOLATILE(c);
        STATIC_ASSERT_NOT_CLBL_VOLATILE(v);
        STATIC_ASSERT_NOT_CLBL_VOLATILE(cv);

        auto nested_normal = harden<const char*(int, char)>(func(&normal));
        auto nested_c = harden<const char*(int, char) const>(func(&c));
        auto nested_v = harden<const char*(int, char) volatile>(func(&v));
        auto nested_cv = harden<const char*(int, char) const volatile>(func(&cv));

        STATIC_ASSERT_NOT_CLBL_CONST(nested_normal);
        STATIC_ASSERT_NOT_CLBL_CONST(nested_c);
        STATIC_ASSERT_NOT_CLBL_CONST(nested_v);
        STATIC_ASSERT_NOT_CLBL_CONST(nested_cv);
        STATIC_ASSERT_NOT_CLBL_VOLATILE(nested_normal);
        STATIC_ASSERT_NOT_CLBL_VOLATILE(nested_c);
        STATIC_ASSERT_NOT_CLBL_VOLATILE(nested_v);
        STATIC_ASSERT_NOT_CLBL_VOLATILE(nested_cv);

        auto stdn = convert_to<std::function>(nested_normal);
        auto stdc = convert_to<std::function>(nested_c);
        auto stdv = convert_to<std::function>(nested_v);
        auto stdcv = convert_to<std::function>(nested_cv);

        TEST(stdn(1, 'c') == test_id::overloaded_int_char_struct_op);
        TEST(stdc(1, 'c') == test_id::overloaded_int_char_struct_op_c);
        TEST(stdv(1, 'c') == test_id::overloaded_int_char_struct_op_v);
        TEST(stdcv(1, 'c') == test_id::overloaded_int_char_struct_op_cv);

        TEST(int_char_std_function_result(stdn) == test_id::overloaded_int_char_struct_op);
        TEST(int_char_std_function_result(stdc) == test_id::overloaded_int_char_struct_op_c);
        TEST(int_char_std_function_result(stdv) == test_id::overloaded_int_char_struct_op_v);
        TEST(int_char_std_function_result(stdcv) == test_id::overloaded_int_char_struct_op_cv);
    }
    {
        auto overloaded_object = overloaded_int_char_struct{};

        auto normal = func(&overloaded_object);
        const auto c = func(&overloaded_object);
        volatile auto v = func(&overloaded_object);
        const volatile auto cv = func(&overloaded_object);

        STATIC_ASSERT_NOT_CLBL_CONST(normal);
        STATIC_ASSERT_NOT_CLBL_CONST(c);
        STATIC_ASSERT_NOT_CLBL_CONST(v);
        STATIC_ASSERT_NOT_CLBL_CONST(cv);
        STATIC_ASSERT_NOT_CLBL_VOLATILE(normal);
        STATIC_ASSERT_NOT_CLBL_VOLATILE(c);
        STATIC_ASSERT_NOT_CLBL_VOLATILE(v);
        STATIC_ASSERT_NOT_CLBL_VOLATILE(cv);

        auto hnormal = harden<const char*(int, char)>(normal);
        const auto hc = harden<const char*(int, char) const>(c);
        volatile auto hv = harden<const char*(int, char) volatile>(v);
        const volatile auto hcv = harden<const char*(int, char) const volatile>(cv);

        STATIC_ASSERT_NOT_CLBL_CONST(hnormal);
        STATIC_ASSERT_CLBL_CONST(hc);
        STATIC_ASSERT_NOT_CLBL_CONST(hv);
        STATIC_ASSERT_CLBL_CONST(hcv);
        STATIC_ASSERT_NOT_CLBL_VOLATILE(hnormal);
        STATIC_ASSERT_NOT_CLBL_VOLATILE(hc);
        STATIC_ASSERT_CLBL_VOLATILE(hv);
        STATIC_ASSERT_CLBL_VOLATILE(hcv);

        auto stdn = convert_to<std::function>(hnormal);
        auto stdc = convert_to<std::function>(hc);
        auto stdv = convert_to<std::function>(hv);
        auto stdcv = convert_to<std::function>(hcv);

        TEST(stdn(1, 'c') == test_id::overloaded_int_char_struct_op);
        TEST(stdc(1, 'c') == test_id::overloaded_int_char_struct_op_c);
        TEST(stdv(1, 'c') == test_id::overloaded_int_char_struct_op_v);
        TEST(stdcv(1, 'c') == test_id::overloaded_int_char_struct_op_cv);

        TEST(int_char_std_function_result(stdn) == test_id::overloaded_int_char_struct_op);
        TEST(int_char_std_function_result(stdc) == test_id::overloaded_int_char_struct_op_c);
        TEST(int_char_std_function_result(stdv) == test_id::overloaded_int_char_struct_op_v);
        TEST(int_char_std_function_result(stdcv) == test_id::overloaded_int_char_struct_op_cv);
    }

#endif
}