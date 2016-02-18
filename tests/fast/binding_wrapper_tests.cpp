/*

Copyright Barrett Adair 2016
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)

*/

#include <cassert>
#include <type_traits>
#include <functional>
#include <iostream>
#include <sstream>
#include <memory>
#include <CLBL/clbl.h>

#undef NDEBUG

using namespace std::placeholders;

std::string to_string(int i, std::string& s, double x, const char* c) {
    auto ss = std::stringstream{};
    ss << i << ", " << s << ", " << x << ", " << c;
    return ss.str();
}

struct foo {
    std::string to_string(int i, std::string& s, double x, const char* c) {
        return ::to_string(i, s, x, c);
    }
};

auto bar = [](int i, std::string& s, double x, const char* c) {
    return ::to_string(i, s, x, c);
};

struct quum {
    std::string operator()(int i, std::string& s, double x, const char* c) {
        return ::to_string(i, s, x, c);
    }
};

struct ugly {
    std::string operator()(char, std::string& s, char, int i, char, const char* c, char, double x) {
        return ::to_string(i, s, x, c);
    }
};

#define BIND_TEST(...) \
{\
auto x = __VA_ARGS__.bind(1, clbl::ph<2>{}, _1, "const char*"); \
static_assert(std::is_same<decltype(x)::arg_types, std::tuple<double, std::string&> >::value, "bad args"); \
auto s = std::string{"string"}; \
assert(x(1.7, s) == "1, string, 1.7, const char*"); \
}\
/**/

int main() {

    BIND_TEST(clbl::fwrap(to_string))
    BIND_TEST(clbl::fwrap(&to_string))
    BIND_TEST(clbl::fwrap<decltype(to_string), to_string>())
    BIND_TEST(clbl::fwrap<decltype(&to_string), &to_string>())
    BIND_TEST(clbl::fwrap(bar))
    BIND_TEST(clbl::fwrap(&bar))
    BIND_TEST(clbl::fwrap(std::make_shared<quum>()))
    BIND_TEST(clbl::fwrap(foo{}, &foo::to_string))
    BIND_TEST(clbl::fwrap(std::make_shared<foo>(), &foo::to_string))
    BIND_TEST(clbl::fwrap<decltype(&foo::to_string), &foo::to_string>().bind(std::make_shared<foo>(), _1, _2, _3, _4))
    BIND_TEST(clbl::fwrap<decltype(&foo::to_string), &foo::to_string>(std::make_shared<foo>()))
    BIND_TEST(clbl::fwrap<decltype(&foo::to_string), &foo::to_string>(foo{}))
    BIND_TEST(clbl::fwrap(ugly{}).bind('u', _2, 'g', _1, 'l', _4, 'y', _3))

    return 0;
}