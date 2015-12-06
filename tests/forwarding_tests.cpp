#include "test.h"
#include "CLBL/func.h"
#include "int_char_definitions.h"

#include <iostream>
#include <functional>

using namespace clbl::tests;
using namespace clbl;

namespace fwd_tests {

    struct copy_counter {

        static int value;

        copy_counter() = default;

        copy_counter(const copy_counter&) {
            ++value;
        }

        copy_counter(copy_counter&) {
            ++value;
        }

        copy_counter(copy_counter&&) = default;
    };

    void take_copy(copy_counter) {

    }

    auto t_forwarder = [](auto&& t) {return take_copy(t);};
}

int fwd_tests::copy_counter::value = 0;

void forwarding_tests() {

#ifdef CLBL_FORWARDING_TESTS
    std::cout << "running CLBL_FORWARDING_TESTS" << std::endl;

    using copy_counter = fwd_tests::copy_counter;

    {
        copy_counter::value = 0;

        auto f = func(&fwd_tests::take_copy);
        copy_counter obj{};

        f(obj);

        TEST(copy_counter::value == 1);
    }

    {
        copy_counter::value = 0;

        auto f = func(&fwd_tests::t_forwarder);
        copy_counter obj{};
        f(obj);

        TEST(copy_counter::value == 1);
    }
    {
        copy_counter::value = 0;

        auto f = func(fwd_tests::t_forwarder);
        copy_counter obj{};
        f(obj);

        TEST(copy_counter::value == 1);
    }
    {
        copy_counter::value = 0;

        auto f = func(std::ref(fwd_tests::t_forwarder));
        copy_counter obj{};
        f(obj);

        TEST(copy_counter::value == 1);
    }

#endif
}