#include <CLBL/clbl.h>
#include "test.h"

#include <memory>
#include <iostream>

using namespace clbl::tests;
using namespace clbl;

namespace ref_tests {

    struct mutable_struct {
        int value;
        mutable_struct(int i) : value(i) {}

        void increment() {
            ++value;
        }
    };

    const int* address_of_mutable_struct_value(mutable_struct& o)
    {
        return &o.value;
    }

    void increment_int(int& i) {
        ++i;
    }

    const int* address_of_int(const int& i) {
        return &i;
    }
}

void reference_arg_tests() {

#ifdef CLBL_REFERENCE_ARG_TESTS
    std::cout << "running CLBL_REFERENCE_ARG_TESTS" << std::endl;

    using mutable_struct = ref_tests::mutable_struct;

    {
        //argument forwarding - non-const reference
        auto f = fwrap(&ref_tests::increment_int);
        auto i = 0;
        f(i);
        TEST(i == 1);

        auto g = fwrap(&f);
        g(i);
        TEST(i == 2);
    }
    {
        auto f = fwrap(&ref_tests::increment_int);
        auto stdf = convert_to<std::function>(f);
        auto i = 0;
        stdf(i);
        TEST(i == 1);

        auto g = fwrap(&f);
        auto stdg = convert_to<std::function>(harden<typename decltype(f)::type>(g));
        stdg(i);
        TEST(i == 2);
    }
    {
        //argument forwarding - const reference
        auto f = fwrap(&ref_tests::address_of_int);
        auto i = 0;
        TEST(f(i) == &i);

        auto g = fwrap(&f);
        TEST(g(i) == &i);
    }
    {
        //argument forwarding - returning address of member of object reference arg
        auto f = fwrap(&ref_tests::address_of_mutable_struct_value);
        auto o = mutable_struct{ 0 };

        TEST(f(o) == &o.value);
    }
    {
        //testing object reference preservation
        {
            auto mutable_object = mutable_struct{ 0 };
            auto f = fwrap(&mutable_object, &mutable_struct::increment);

            f();
            f();
            f();

            TEST(mutable_object.value == 3);
        }
        {
            auto mutable_object_ptr = std::make_shared<mutable_struct>(0);
            auto f = fwrap(mutable_object_ptr, &mutable_struct::increment);

            f();
            f();
            f();

            TEST(mutable_object_ptr->value == 3);
        }
    }
    {
        auto lambda_wrapper = fwrap([](int& i) {++i;});
        static_assert(std::is_same<decltype(lambda_wrapper)::type, void(int&)>::value, "");
        int i = 0;
        lambda_wrapper(i);
        TEST(i == 1);
        auto stdfunction_lambda_wrapper = convert_to<std::function>(lambda_wrapper);
        stdfunction_lambda_wrapper(i);
        TEST(i == 2);

        //both of these should cause compiler errors
        //lambda_wrapper(1);
        //stdfunction_lambda_wrapper(1);
    }
    {
        auto lambda_wrapper = fwrap([](int&& i) {return i + 1;});
        static_assert(std::is_same<decltype(lambda_wrapper)::type, int(int&&)>::value, "");

        auto stdfunction_lambda_wrapper = convert_to<std::function>(lambda_wrapper);
        static_assert(std::is_same<decltype(stdfunction_lambda_wrapper), 
                                   std::function<int(forward<int&&>)> >::value, "");
        
        //these should cause compiler errors
        //int i = 0;
        //lambda_wrapper(i);
        //stdfunction_lambda_wrapper(i);

        TEST(lambda_wrapper(0) == 1);
        TEST(stdfunction_lambda_wrapper(0) == 1);
        
        int&& j = 0;

        TEST([](int&& i) {return i + 1;}(static_cast<int&&>(j)) == 1);
        TEST(lambda_wrapper(static_cast<int&&>(j)) == 1);
        TEST(stdfunction_lambda_wrapper(static_cast<int&&>(j)) == 1);
    }

#endif
}

