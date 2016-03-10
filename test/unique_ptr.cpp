/*

Copyright Barrett Adair 2015
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)

*/

#include <sstream>
#include <memory>
#include <cassert>

#include <CLBL/clbl.hpp>
#include <quali/quali_macros.hpp>

#ifndef CLBL_RUNTIME_ASSERT
#undef NDEBUG
#define CLBL_RUNTIME_ASSERT(...) assert(__VA_ARGS__)
#endif //#ifndef CLBL_RUNTIME_ASSERT

using namespace clbl;

class scope_test {

private:
    bool& was_destroyed;

public:

    void increment(int& i) { ++i; }
    void operator()(int& i) { increment(i); }

    scope_test(bool& wd)
        : was_destroyed(wd) {}

    ~scope_test() {
        was_destroyed = true;
    }
};

int main() {


    {
        bool was_destroyed = false;
        int i = 0;

        {
            auto ptr = std::make_unique<scope_test>(was_destroyed);
            auto f = fwrap(&scope_test::increment, std::move(ptr));
            f(i);
        }

        CLBL_RUNTIME_ASSERT(i == 1);
        CLBL_RUNTIME_ASSERT(was_destroyed);
    }

    {
        bool was_destroyed = false;
        int i = 0;
       
        {
            auto ptr = std::make_unique<scope_test>(was_destroyed);
            auto f = fwrap(&scope_test::increment, std::move(ptr));
            f(i);

            auto g = std::move(f);
            CLBL_RUNTIME_ASSERT(!was_destroyed);
            g(i);
        }

        CLBL_RUNTIME_ASSERT(i == 2);
        CLBL_RUNTIME_ASSERT(was_destroyed);
    }
    {
        bool was_destroyed = false;
        int i = 0;

        {
            auto ptr = std::make_unique<scope_test>(was_destroyed);
            auto f = fwrap(std::move(ptr));
            f(i);
        }

        CLBL_RUNTIME_ASSERT(i == 1);
        CLBL_RUNTIME_ASSERT(was_destroyed);
    }

    {
        bool was_destroyed = false;
        int i = 0;

        {
            auto ptr = std::make_unique<scope_test>(was_destroyed);
            auto f = fwrap(std::move(ptr));
            f(i);

            auto g = std::move(f);
            CLBL_RUNTIME_ASSERT(!was_destroyed);
            g(i);
        }

        CLBL_RUNTIME_ASSERT(i == 2);
        CLBL_RUNTIME_ASSERT(was_destroyed);
    }

    return 0;
}