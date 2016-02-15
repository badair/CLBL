/*

Copyright Barrett Adair 2016
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)

*/

#include <cassert>
#include <type_traits>
#include <functional>
#include <iostream>
#include <chrono>
#include <sstream>
#include <CLBL/clbl.h>

using namespace std::placeholders;

constexpr auto BASE = "BASELINE";
constexpr auto STD_INVOKE = "STD::INVOKE";
constexpr auto STD_BIND = "STD::BIND";
constexpr auto STD_FUNCTION = "STD::FUNCTION";
constexpr auto CLBL = "CLBL";

auto benchmark = [](auto name, auto f) {
    auto j = 0;
    auto start = std::chrono::steady_clock::now();
    f(j);
    auto end = std::chrono::steady_clock::now();
    auto elapsed = std::chrono::duration<double, std::milli>(end - start);
    std::cout << name << ": " << elapsed.count() << " ms" << std::endl;
};

auto finish_benchmarks_category = []() {
    std::cout << std::endl << std::endl;
};

namespace lambda {

    auto add_to = [](auto& a, auto b) { a = a + b; };

    auto run_benchmarks = [](auto iterations) {

        std::cout << "LAMBDA BENCHMARK" << std::endl << std::endl;

        benchmark(BASE, [&](auto& j) {
            const auto f = add_to;
            for (int i = 0; i < iterations; ++i) f(j, 1);
        });

        benchmark(CLBL, [&](auto& j) {
            const auto f = clbl::fwrap(add_to);
            for (int i = 0; i < iterations; ++i) f(j, 1);
        });

        benchmark(STD_INVOKE, [&](auto& j) {
            for (int i = 0; i < iterations; ++i) std::invoke(add_to, j, 1);
        });

        benchmark(STD_FUNCTION, [&](auto& j) {
            const auto f = std::function<void(int&, int)>{ add_to };
            for (int i = 0; i < iterations; ++i) f(j, 1);
        });

        benchmark(STD_BIND, [&](auto& j) {
            const auto f = std::bind(add_to, _1, _2);
            for (int i = 0; i < iterations; ++i) f(j, 1);
        });

        finish_benchmarks_category();
    };
}

namespace function {

    void add_to(int& a, int b) {
        a = a + b;
    }

    auto run_benchmarks = [](auto iterations) {

        std::cout << "FUNCTION BENCHMARK" << std::endl << std::endl;

        benchmark(BASE, [&](auto& j) {
            for (int i = 0; i < iterations; ++i) add_to(j, 1);
        });

        benchmark(CLBL, [&](auto& j) {
            const auto f = clbl::fwrap(add_to);
            for (int i = 0; i < iterations; ++i) f(j, 1);
        });

        /* MSVC doesn't like this
        benchmark(CLBL, [&](auto& j) {
            const auto f = clbl::fwrap<decltype(add_to), add_to>();
            for (int i = 0; i < iterations; ++i) f(j, 1);
        });*/

        benchmark(STD_INVOKE, [&](auto& j) {
            for (int i = 0; i < iterations; ++i) std::invoke(add_to, j, 1);
        });

        benchmark(STD_FUNCTION, [&](auto& j) {
            const auto f = std::function<void(int&, int)>{ add_to };
            for (int i = 0; i < iterations; ++i) f(j, 1);
        });

        benchmark(STD_BIND, [&](auto& j) {
            const auto f = std::bind(add_to, _1, _2);
            for (int i = 0; i < iterations; ++i) f(j, 1);
        });

        finish_benchmarks_category();
    };
}

namespace member_function {

    struct foo {
        void add_to(int& a, int b) const volatile {
            a = a + b;
        }
    };

    //volatile removes some optimizations

    auto obj = foo{};
    constexpr auto add_to = &foo::add_to;

    auto run_benchmarks = [](auto iterations) {

        std::cout << "MEMBER FUNCTION BENCHMARK" << std::endl << std::endl;

        benchmark(BASE, [&](auto& j) {
            for (int i = 0; i < iterations; ++i) obj.add_to(j, 1);
        });

        benchmark(CLBL, [&](auto& j) {
            const auto f = clbl::fwrap(obj, add_to);
            for (int i = 0; i < iterations; ++i) 
                f(j, 1);
        });

        benchmark(CLBL, [&](auto& j) {
            const auto f = clbl::fwrap<decltype(&foo::add_to), &foo::add_to>();
            for (int i = 0; i < iterations; ++i) f(obj, j, 1);
        });

        benchmark(CLBL, [&](auto& j) {
            const auto f = clbl::fwrap<decltype(&foo::add_to), &foo::add_to>(obj);
            for (int i = 0; i < iterations; ++i) f(j, 1);
        });

        benchmark(CLBL, [&](auto& j) {
            const auto f = clbl::fwrap<decltype(&foo::add_to), &foo::add_to>(&obj);
            for (int i = 0; i < iterations; ++i) f(j, 1);
        });

        benchmark(CLBL, [&](auto& j) {
            const auto f = clbl::fwrap<decltype(&foo::add_to), &foo::add_to>(std::ref(obj));
            for (int i = 0; i < iterations; ++i) f(j, 1);
        });

        benchmark(STD_INVOKE, [&](auto& j) {
            for (int i = 0; i < iterations; ++i) std::invoke(add_to, obj, j, 1);
        });

        /*?? benchmark(STD_FUNCTION, [&](auto& j) {
        const auto f = std::function<void(int&, int)>{ add_to };
        for (int i = 0; i < iterations; ++i) f(j, 1);
        });*/

        benchmark(STD_BIND, [&](auto& j) {
            const auto f = std::bind(add_to, obj, _1, _2);
            for (int i = 0; i < iterations; ++i) f(j, 1);
        });

        finish_benchmarks_category();
    };
}

auto get_iterations = [] {

    //preventing const propagation optimizations

    auto ss = std::stringstream{};
    auto iterations = std::numeric_limits<int>::max() / 1000;

    ss << iterations;
    ss >> iterations;

    std::cout << "iterations per case: " << iterations << std::endl << std::endl;

    return iterations;
};

int main() {

    auto iterations = get_iterations();

    lambda::run_benchmarks(iterations);
    function::run_benchmarks(iterations);
    member_function::run_benchmarks(iterations);

    std::cout << std::endl << "Finished.";
    std::cin.get();

    return 0;
}