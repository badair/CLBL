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

        std::cout << std::boolalpha << "is_const: " << decltype(normal)::clbl_is_const << "; is_volatile: " << decltype(normal)::clbl_is_volatile << std::endl;
        std::cout << std::boolalpha << "is_const: " << decltype(c)::clbl_is_const << "; is_volatile: " << decltype(c)::clbl_is_volatile << std::endl;
        std::cout << std::boolalpha << "is_const: " << decltype(v)::clbl_is_const << "; is_volatile: " << decltype(v)::clbl_is_volatile << std::endl;
        std::cout << std::boolalpha << "is_const: " << decltype(cv)::clbl_is_const << "; is_volatile: " << decltype(cv)::clbl_is_volatile << std::endl;

        auto hnormal = harden<const char*(int, char)>(normal);
        auto hc = harden<const char*(int, char) const>(c);
        auto hv = harden<const char*(int, char) volatile>(v);
        auto hcv = harden<const char*(int, char) const volatile>(cv);

        std::cout << std::boolalpha << "is_const: " << decltype(hnormal)::clbl_is_const << "; is_volatile: " << decltype(hnormal)::clbl_is_volatile << std::endl;
        std::cout << std::boolalpha << "is_const: " << decltype(hc)::clbl_is_const << "; is_volatile: " << decltype(hc)::clbl_is_volatile << std::endl;
        std::cout << std::boolalpha << "is_const: " << decltype(hv)::clbl_is_const << "; is_volatile: " << decltype(hv)::clbl_is_volatile << std::endl;
        std::cout << std::boolalpha << "is_const: " << decltype(hcv)::clbl_is_const << "; is_volatile: " << decltype(hcv)::clbl_is_volatile << std::endl;

        std::cout << hnormal(1, 'c') << std::endl;
        std::cout << hc(1, 'c') << std::endl;
        std::cout << hv(1, 'c') << std::endl;
        std::cout << hcv(1, 'c') << std::endl;

        auto stdn = convert_to<std::function>(hnormal);
        auto stdc = convert_to<std::function>(hc);
        auto stdv = convert_to<std::function>(hv);
        auto stdcv = convert_to<std::function>(hcv);

        std::cout << stdn(1, 'c') << std::endl;
        std::cout << stdc(1, 'c') << std::endl;
        std::cout << stdv(1, 'c') << std::endl;
        std::cout << stdcv(1, 'c') << std::endl;
        TEST(stdn(1, 'c') == test_id::overloaded_int_char_struct_op);
        TEST(stdc(1, 'c') == test_id::overloaded_int_char_struct_op_c);
        TEST(stdv(1, 'c') == test_id::overloaded_int_char_struct_op_v);
        TEST(stdcv(1, 'c') == test_id::overloaded_int_char_struct_op_cv);

        std::cout << int_char_std_function_result(stdn) << std::endl;
        std::cout << int_char_std_function_result(stdc) << std::endl;
        std::cout << int_char_std_function_result(stdv) << std::endl;
        std::cout << int_char_std_function_result(stdcv) << std::endl;

        TEST(int_char_std_function_result(stdn) == test_id::overloaded_int_char_struct_op);
        TEST(int_char_std_function_result(stdc) == test_id::overloaded_int_char_struct_op_c);
        TEST(int_char_std_function_result(stdv) == test_id::overloaded_int_char_struct_op_v);
        TEST(int_char_std_function_result(stdcv) == test_id::overloaded_int_char_struct_op_cv);
    }
    {
        //testing cv-correctness of nested clbl callbles converted to std::function

        auto overloaded_object = overloaded_int_char_struct{};

        
        auto normal = func(&overloaded_object);
        auto c = func(&overloaded_object);
        auto v = func(&overloaded_object);
        auto cv = func(&overloaded_object);

        std::cout << std::boolalpha << "is_const: " << decltype(normal)::clbl_is_const << "; is_volatile: " << decltype(normal)::clbl_is_volatile << std::endl;
        std::cout << std::boolalpha << "is_const: " << decltype(c)::clbl_is_const << "; is_volatile: " << decltype(c)::clbl_is_volatile << std::endl;
        std::cout << std::boolalpha << "is_const: " << decltype(v)::clbl_is_const << "; is_volatile: " << decltype(v)::clbl_is_volatile << std::endl;
        std::cout << std::boolalpha << "is_const: " << decltype(cv)::clbl_is_const << "; is_volatile: " << decltype(cv)::clbl_is_volatile << std::endl;

        auto nested_normal = harden<const char*(int, char)>(func(&normal));
        auto nested_c = harden<const char*(int, char) const>(func(&c));
        auto nested_v = harden<const char*(int, char) volatile>(func(&v));
        auto nested_cv = harden<const char*(int, char) const volatile>(func(&cv));

        std::cout << std::boolalpha << "is_const: " << decltype(nested_normal)::clbl_is_const << "; is_volatile: " << decltype(nested_normal)::clbl_is_volatile << std::endl;
        std::cout << std::boolalpha << "is_const: " << decltype(nested_c)::clbl_is_const << "; is_volatile: " << decltype(nested_c)::clbl_is_volatile << std::endl;
        std::cout << std::boolalpha << "is_const: " << decltype(nested_v)::clbl_is_const << "; is_volatile: " << decltype(nested_v)::clbl_is_volatile << std::endl;
        std::cout << std::boolalpha << "is_const: " << decltype(nested_cv)::clbl_is_const << "; is_volatile: " << decltype(nested_cv)::clbl_is_volatile << std::endl;

        auto stdn = convert_to<std::function>(nested_normal);
        auto stdc = convert_to<std::function>(nested_c);
        auto stdv = convert_to<std::function>(nested_v);
        auto stdcv = convert_to<std::function>(nested_cv);

        std::cout << std::endl << stdn(1, 'c') << std::endl;
        std::cout << stdc(1, 'c') << std::endl;
        std::cout << stdv(1, 'c') << std::endl;
        std::cout << stdcv(1, 'c') << std::endl;
        TEST(stdn(1, 'c') == test_id::overloaded_int_char_struct_op);
        TEST(stdc(1, 'c') == test_id::overloaded_int_char_struct_op_c);
        TEST(stdv(1, 'c') == test_id::overloaded_int_char_struct_op_v);
        TEST(stdcv(1, 'c') == test_id::overloaded_int_char_struct_op_cv);

        std::cout << int_char_std_function_result(stdn) << std::endl;
        std::cout << int_char_std_function_result(stdc) << std::endl;
        std::cout << int_char_std_function_result(stdv) << std::endl;
        std::cout << int_char_std_function_result(stdcv) << std::endl;
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

        std::cout << std::boolalpha << "is_const: " << decltype(normal)::clbl_is_const << "; is_volatile: " << decltype(normal)::clbl_is_volatile << std::endl;
        std::cout << std::boolalpha << "is_const: " << decltype(c)::clbl_is_const << "; is_volatile: " << decltype(c)::clbl_is_volatile << std::endl;
        std::cout << std::boolalpha << "is_const: " << decltype(v)::clbl_is_const << "; is_volatile: " << decltype(v)::clbl_is_volatile << std::endl;
        std::cout << std::boolalpha << "is_const: " << decltype(cv)::clbl_is_const << "; is_volatile: " << decltype(cv)::clbl_is_volatile << std::endl;

        auto hnormal = harden<const char*(int, char)>(normal);
        const auto hc = harden<const char*(int, char) const>(c);
        volatile auto hv = harden<const char*(int, char) volatile>(v);
        const volatile auto hcv = harden<const char*(int, char) const volatile>(cv);

        std::cout << std::boolalpha << "is_const: " << decltype(hnormal)::clbl_is_const << "; is_volatile: " << decltype(hnormal)::clbl_is_volatile << std::endl;
        std::cout << std::boolalpha << "is_const: " << decltype(hc)::clbl_is_const << "; is_volatile: " << decltype(hc)::clbl_is_volatile << std::endl;
        std::cout << std::boolalpha << "is_const: " << decltype(hv)::clbl_is_const << "; is_volatile: " << decltype(hv)::clbl_is_volatile << std::endl;
        std::cout << std::boolalpha << "is_const: " << decltype(hcv)::clbl_is_const << "; is_volatile: " << decltype(hcv)::clbl_is_volatile << std::endl;

        std::cout << hnormal(1, 'c') << std::endl;
        std::cout << hc(1, 'c') << std::endl;
        std::cout << hv(1, 'c') << std::endl;
        std::cout << hcv(1, 'c') << std::endl;

        auto stdn = convert_to<std::function>(hnormal);
        auto stdc = convert_to<std::function>(hc);
        auto stdv = convert_to<std::function>(hv);
        auto stdcv = convert_to<std::function>(hcv);

        std::cout << stdn(1, 'c') << std::endl;
        std::cout << stdc(1, 'c') << std::endl;
        std::cout << stdv(1, 'c') << std::endl;
        std::cout << stdcv(1, 'c') << std::endl;
        TEST(stdn(1, 'c') == test_id::overloaded_int_char_struct_op);
        TEST(stdc(1, 'c') == test_id::overloaded_int_char_struct_op_c);
        TEST(stdv(1, 'c') == test_id::overloaded_int_char_struct_op_v);
        TEST(stdcv(1, 'c') == test_id::overloaded_int_char_struct_op_cv);

        std::cout << int_char_std_function_result(stdn) << std::endl;
        std::cout << int_char_std_function_result(stdc) << std::endl;
        std::cout << int_char_std_function_result(stdv) << std::endl;
        std::cout << int_char_std_function_result(stdcv) << std::endl;

        TEST(int_char_std_function_result(stdn) == test_id::overloaded_int_char_struct_op);
        TEST(int_char_std_function_result(stdc) == test_id::overloaded_int_char_struct_op_c);
        TEST(int_char_std_function_result(stdv) == test_id::overloaded_int_char_struct_op_v);
        TEST(int_char_std_function_result(stdcv) == test_id::overloaded_int_char_struct_op_cv);
    }

#endif
}