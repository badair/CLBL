/*

Copyright Barrett Adair 2015
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)

*/

#include "test.hpp"

#include <memory>
#include <iostream>
#include <CLBL/clbl.hpp>

using namespace clbl::test;
using namespace clbl;

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

int main() {

#ifdef CLBL_REFERENCE_ARG_TESTS
    std::cout << "running CLBL_REFERENCE_ARG_TESTS" << std::endl;

    using mutable_struct = mutable_struct;

    {
        //argument forwarding - non-const reference
        auto f = fwrap(&increment_int);
        auto i = 0;
        f(i);
        assert(i == 1);

        auto g = fwrap(&f);
        g(i);
        assert(i == 2);
    }
    {
        auto f = fwrap(&increment_int);
        auto stdf = convert_to<std::function>(f);
        auto i = 0;
        stdf(i);
        assert(i == 1);

        auto g = fwrap(&f);
        auto stdg = convert_to<std::function>(harden<typename decltype(f)::type>(g));
        stdg(i);
        assert(i == 2);
    }
    {
        //argument forwarding - const reference
        auto f = fwrap(&address_of_int);
        auto i = 0;
        assert(f(i) == &i);

        auto g = fwrap(&f);
        assert(g(i) == &i);
    }
    {
        //argument forwarding - returning address of member of object reference arg
        auto f = fwrap(&address_of_mutable_struct_value);
        auto o = mutable_struct{ 0 };

        assert(f(o) == &o.value);
    }
    {
        //testing object reference preservation
        {
            auto mutable_object = mutable_struct{ 0 };
            auto f = fwrap(&mutable_struct::increment, &mutable_object);

            f();
            f();
            f();

            assert(mutable_object.value == 3);
        }
        {
            auto mutable_object_ptr = std::make_shared<mutable_struct>(0);
            auto f = fwrap(&mutable_struct::increment, mutable_object_ptr);

            f();
            f();
            f();

            assert(mutable_object_ptr->value == 3);
        }
    }
    {
        auto lambda_wrapper = fwrap([](int& i) {++i;});
        static_assert(std::is_same<decltype(lambda_wrapper)::function_type, void(int&)>::value, "");
        int i = 0;
        lambda_wrapper(i);
        assert(i == 1);
        auto stdfunction_lambda_wrapper = convert_to<std::function>(lambda_wrapper);
        stdfunction_lambda_wrapper(i);
        assert(i == 2);

        //both of these should cause compiler errors
        //lambda_wrapper(1);
        //stdfunction_lambda_wrapper(1);
    }
    {
        auto lambda_wrapper = fwrap([](int&& i) {return i + 1;});
        static_assert(std::is_same<decltype(lambda_wrapper)::function_type, int(int&&)>::value, "");

        auto stdfunction_lambda_wrapper = convert_to<std::function>(lambda_wrapper);
        static_assert(std::is_same<decltype(stdfunction_lambda_wrapper),
            std::function<int(forward<int&&>)> >::value, "");

        //these should cause compiler errors
        //int i = 0;
        //lambda_wrapper(i);
        //stdfunction_lambda_wrapper(i);

        assert(lambda_wrapper(0) == 1);
        assert(stdfunction_lambda_wrapper(0) == 1);

        int&& j = 0;

        assert([](int&& i) {return i + 1;}(static_cast<int&&>(j)) == 1);
        assert(lambda_wrapper(static_cast<int&&>(j)) == 1);
        assert(stdfunction_lambda_wrapper(static_cast<int&&>(j)) == 1);
    }

#endif

    return 0;
}

