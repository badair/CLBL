#include "test.h"
#include "CLBL/clbl.h"
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
        auto f = fwrap(&mutable_object, &mutable_struct::increment);
        auto stdf = convert_to<std::function>(f);

        f();
        stdf();
        call_std_function(stdf);

        TEST(mutable_object.value == 3);
    }

    {
        //testing cv-correctness of clbl callables converted to std::function
        auto overloaded_object = overloaded_int_char_struct{};

        auto normal = fwrap(&overloaded_object);
        auto c = fwrap(&overloaded_object);
        auto v = fwrap(&overloaded_object);
        auto cv = fwrap(&overloaded_object);

        auto hnormal = harden<const char*(int, char)>(normal);
        auto hc = harden<const char*(int, char) const>(c);
        auto hv = harden<const char*(int, char) volatile>(v);
        auto hcv = harden<const char*(int, char) const volatile>(cv);

        std::cout << std::endl << "line 62: " << hnormal(1, 'c') << std::endl << hc(1, 'c') << std::endl << hv(1, 'c') << std::endl << hcv(1, 'c') << std::endl;

        TEST(hnormal(1, 'c') == test_id::overloaded_int_char_struct_op);
        TEST(hc(1, 'c') == test_id::overloaded_int_char_struct_op_c);
        TEST(hv(1, 'c') == test_id::overloaded_int_char_struct_op_v);
        TEST(hcv(1, 'c') == test_id::overloaded_int_char_struct_op_cv);

        CLBL_STATIC_ASSERT_NOT_DEEP_CONST(hnormal);
        CLBL_STATIC_ASSERT_NOT_DEEP_CONST(hc);
        CLBL_STATIC_ASSERT_NOT_DEEP_CONST(hv);
        CLBL_STATIC_ASSERT_NOT_DEEP_CONST(hcv);
        CLBL_STATIC_ASSERT_NOT_DEEP_VOLATILE(hnormal);
        CLBL_STATIC_ASSERT_NOT_DEEP_VOLATILE(hc);
        CLBL_STATIC_ASSERT_NOT_DEEP_VOLATILE(hv);
        CLBL_STATIC_ASSERT_NOT_DEEP_VOLATILE(hcv);

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
    {
        auto overloaded_object = overloaded_int_char_struct{};

        auto normal = fwrap(&overloaded_object);
        auto c = fwrap(&overloaded_object);
        auto v = fwrap(&overloaded_object);
        auto cv = fwrap(&overloaded_object);

        CLBL_STATIC_ASSERT_NOT_DEEP_CONST(normal);
        CLBL_STATIC_ASSERT_NOT_DEEP_CONST(c);
        CLBL_STATIC_ASSERT_NOT_DEEP_CONST(v);
        CLBL_STATIC_ASSERT_NOT_DEEP_CONST(cv);
        CLBL_STATIC_ASSERT_NOT_DEEP_VOLATILE(normal);
        CLBL_STATIC_ASSERT_NOT_DEEP_VOLATILE(c);
        CLBL_STATIC_ASSERT_NOT_DEEP_VOLATILE(v);
        CLBL_STATIC_ASSERT_NOT_DEEP_VOLATILE(cv);

        auto nested_normal = harden<const char*(int, char)>(fwrap(&normal));
        auto nested_c = harden<const char*(int, char) const>(fwrap(&c));
        auto nested_v = harden<const char*(int, char) volatile>(fwrap(&v));
        auto nested_cv = harden<const char*(int, char) const volatile>(fwrap(&cv));

        std::cout << std::endl << "line 114: " << nested_normal(1, 'c') << std::endl << nested_c(1, 'c') << std::endl << nested_v(1, 'c') << std::endl << nested_cv(1, 'c') << std::endl;

        TEST(nested_normal(1, 'c') == test_id::overloaded_int_char_struct_op);
        TEST(nested_c(1, 'c') == test_id::overloaded_int_char_struct_op_c);
        TEST(nested_v(1, 'c') == test_id::overloaded_int_char_struct_op_v);
        TEST(nested_cv(1, 'c') == test_id::overloaded_int_char_struct_op_cv);

        CLBL_STATIC_ASSERT_NOT_DEEP_CONST(nested_normal);
        CLBL_STATIC_ASSERT_NOT_DEEP_CONST(nested_c);
        CLBL_STATIC_ASSERT_NOT_DEEP_CONST(nested_v);
        CLBL_STATIC_ASSERT_NOT_DEEP_CONST(nested_cv);
        CLBL_STATIC_ASSERT_NOT_DEEP_VOLATILE(nested_normal);
        CLBL_STATIC_ASSERT_NOT_DEEP_VOLATILE(nested_c);
        CLBL_STATIC_ASSERT_NOT_DEEP_VOLATILE(nested_v);
        CLBL_STATIC_ASSERT_NOT_DEEP_VOLATILE(nested_cv);

        auto stdn = convert_to<std::function>(nested_normal);
        auto stdc = convert_to<std::function>(nested_c);
        auto stdv = convert_to<std::function>(nested_v);
        auto stdcv = convert_to<std::function>(nested_cv);

        std::cout << std::endl << "line 135: " << stdn(1, 'c') << std::endl << stdc(1, 'c') << std::endl << stdv(1, 'c') << std::endl << stdcv(1, 'c') << std::endl;

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

        auto normal = fwrap(&overloaded_object);
        const auto c = fwrap(&overloaded_object);
        volatile auto v = fwrap(&overloaded_object);
        const volatile auto cv = fwrap(&overloaded_object);

        CLBL_STATIC_ASSERT_NOT_DEEP_CONST(normal);
        CLBL_STATIC_ASSERT_NOT_DEEP_CONST(c);
        CLBL_STATIC_ASSERT_NOT_DEEP_CONST(v);
        CLBL_STATIC_ASSERT_NOT_DEEP_CONST(cv);
        CLBL_STATIC_ASSERT_NOT_DEEP_VOLATILE(normal);
        CLBL_STATIC_ASSERT_NOT_DEEP_VOLATILE(c);
        CLBL_STATIC_ASSERT_NOT_DEEP_VOLATILE(v);
        CLBL_STATIC_ASSERT_NOT_DEEP_VOLATILE(cv);

        auto hnormal = harden<const char*(int, char)>(normal);
        auto hc = harden<const char*(int, char) const>(c);
        auto hv = harden<const char*(int, char) volatile>(v);
        auto hcv = harden<const char*(int, char) const volatile>(cv);

        CLBL_STATIC_ASSERT_NOT_DEEP_CONST(hnormal);
        CLBL_STATIC_ASSERT_NOT_DEEP_CONST(hc);
        CLBL_STATIC_ASSERT_NOT_DEEP_CONST(hv);
        CLBL_STATIC_ASSERT_NOT_DEEP_CONST(hcv);
        CLBL_STATIC_ASSERT_NOT_DEEP_VOLATILE(hnormal);
        CLBL_STATIC_ASSERT_NOT_DEEP_VOLATILE(hc);
        CLBL_STATIC_ASSERT_NOT_DEEP_VOLATILE(hv);
        CLBL_STATIC_ASSERT_NOT_DEEP_VOLATILE(hcv);

        auto stdn = convert_to<std::function>(hnormal);
        auto stdc = convert_to<std::function>(hc);
        auto stdv = convert_to<std::function>(hv);
        auto stdcv = convert_to<std::function>(hcv);

        static_assert(std::is_same<decltype(stdn), std::function<const char* (forward<int>, forward<char>) > >::value, "");

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
        const auto overloaded_object = overloaded_int_char_struct{};

        auto normal = fwrap(&overloaded_object);
        const auto c = fwrap(&overloaded_object);
        volatile auto v = fwrap(&overloaded_object);
        const volatile auto cv = fwrap(&overloaded_object);

        CLBL_STATIC_ASSERT_DEEP_CONST(normal);
        CLBL_STATIC_ASSERT_DEEP_CONST(c);
        CLBL_STATIC_ASSERT_DEEP_CONST(v);
        CLBL_STATIC_ASSERT_DEEP_CONST(cv);
        CLBL_STATIC_ASSERT_NOT_DEEP_VOLATILE(normal);
        CLBL_STATIC_ASSERT_NOT_DEEP_VOLATILE(c);
        CLBL_STATIC_ASSERT_NOT_DEEP_VOLATILE(v);
        CLBL_STATIC_ASSERT_NOT_DEEP_VOLATILE(cv);

        //should cause compile error if called: auto hnormal = harden<const char*(int, char)>(normal);
        const auto hc = harden<const char*(int, char) const>(c);
        //should cause compile error if called: volatile auto hv = harden<const char*(int, char) volatile>(v);
        const volatile auto hcv = harden<const char*(int, char) const volatile>(cv);

        CLBL_STATIC_ASSERT_DEEP_CONST(hc);
        CLBL_STATIC_ASSERT_DEEP_CONST(hcv);
        CLBL_STATIC_ASSERT_NOT_DEEP_VOLATILE(hc);
        CLBL_STATIC_ASSERT_NOT_DEEP_VOLATILE(hcv);

        auto stdc = convert_to<std::function>(hc);
        auto stdcv = convert_to<std::function>(hcv);

        TEST(stdc(1, 'c') == test_id::overloaded_int_char_struct_op_c);
        TEST(stdcv(1, 'c') == test_id::overloaded_int_char_struct_op_cv);

        TEST(int_char_std_function_result(stdc) == test_id::overloaded_int_char_struct_op_c);
        TEST(int_char_std_function_result(stdcv) == test_id::overloaded_int_char_struct_op_cv);
    }
    {
        volatile auto overloaded_object = overloaded_int_char_struct{};

        auto normal = fwrap(&overloaded_object);
        const auto c = fwrap(&overloaded_object);
        volatile auto v = fwrap(&overloaded_object);
        const volatile auto cv = fwrap(&overloaded_object);

        CLBL_STATIC_ASSERT_NOT_DEEP_CONST(normal);
        CLBL_STATIC_ASSERT_NOT_DEEP_CONST(c);
        CLBL_STATIC_ASSERT_NOT_DEEP_CONST(v);
        CLBL_STATIC_ASSERT_NOT_DEEP_CONST(cv);
        CLBL_STATIC_ASSERT_DEEP_VOLATILE(normal);
        CLBL_STATIC_ASSERT_DEEP_VOLATILE(c);
        CLBL_STATIC_ASSERT_DEEP_VOLATILE(v);
        CLBL_STATIC_ASSERT_DEEP_VOLATILE(cv);

        //should cause compile error if called: auto hnormal = harden<const char*(int, char)>(normal);
        //should cause compile error if called: const auto hc = harden<const char*(int, char) const>(c);
        volatile auto hv = harden<const char*(int, char) volatile>(v);
        const volatile auto hcv = harden<const char*(int, char) const volatile>(cv);

        CLBL_STATIC_ASSERT_DEEP_VOLATILE(hv);
        CLBL_STATIC_ASSERT_DEEP_VOLATILE(hcv);

        auto stdv = convert_to<std::function>(hv);
        auto stdcv = convert_to<std::function>(hcv);

        TEST(stdv(1, 'c') == test_id::overloaded_int_char_struct_op_v);
        TEST(stdcv(1, 'c') == test_id::overloaded_int_char_struct_op_cv);

        TEST(int_char_std_function_result(stdv) == test_id::overloaded_int_char_struct_op_v);
        TEST(int_char_std_function_result(stdcv) == test_id::overloaded_int_char_struct_op_cv);
    }
#endif
}