/*

Copyright Barrett Adair 2015
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)

*/

#include <functional>
#include <iostream>
#include <iostream>
#include <cassert>

#include <CLBL/clbl.hpp>

#ifndef CLBL_RUNTIME_ASSERT
#undef NDEBUG
#define CLBL_RUNTIME_ASSERT(...) assert(__VA_ARGS__)
#endif //#ifndef CLBL_RUNTIME_ASSERT

#ifndef CLBL_STATIC_ASSERT
#define CLBL_STATIC_ASSERT(...) static_assert(__VA_ARGS__, #__VA_ARGS__)
#endif //#ifndef CLBL_STATIC_ASSERT

auto int_char_std_function_result(std::function<std::string(int, char)> f) {
    return f(1, 'c');
}

void call_std_function(std::function<void(int)> f) {
    f(0);
}

struct mutable_struct {
    int value;
    static int copy_count;

    mutable_struct(int i) : value(i) {}

    void increment(int) {
        ++value;
    }

    mutable_struct(mutable_struct&&) = default;
    mutable_struct(const mutable_struct&) { ++copy_count; }
    mutable_struct(mutable_struct&) { ++copy_count; }
};

int mutable_struct::copy_count = 0;

int main() {

    //testing object reference preservation when converted to std::function
    auto mutable_object = mutable_struct{ 0 };
    auto f = clbl::fwrap(&mutable_struct::increment, &mutable_object);
    auto stdf = clbl::convert_to<std::function>(f);

    f(0);
    stdf(0);
    call_std_function(stdf);

    CLBL_RUNTIME_ASSERT(mutable_object.value == 3);

    return 0;
}