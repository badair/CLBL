#include "CLBL/func.h"
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

void reference_tests() {

#ifdef CLBL_REFERENCE_ARG_TESTS
    std::cout << "running CLBL_REFERENCE_ARG_TESTS" << std::endl;

    using mutable_struct = ref_tests::mutable_struct;

    {
        //argument forwarding - non-const reference
        auto f = func(&ref_tests::increment_int);
        auto i = 0;
        f(i);
        TEST(i == 1);

        auto g = func(&f);
        g(i);
        TEST(i == 2);
    }
    {
        //argument forwarding - const reference
        auto f = func(&ref_tests::address_of_int);
        auto i = 0;
        TEST(f(i) == &i);

        auto g = func(&f);
        TEST(g(i) == &i);
    }
    {
        //argument forwarding - returning address of member of object reference arg
        auto f = func(&ref_tests::address_of_mutable_struct_value);
        auto o = mutable_struct{ 0 };

        TEST(f(o) == &o.value);
    }
    {
        //testing object reference preservation
        {
            auto mutable_object = mutable_struct{ 0 };
            auto f = func(&mutable_object, &mutable_struct::increment);

            f();
            f();
            f();

            TEST(mutable_object.value == 3);
        }
        {
            auto mutable_object_ptr = std::make_shared<mutable_struct>(0);
            auto f = func(mutable_object_ptr, &mutable_struct::increment);

            f();
            f();
            f();

            TEST(mutable_object_ptr->value == 3);
        }
    }

#endif
}

