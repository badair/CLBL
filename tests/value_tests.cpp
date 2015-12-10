#include "CLBL/clbl.h"
#include "test.h"

#include <memory>
#include <iostream>

using namespace clbl::tests;
using namespace clbl;

namespace val_tests {

    template<typename T>
    void increment_T(T& i) {
        ++i;
    }
}

void value_tests() {

#ifdef CLBL_VALUE_TESTS
    std::cout << "running CLBL_VALUE_TESTS" << std::endl;

    {
        //decays to function pointer
        auto f = fwrap([](int& i) {++i;});
        auto i = 0;
        f(i);
        TEST(i == 1);
    }
    {
        //passing ref-capturing lambda by value
        auto i = 0;
        auto f = fwrap([&i]{++i;});
        
        f();
        TEST(i == 1);
    }
    {
        //passing by reference wrapper
        auto i = 0;
        auto lam = [&i]{++i;};
        auto f = fwrap(std::ref(lam));

        f();
        TEST(i == 1);
    }
    {
        //passing generic lambda by value
        auto i = 0;
        auto f = fwrap([](auto&& i) {val_tests::increment_T(i);});

        f(i);
        TEST(i == 1);
    }
    {
        //passing generic lambda object by reference wrapper
        auto i = 0;
        auto lam = [](auto&& i) {val_tests::increment_T(i);};
        auto f = fwrap(std::ref(lam));

        f(i);
        TEST(i == 1);
    }
    
#endif
}

