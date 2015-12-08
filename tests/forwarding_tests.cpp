#include "test.h"
#include "CLBL/func.h"
#include "int_char_definitions.h"

#include <iostream>
#include <functional>

using namespace clbl::tests;
using namespace clbl;

namespace fwd_tests {

    struct copy_counter {

        //increment value on every copy
        static int value;
        static void reset() { value = 0; }

        copy_counter() = default;
        copy_counter(const copy_counter&) { ++value; }
        copy_counter(copy_counter&) { ++value; }
        copy_counter(copy_counter&&) = default;
    };

    /*
    calling the take_copy function will increment
    the copy_counter::value. This will allow us to
    confirm that arguments passing through CLBL are
    not copied superfluously
    */
    void take_copy(copy_counter) {}
    
    /*
    we will use this to test something that forwards what 
    we're forwarding
    */
    auto forwarder = [](auto&& t) {return take_copy(t);};

    struct copy_taker{
        void take_copy(copy_counter) {}
    };
}

int fwd_tests::copy_counter::value = 0;

//todo test for RVO
void forwarding_tests() {

#ifdef CLBL_FORWARDING_TESTS
    std::cout << "running CLBL_FORWARDING_TESTS" << std::endl;

    using copy_counter = fwd_tests::copy_counter;

    {
        copy_counter::reset();

        auto f = func(&fwd_tests::take_copy);
        copy_counter obj{};

        f(obj);

        TEST(copy_counter::value == 1);
    }

    {
        copy_counter::reset();

        auto f = func(&fwd_tests::forwarder);
        copy_counter obj{};
        f(obj);

        TEST(copy_counter::value == 1);
    }
    {
        copy_counter::reset();

        auto f = func(fwd_tests::forwarder);
        copy_counter obj{};
        f(obj);

        TEST(copy_counter::value == 1);
    }
    {
        copy_counter::reset();

        auto f = func(std::ref(fwd_tests::forwarder));
        copy_counter obj{};
        f(obj);

        TEST(copy_counter::value == 1);
    }
    {
        copy_counter::reset();
        copy_counter obj{};

        auto f = func(&fwd_tests::take_copy);
        auto hardened = harden<void(copy_counter)>(f);

        hardened(obj);
        TEST(copy_counter::value == 1);

        auto std_func = convert_to<std::function>(hardened);

        std_func(obj);
        TEST(copy_counter::value == 2);
    }
    {
        copy_counter::reset();
        copy_counter obj{};

        auto f = func(fwd_tests::forwarder);
        auto hardened = harden<void(copy_counter)>(f);

        hardened(obj);
        TEST(copy_counter::value == 1);

        auto std_func = convert_to<std::function>(hardened);

        std_func(obj);
        TEST(copy_counter::value == 2);
    }
    {
        copy_counter::reset();
        copy_counter obj{};

        auto std_func = convert_to<std::function>(harden<void(copy_counter)>(func(fwd_tests::forwarder)));

        std_func(obj);
        TEST(copy_counter::value == 1);
    }
    {
        copy_counter::reset();
        copy_counter obj{};

        auto f = func(fwd_tests::copy_taker{}, &fwd_tests::copy_taker::take_copy);
        auto hardened = harden<void(copy_counter)>(f);

        hardened(obj);
        TEST(copy_counter::value == 1);

        auto std_func = convert_to<std::function>(hardened);

        std_func(obj);
        TEST(copy_counter::value == 2);
    }

#endif
}