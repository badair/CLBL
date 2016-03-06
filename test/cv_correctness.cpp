/*

Copyright Barrett Adair 2015
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)

*/

#include <functional>
#include <iostream>
#include <iostream>

#include <CLBL/clbl.hpp>
#include <quali/quali_macros.hpp>

using namespace clbl;

#ifndef CLBL_STATIC_ASSERT
#undef NDEBUG
#define CLBL_STATIC_ASSERT(...) static_assert(__VA_ARGS__, #__VA_ARGS__)
#endif //#ifndef CLBL_STATIC_ASSERT

struct foo {

    QUALI_INTC(QUALI_DEFAULT)   operator()(int, char) { return{}; }
    QUALI_INTC(const)           operator()(int, char) const { return{}; }
    QUALI_INTC(volatile)        operator()(int, char) volatile { return{}; }
    QUALI_INTC(const, volatile) operator()(int, char) const volatile { return{}; }

    foo() = default;

    template<typename T>
    foo(T&&) {}
};

template<typename T>
auto call_std_func(std::function<T> f) {
    return f(1, 'c');
}

int main() {

    {
        //testing cv-correctness of clbl callables converted to std::function
        auto overloaded_object = foo{};

        auto normal = fwrap(&overloaded_object);
        auto c = fwrap(&overloaded_object);
        auto v = fwrap(&overloaded_object);
        auto cv = fwrap(&overloaded_object);

        auto hnormal = harden<auto_(int, char)>(normal);
        auto hc = harden<auto_(int, char) const>(c);
        auto hv = harden<auto_(int, char) volatile>(v);
        auto hcv = harden<auto_(int, char) const volatile>(cv);

        CLBL_STATIC_ASSERT(decltype(hnormal(1, 'c')){} == QUALI_FLAGS(QUALI_DEFAULT));
        CLBL_STATIC_ASSERT(decltype(hc(1, 'c')){} == QUALI_FLAGS(const));
        CLBL_STATIC_ASSERT(decltype(hv(1, 'c')){} == QUALI_FLAGS(volatile));
        CLBL_STATIC_ASSERT(decltype(hcv(1, 'c')){} == QUALI_FLAGS(const, volatile));

        auto stdn = convert_to<std::function>(hnormal);
        auto stdc = convert_to<std::function>(hc);
        auto stdv = convert_to<std::function>(hv);
        auto stdcv = convert_to<std::function>(hcv);

        CLBL_STATIC_ASSERT(decltype(stdn(1, 'c')){} == QUALI_FLAGS(QUALI_DEFAULT));
        CLBL_STATIC_ASSERT(decltype(stdc(1, 'c')){} == QUALI_FLAGS(const));
        CLBL_STATIC_ASSERT(decltype(stdv(1, 'c')){} == QUALI_FLAGS(volatile));
        CLBL_STATIC_ASSERT(decltype(stdcv(1, 'c')){} == QUALI_FLAGS(const, volatile));

        CLBL_STATIC_ASSERT(decltype(call_std_func(stdn)){} == QUALI_FLAGS(QUALI_DEFAULT));
        CLBL_STATIC_ASSERT(decltype(call_std_func(stdc)){} == QUALI_FLAGS(const));
        CLBL_STATIC_ASSERT(decltype(call_std_func(stdv)){} == QUALI_FLAGS(volatile));
        CLBL_STATIC_ASSERT(decltype(call_std_func(stdcv)){} == QUALI_FLAGS(const, volatile));

        auto rehardened = harden<auto_(int, char) const>(hnormal);
        CLBL_STATIC_ASSERT(decltype(rehardened(1, 'c')){} == QUALI_FLAGS(const));

        auto rehardened_again = harden<auto_(int, char) const volatile>(rehardened);
        CLBL_STATIC_ASSERT(decltype(rehardened_again(1, 'c')){} == QUALI_FLAGS(const, volatile));

        auto rehardened_v = harden<auto_(int, char) volatile>(hnormal);
        CLBL_STATIC_ASSERT(decltype(rehardened_v(1, 'c')){} == QUALI_FLAGS(volatile));

        auto rehardened_again_v = harden<auto_(int, char) const volatile>(rehardened_v);
        CLBL_STATIC_ASSERT(decltype(rehardened_again_v(1, 'c')){} == QUALI_FLAGS(const, volatile));
    }
    {
        auto overloaded_object = foo{};

        auto normal = fwrap(&overloaded_object);
        auto c = fwrap(&overloaded_object);
        auto v = fwrap(&overloaded_object);
        auto cv = fwrap(&overloaded_object);

        /*CLBL_PRINT_NAME_AND_TYPE(normal);
        CLBL_PRINT_NAME_AND_TYPE(c);
        CLBL_PRINT_NAME_AND_TYPE(v);
        CLBL_PRINT_NAME_AND_TYPE(cv);*/

        auto nested_normal = fwrap(&normal);
        auto nested_c = fwrap(&c);
        auto nested_v = fwrap(&v);
        auto nested_cv = fwrap(&cv);

        /*CLBL_PRINT_NAME_AND_TYPE(nested_normal);
        CLBL_PRINT_NAME_AND_TYPE(nested_c);
        CLBL_PRINT_NAME_AND_TYPE(nested_v);
        CLBL_PRINT_NAME_AND_TYPE(nested_cv);*/

        CLBL_STATIC_ASSERT(decltype(nested_normal(1, 'c')){} == QUALI_FLAGS(QUALI_DEFAULT));
        CLBL_STATIC_ASSERT(decltype(nested_c(1, 'c')){} == QUALI_FLAGS(QUALI_DEFAULT));
        CLBL_STATIC_ASSERT(decltype(nested_v(1, 'c')){} == QUALI_FLAGS(QUALI_DEFAULT));
        CLBL_STATIC_ASSERT(decltype(nested_cv(1, 'c')){} == QUALI_FLAGS(QUALI_DEFAULT));

        auto hnormal = harden<auto_(int, char)>(nested_normal);
        auto hc = harden<auto_(int, char) const>(nested_c);
        auto hv = harden<auto_(int, char) volatile>(nested_v);
        auto hcv = harden<auto_(int, char) const volatile>(nested_cv);

        /*CLBL_PRINT_NAME_AND_TYPE(hnormal);
        CLBL_PRINT_NAME_AND_TYPE(hc);
        CLBL_PRINT_NAME_AND_TYPE(hv);
        CLBL_PRINT_NAME_AND_TYPE(hcv);*/

        CLBL_STATIC_ASSERT(decltype(hnormal(1, 'c')){} == QUALI_FLAGS(QUALI_DEFAULT));
        CLBL_STATIC_ASSERT(decltype(hc(1, 'c')){} == QUALI_FLAGS(const));
        CLBL_STATIC_ASSERT(decltype(hv(1, 'c')){} == QUALI_FLAGS(volatile));
        CLBL_STATIC_ASSERT(decltype(hcv(1, 'c')){} == QUALI_FLAGS(const, volatile));

        auto stdn = convert_to<std::function>(hnormal);
        auto stdc = convert_to<std::function>(hc);
        auto stdv = convert_to<std::function>(hv);
        auto stdcv = convert_to<std::function>(hcv);

        /*CLBL_PRINT_NAME_AND_TYPE(stdn);
        CLBL_PRINT_NAME_AND_TYPE(stdc);
        CLBL_PRINT_NAME_AND_TYPE(stdv);
        CLBL_PRINT_NAME_AND_TYPE(stdcv);*/

        CLBL_STATIC_ASSERT(decltype(stdn(1, 'c')){} == QUALI_FLAGS(QUALI_DEFAULT));
        CLBL_STATIC_ASSERT(decltype(stdc(1, 'c')){} == QUALI_FLAGS(const));
        CLBL_STATIC_ASSERT(decltype(stdv(1, 'c')){} == QUALI_FLAGS(volatile));
        CLBL_STATIC_ASSERT(decltype(stdcv(1, 'c')){} == QUALI_FLAGS(const, volatile));

        CLBL_STATIC_ASSERT(decltype(call_std_func(stdn)){} == QUALI_FLAGS(QUALI_DEFAULT));
        CLBL_STATIC_ASSERT(decltype(call_std_func(stdc)){} == QUALI_FLAGS(const));
        CLBL_STATIC_ASSERT(decltype(call_std_func(stdv)){} == QUALI_FLAGS(volatile));
        CLBL_STATIC_ASSERT(decltype(call_std_func(stdcv)){} == QUALI_FLAGS(const, volatile));

        auto rehardened = harden<auto_(int, char) const>(hnormal);
        CLBL_STATIC_ASSERT(decltype(rehardened(1, 'c')){} == QUALI_FLAGS(const));

        auto rehardened_again = harden<auto_(int, char) const volatile>(rehardened);
        CLBL_STATIC_ASSERT(decltype(rehardened_again(1, 'c')){} == QUALI_FLAGS(const, volatile));

        auto rehardened_v = harden<auto_(int, char) volatile>(hnormal);
        CLBL_STATIC_ASSERT(decltype(rehardened_v(1, 'c')){} == QUALI_FLAGS(volatile));

        auto rehardened_again_v = harden<auto_(int, char) const volatile>(rehardened_v);
        CLBL_STATIC_ASSERT(decltype(rehardened_again_v(1, 'c')){} == QUALI_FLAGS(const, volatile));
    }
    {
        auto overloaded_object = foo{};

        auto normal = fwrap(&overloaded_object);
        auto c = fwrap(&overloaded_object);
        auto v = fwrap(&overloaded_object);
        auto cv = fwrap(&overloaded_object);

        /*CLBL_PRINT_NAME_AND_TYPE(normal);
        CLBL_PRINT_NAME_AND_TYPE(c);
        CLBL_PRINT_NAME_AND_TYPE(v);
        CLBL_PRINT_NAME_AND_TYPE(cv);*/

        auto nested_normal_intermediate = fwrap(&normal);
        auto nested_c_intermediate = fwrap(&c);
        auto nested_v_intermediate = fwrap(&v);
        auto nested_cv_intermediate = fwrap(&cv);

        auto nested_normal = fwrap(&nested_normal_intermediate);
        auto nested_c = fwrap(&nested_c_intermediate);
        auto nested_v = fwrap(&nested_v_intermediate);
        auto nested_cv = fwrap(&nested_cv_intermediate);

        /*CLBL_PRINT_NAME_AND_TYPE(nested_normal);
        CLBL_PRINT_NAME_AND_TYPE(nested_c);
        CLBL_PRINT_NAME_AND_TYPE(nested_v);
        CLBL_PRINT_NAME_AND_TYPE(nested_cv);*/

        CLBL_STATIC_ASSERT(decltype(nested_normal(1, 'c')){} == QUALI_FLAGS(QUALI_DEFAULT));
        CLBL_STATIC_ASSERT(decltype(nested_c(1, 'c')){} == QUALI_FLAGS(QUALI_DEFAULT));
        CLBL_STATIC_ASSERT(decltype(nested_v(1, 'c')){} == QUALI_FLAGS(QUALI_DEFAULT));
        CLBL_STATIC_ASSERT(decltype(nested_cv(1, 'c')){} == QUALI_FLAGS(QUALI_DEFAULT));

        auto hnormal = harden<auto_(int, char)>(nested_normal);
        auto hc = harden<auto_(int, char) const>(nested_c);
        auto hv = harden<auto_(int, char) volatile>(nested_v);
        auto hcv = harden<auto_(int, char) const volatile>(nested_cv);

        /*CLBL_PRINT_NAME_AND_TYPE(hnormal);
        CLBL_PRINT_NAME_AND_TYPE(hc);
        CLBL_PRINT_NAME_AND_TYPE(hv);
        CLBL_PRINT_NAME_AND_TYPE(hcv);*/

        CLBL_STATIC_ASSERT(decltype(hnormal(1, 'c')){} == QUALI_FLAGS(QUALI_DEFAULT));
        CLBL_STATIC_ASSERT(decltype(hc(1, 'c')){} == QUALI_FLAGS(const));
        CLBL_STATIC_ASSERT(decltype(hv(1, 'c')){} == QUALI_FLAGS(volatile));
        CLBL_STATIC_ASSERT(decltype(hcv(1, 'c')){} == QUALI_FLAGS(const, volatile));

        auto stdn = convert_to<std::function>(hnormal);
        auto stdc = convert_to<std::function>(hc);
        auto stdv = convert_to<std::function>(hv);
        auto stdcv = convert_to<std::function>(hcv);

        /*CLBL_PRINT_NAME_AND_TYPE(stdn);
        CLBL_PRINT_NAME_AND_TYPE(stdc);
        CLBL_PRINT_NAME_AND_TYPE(stdv);
        CLBL_PRINT_NAME_AND_TYPE(stdcv);*/

        CLBL_STATIC_ASSERT(decltype(stdn(1, 'c')){} == QUALI_FLAGS(QUALI_DEFAULT));
        CLBL_STATIC_ASSERT(decltype(stdc(1, 'c')){} == QUALI_FLAGS(const));
        CLBL_STATIC_ASSERT(decltype(stdv(1, 'c')){} == QUALI_FLAGS(volatile));
        CLBL_STATIC_ASSERT(decltype(stdcv(1, 'c')){} == QUALI_FLAGS(const, volatile));

        CLBL_STATIC_ASSERT(decltype(call_std_func(stdn)){} == QUALI_FLAGS(QUALI_DEFAULT));
        CLBL_STATIC_ASSERT(decltype(call_std_func(stdc)){} == QUALI_FLAGS(const));
        CLBL_STATIC_ASSERT(decltype(call_std_func(stdv)){} == QUALI_FLAGS(volatile));
        CLBL_STATIC_ASSERT(decltype(call_std_func(stdcv)){} == QUALI_FLAGS(const, volatile));

        auto rehardened = harden<auto_(int, char) const>(hnormal);
        CLBL_STATIC_ASSERT(decltype(rehardened(1, 'c')){} == QUALI_FLAGS(const));

        auto rehardened_again = harden<auto_(int, char) const volatile>(rehardened);
        CLBL_STATIC_ASSERT(decltype(rehardened_again(1, 'c')){} == QUALI_FLAGS(const, volatile));

        auto rehardened_v = harden<auto_(int, char) volatile>(hnormal);
        CLBL_STATIC_ASSERT(decltype(rehardened_v(1, 'c')){} == QUALI_FLAGS(volatile));

        auto rehardened_again_v = harden<auto_(int, char) const volatile>(rehardened_v);
        CLBL_STATIC_ASSERT(decltype(rehardened_again_v(1, 'c')){} == QUALI_FLAGS(const, volatile));
    }
    {
        auto overloaded_object = foo{};

        auto normal = fwrap(&overloaded_object);
        const auto c = fwrap(&overloaded_object);
        volatile auto v = fwrap(&overloaded_object);
        const volatile auto cv = fwrap(&overloaded_object);

        auto hnormal = harden<auto_(int, char)>(normal);
        auto hc = harden<auto_(int, char) const>(c);
        auto hv = harden<auto_(int, char) volatile>(v);
        auto hcv = harden<auto_(int, char) const volatile>(cv);

        auto stdn = convert_to<std::function>(hnormal);
        auto stdc = convert_to<std::function>(hc);
        auto stdv = convert_to<std::function>(hv);
        auto stdcv = convert_to<std::function>(hcv);

        CLBL_STATIC_ASSERT(std::is_same<
            decltype(stdn),
            std::function<QUALI_INTC(QUALI_DEFAULT)(forward<int>, forward<char>) >
        >::value);

        CLBL_STATIC_ASSERT(decltype(stdn(1, 'c')){} == QUALI_FLAGS(QUALI_DEFAULT));
        CLBL_STATIC_ASSERT(decltype(stdc(1, 'c')){} == QUALI_FLAGS(const));
        CLBL_STATIC_ASSERT(decltype(stdv(1, 'c')){} == QUALI_FLAGS(volatile));
        CLBL_STATIC_ASSERT(decltype(stdcv(1, 'c')){} == QUALI_FLAGS(const, volatile));

        CLBL_STATIC_ASSERT(decltype(call_std_func(stdn)){} == QUALI_FLAGS(QUALI_DEFAULT));
        CLBL_STATIC_ASSERT(decltype(call_std_func(stdc)){} == QUALI_FLAGS(const));
        CLBL_STATIC_ASSERT(decltype(call_std_func(stdv)){} == QUALI_FLAGS(volatile));
        CLBL_STATIC_ASSERT(decltype(call_std_func(stdcv)){} == QUALI_FLAGS(const, volatile));

        auto rehardened = harden<auto_(int, char) const>(hnormal);
        CLBL_STATIC_ASSERT(decltype(rehardened(1, 'c')){} == QUALI_FLAGS(const));

        auto rehardened_again = harden<auto_(int, char) const volatile>(rehardened);
        CLBL_STATIC_ASSERT(decltype(rehardened_again(1, 'c')){} == QUALI_FLAGS(const, volatile));

        auto rehardened_v = harden<auto_(int, char) volatile>(hnormal);
        CLBL_STATIC_ASSERT(decltype(rehardened_v(1, 'c')){} == QUALI_FLAGS(volatile));

        auto rehardened_again_v = harden<auto_(int, char) const volatile>(rehardened_v);
        CLBL_STATIC_ASSERT(decltype(rehardened_again_v(1, 'c')){} == QUALI_FLAGS(const, volatile));
    }
    {
        const auto overloaded_object = foo{};

        //auto normal = fwrap(&overloaded_object);
        const auto c = fwrap(&overloaded_object);
        //volatile auto v = fwrap(&overloaded_object);
        const volatile auto cv = fwrap(&overloaded_object);

        //should cause compile error if called: auto hnormal = harden<auto_(int, char)>(normal);
        const auto hc = harden<auto_(int, char) const>(c);
        //should cause compile error if called: volatile auto hv = harden<auto_(int, char) volatile>(v);
        const volatile auto hcv = harden<auto_(int, char) const volatile>(cv);

        auto stdc = convert_to<std::function>(hc);
        auto stdcv = convert_to<std::function>(hcv);

        CLBL_STATIC_ASSERT(decltype(stdc(1, 'c')){} == QUALI_FLAGS(const));
        CLBL_STATIC_ASSERT(decltype(stdcv(1, 'c')){} == QUALI_FLAGS(const, volatile));

        CLBL_STATIC_ASSERT(decltype(call_std_func(stdc)){} == QUALI_FLAGS(const));
        CLBL_STATIC_ASSERT(decltype(call_std_func(stdcv)){} == QUALI_FLAGS(const, volatile));
    }
    {
        volatile auto overloaded_object = foo{};

        //auto normal = fwrap(&overloaded_object);
        //const auto c = fwrap(&overloaded_object);
        volatile auto v = fwrap(&overloaded_object);
        const volatile auto cv = fwrap(&overloaded_object);

        //should cause compile error if called: auto hnormal = harden<auto_(int, char)>(normal);
        //should cause compile error if called: const auto hc = harden<auto_(int, char) const>(c);
        volatile auto hv = harden<auto_(int, char) volatile>(v);
        const volatile auto hcv = harden<auto_(int, char) const volatile>(cv);

        auto stdv = convert_to<std::function>(hv);
        auto stdcv = convert_to<std::function>(hcv);

        CLBL_STATIC_ASSERT(decltype(stdv(1, 'c')){} == QUALI_FLAGS(volatile));
        CLBL_STATIC_ASSERT(decltype(stdcv(1, 'c')){} == QUALI_FLAGS(const, volatile));

        CLBL_STATIC_ASSERT(decltype(call_std_func(stdv)){} == QUALI_FLAGS(volatile));
        CLBL_STATIC_ASSERT(decltype(call_std_func(stdcv)){} == QUALI_FLAGS(const, volatile));
    }
    {
        //testing cv-correctness of clbl callables converted to std::function
        auto overloaded_object = foo{};

        auto normal = fwrap(overloaded_object);
        auto c = fwrap(overloaded_object);
        auto v = fwrap(overloaded_object);
        auto cv = fwrap(overloaded_object);

        auto hnormal = harden<auto_(int, char)>(normal);
        auto hc = harden<auto_(int, char) const>(c);
        auto hv = harden<auto_(int, char) volatile>(v);
        auto hcv = harden<auto_(int, char) const volatile>(cv);

        CLBL_STATIC_ASSERT(decltype(hnormal(1, 'c')){} == QUALI_FLAGS(QUALI_DEFAULT));
        CLBL_STATIC_ASSERT(decltype(hc(1, 'c')){} == QUALI_FLAGS(const));
        CLBL_STATIC_ASSERT(decltype(hv(1, 'c')){} == QUALI_FLAGS(volatile));
        CLBL_STATIC_ASSERT(decltype(hcv(1, 'c')){} == QUALI_FLAGS(const, volatile));

        auto stdn = convert_to<std::function>(hnormal);
        auto stdc = convert_to<std::function>(hc);
        auto stdv = convert_to<std::function>(hv);
        auto stdcv = convert_to<std::function>(hcv);

        CLBL_STATIC_ASSERT(decltype(stdn(1, 'c')){} == QUALI_FLAGS(QUALI_DEFAULT));
        CLBL_STATIC_ASSERT(decltype(stdc(1, 'c')){} == QUALI_FLAGS(const));
        CLBL_STATIC_ASSERT(decltype(stdv(1, 'c')){} == QUALI_FLAGS(volatile));
        CLBL_STATIC_ASSERT(decltype(stdcv(1, 'c')){} == QUALI_FLAGS(const, volatile));

        CLBL_STATIC_ASSERT(decltype(call_std_func(stdn)){} == QUALI_FLAGS(QUALI_DEFAULT));
        CLBL_STATIC_ASSERT(decltype(call_std_func(stdc)){} == QUALI_FLAGS(const));
        CLBL_STATIC_ASSERT(decltype(call_std_func(stdv)){} == QUALI_FLAGS(volatile));
        CLBL_STATIC_ASSERT(decltype(call_std_func(stdcv)){} == QUALI_FLAGS(const, volatile));

        auto rehardened = harden<auto_(int, char) const>(hnormal);
        CLBL_STATIC_ASSERT(decltype(rehardened(1, 'c')){} == QUALI_FLAGS(const));

        auto rehardened_again = harden<auto_(int, char) const volatile>(rehardened);
        CLBL_STATIC_ASSERT(decltype(rehardened_again(1, 'c')){} == QUALI_FLAGS(const, volatile));

        auto rehardened_v = harden<auto_(int, char) volatile>(hnormal);
        CLBL_STATIC_ASSERT(decltype(rehardened_v(1, 'c')){} == QUALI_FLAGS(volatile));

        auto rehardened_again_v = harden<auto_(int, char) const volatile>(rehardened_v);
        CLBL_STATIC_ASSERT(decltype(rehardened_again_v(1, 'c')){} == QUALI_FLAGS(const, volatile));
    }
    {
        auto overloaded_object = foo{};

        auto normal = fwrap(overloaded_object);
        auto c = fwrap(overloaded_object);
        auto v = fwrap(overloaded_object);
        auto cv = fwrap(overloaded_object);

        /*CLBL_PRINT_NAME_AND_TYPE(normal);
        CLBL_PRINT_NAME_AND_TYPE(c);
        CLBL_PRINT_NAME_AND_TYPE(v);
        CLBL_PRINT_NAME_AND_TYPE(cv);*/

        auto nested_normal = fwrap(normal);
        auto nested_c = fwrap(c);
        auto nested_v = fwrap(v);
        auto nested_cv = fwrap(cv);

        /*CLBL_PRINT_NAME_AND_TYPE(nested_normal);
        CLBL_PRINT_NAME_AND_TYPE(nested_c);
        CLBL_PRINT_NAME_AND_TYPE(nested_v);
        CLBL_PRINT_NAME_AND_TYPE(nested_cv);*/

        CLBL_STATIC_ASSERT(decltype(nested_normal(1, 'c')){} == QUALI_FLAGS(QUALI_DEFAULT));
        CLBL_STATIC_ASSERT(decltype(nested_c(1, 'c')){} == QUALI_FLAGS(QUALI_DEFAULT));
        CLBL_STATIC_ASSERT(decltype(nested_v(1, 'c')){} == QUALI_FLAGS(QUALI_DEFAULT));
        CLBL_STATIC_ASSERT(decltype(nested_cv(1, 'c')){} == QUALI_FLAGS(QUALI_DEFAULT));

        auto hnormal = harden<auto_(int, char)>(nested_normal);
        auto hc = harden<auto_(int, char) const>(nested_c);
        auto hv = harden<auto_(int, char) volatile>(nested_v);
        auto hcv = harden<auto_(int, char) const volatile>(nested_cv);

        /*CLBL_PRINT_NAME_AND_TYPE(hnormal);
        CLBL_PRINT_NAME_AND_TYPE(hc);
        CLBL_PRINT_NAME_AND_TYPE(hv);
        CLBL_PRINT_NAME_AND_TYPE(hcv);*/

        CLBL_STATIC_ASSERT(decltype(hnormal(1, 'c')){} == QUALI_FLAGS(QUALI_DEFAULT));
        CLBL_STATIC_ASSERT(decltype(hc(1, 'c')){} == QUALI_FLAGS(const));
        CLBL_STATIC_ASSERT(decltype(hv(1, 'c')){} == QUALI_FLAGS(volatile));
        CLBL_STATIC_ASSERT(decltype(hcv(1, 'c')){} == QUALI_FLAGS(const, volatile));

        auto stdn = convert_to<std::function>(hnormal);
        auto stdc = convert_to<std::function>(hc);
        auto stdv = convert_to<std::function>(hv);
        auto stdcv = convert_to<std::function>(hcv);

        /*CLBL_PRINT_NAME_AND_TYPE(stdn);
        CLBL_PRINT_NAME_AND_TYPE(stdc);
        CLBL_PRINT_NAME_AND_TYPE(stdv);
        CLBL_PRINT_NAME_AND_TYPE(stdcv);*/

        CLBL_STATIC_ASSERT(decltype(stdn(1, 'c')){} == QUALI_FLAGS(QUALI_DEFAULT));
        CLBL_STATIC_ASSERT(decltype(stdc(1, 'c')){} == QUALI_FLAGS(const));
        CLBL_STATIC_ASSERT(decltype(stdv(1, 'c')){} == QUALI_FLAGS(volatile));
        CLBL_STATIC_ASSERT(decltype(stdcv(1, 'c')){} == QUALI_FLAGS(const, volatile));

        CLBL_STATIC_ASSERT(decltype(call_std_func(stdn)){} == QUALI_FLAGS(QUALI_DEFAULT));
        CLBL_STATIC_ASSERT(decltype(call_std_func(stdc)){} == QUALI_FLAGS(const));
        CLBL_STATIC_ASSERT(decltype(call_std_func(stdv)){} == QUALI_FLAGS(volatile));
        CLBL_STATIC_ASSERT(decltype(call_std_func(stdcv)){} == QUALI_FLAGS(const, volatile));

        auto rehardened = harden<auto_(int, char) const>(hnormal);
        CLBL_STATIC_ASSERT(decltype(rehardened(1, 'c')){} == QUALI_FLAGS(const));

        auto rehardened_again = harden<auto_(int, char) const volatile>(rehardened);
        CLBL_STATIC_ASSERT(decltype(rehardened_again(1, 'c')){} == QUALI_FLAGS(const, volatile));

        auto rehardened_v = harden<auto_(int, char) volatile>(hnormal);
        CLBL_STATIC_ASSERT(decltype(rehardened_v(1, 'c')){} == QUALI_FLAGS(volatile));

        auto rehardened_again_v = harden<auto_(int, char) const volatile>(rehardened_v);
        CLBL_STATIC_ASSERT(decltype(rehardened_again_v(1, 'c')){} == QUALI_FLAGS(const, volatile));
    }
    {
        auto overloaded_object = foo{};

        auto normal = fwrap(overloaded_object);
        auto c = fwrap(overloaded_object);
        auto v = fwrap(overloaded_object);
        auto cv = fwrap(overloaded_object);

        /*CLBL_PRINT_NAME_AND_TYPE(normal);
        CLBL_PRINT_NAME_AND_TYPE(c);
        CLBL_PRINT_NAME_AND_TYPE(v);
        CLBL_PRINT_NAME_AND_TYPE(cv);*/

        auto nested_normal_intermediate = fwrap(normal);
        auto nested_c_intermediate = fwrap(c);
        auto nested_v_intermediate = fwrap(v);
        auto nested_cv_intermediate = fwrap(cv);

        auto nested_normal = fwrap(nested_normal_intermediate);
        auto nested_c = fwrap(nested_c_intermediate);
        auto nested_v = fwrap(nested_v_intermediate);
        auto nested_cv = fwrap(nested_cv_intermediate);

        /*CLBL_PRINT_NAME_AND_TYPE(nested_normal);
        CLBL_PRINT_NAME_AND_TYPE(nested_c);
        CLBL_PRINT_NAME_AND_TYPE(nested_v);
        CLBL_PRINT_NAME_AND_TYPE(nested_cv);*/

        CLBL_STATIC_ASSERT(decltype(nested_normal(1, 'c')){} == QUALI_FLAGS(QUALI_DEFAULT));
        CLBL_STATIC_ASSERT(decltype(nested_c(1, 'c')){} == QUALI_FLAGS(QUALI_DEFAULT));
        CLBL_STATIC_ASSERT(decltype(nested_v(1, 'c')){} == QUALI_FLAGS(QUALI_DEFAULT));
        CLBL_STATIC_ASSERT(decltype(nested_cv(1, 'c')){} == QUALI_FLAGS(QUALI_DEFAULT));

        auto hnormal = harden<auto_(int, char)>(nested_normal);
        auto hc = harden<auto_(int, char) const>(nested_c);
        auto hv = harden<auto_(int, char) volatile>(nested_v);
        auto hcv = harden<auto_(int, char) const volatile>(nested_cv);

        /*CLBL_PRINT_NAME_AND_TYPE(hnormal);
        CLBL_PRINT_NAME_AND_TYPE(hc);
        CLBL_PRINT_NAME_AND_TYPE(hv);
        CLBL_PRINT_NAME_AND_TYPE(hcv);*/

        CLBL_STATIC_ASSERT(decltype(hnormal(1, 'c')){} == QUALI_FLAGS(QUALI_DEFAULT));
        CLBL_STATIC_ASSERT(decltype(hc(1, 'c')){} == QUALI_FLAGS(const));
        CLBL_STATIC_ASSERT(decltype(hv(1, 'c')){} == QUALI_FLAGS(volatile));
        CLBL_STATIC_ASSERT(decltype(hcv(1, 'c')){} == QUALI_FLAGS(const, volatile));

        auto stdn = convert_to<std::function>(hnormal);
        auto stdc = convert_to<std::function>(hc);
        auto stdv = convert_to<std::function>(hv);
        auto stdcv = convert_to<std::function>(hcv);

        /*CLBL_PRINT_NAME_AND_TYPE(stdn);
        CLBL_PRINT_NAME_AND_TYPE(stdc);
        CLBL_PRINT_NAME_AND_TYPE(stdv);
        CLBL_PRINT_NAME_AND_TYPE(stdcv);*/

        CLBL_STATIC_ASSERT(decltype(stdn(1, 'c')){} == QUALI_FLAGS(QUALI_DEFAULT));
        CLBL_STATIC_ASSERT(decltype(stdc(1, 'c')){} == QUALI_FLAGS(const));
        CLBL_STATIC_ASSERT(decltype(stdv(1, 'c')){} == QUALI_FLAGS(volatile));
        CLBL_STATIC_ASSERT(decltype(stdcv(1, 'c')){} == QUALI_FLAGS(const, volatile));

        CLBL_STATIC_ASSERT(decltype(call_std_func(stdn)){} == QUALI_FLAGS(QUALI_DEFAULT));
        CLBL_STATIC_ASSERT(decltype(call_std_func(stdc)){} == QUALI_FLAGS(const));
        CLBL_STATIC_ASSERT(decltype(call_std_func(stdv)){} == QUALI_FLAGS(volatile));
        CLBL_STATIC_ASSERT(decltype(call_std_func(stdcv)){} == QUALI_FLAGS(const, volatile));

        auto rehardened = harden<auto_(int, char) const>(hnormal);
        CLBL_STATIC_ASSERT(decltype(rehardened(1, 'c')){} == QUALI_FLAGS(const));

        auto rehardened_again = harden<auto_(int, char) const volatile>(rehardened);
        CLBL_STATIC_ASSERT(decltype(rehardened_again(1, 'c')){} == QUALI_FLAGS(const, volatile));

        auto rehardened_v = harden<auto_(int, char) volatile>(hnormal);
        CLBL_STATIC_ASSERT(decltype(rehardened_v(1, 'c')){} == QUALI_FLAGS(volatile));

        auto rehardened_again_v = harden<auto_(int, char) const volatile>(rehardened_v);
        CLBL_STATIC_ASSERT(decltype(rehardened_again_v(1, 'c')){} == QUALI_FLAGS(const, volatile));
    }
    {
        auto overloaded_object = foo{};

        auto normal = fwrap(overloaded_object);
        const auto c = fwrap(overloaded_object);
        volatile auto v = fwrap(overloaded_object);
        const volatile auto cv = fwrap(overloaded_object);

        auto hnormal = harden<auto_(int, char)>(normal);
        auto hc = harden<auto_(int, char) const>(c);
        auto hv = harden<auto_(int, char) volatile>(v);
        auto hcv = harden<auto_(int, char) const volatile>(cv);

        auto stdn = convert_to<std::function>(hnormal);
        auto stdc = convert_to<std::function>(hc);
        auto stdv = convert_to<std::function>(hv);
        auto stdcv = convert_to<std::function>(hcv);

        CLBL_STATIC_ASSERT(std::is_same<
            decltype(stdn),
            std::function<QUALI_INTC(QUALI_DEFAULT)(forward<int>, forward<char>) >
        >::value);

        CLBL_STATIC_ASSERT(decltype(stdn(1, 'c')){} == QUALI_FLAGS(QUALI_DEFAULT));
        CLBL_STATIC_ASSERT(decltype(stdc(1, 'c')){} == QUALI_FLAGS(const));
        CLBL_STATIC_ASSERT(decltype(stdv(1, 'c')){} == QUALI_FLAGS(volatile));
        CLBL_STATIC_ASSERT(decltype(stdcv(1, 'c')){} == QUALI_FLAGS(const, volatile));

        CLBL_STATIC_ASSERT(decltype(call_std_func(stdn)){} == QUALI_FLAGS(QUALI_DEFAULT));
        CLBL_STATIC_ASSERT(decltype(call_std_func(stdc)){} == QUALI_FLAGS(const));
        CLBL_STATIC_ASSERT(decltype(call_std_func(stdv)){} == QUALI_FLAGS(volatile));
        CLBL_STATIC_ASSERT(decltype(call_std_func(stdcv)){} == QUALI_FLAGS(const, volatile));

        auto rehardened = harden<auto_(int, char) const>(hnormal);
        CLBL_STATIC_ASSERT(decltype(rehardened(1, 'c')){} == QUALI_FLAGS(const));

        auto rehardened_again = harden<auto_(int, char) const volatile>(rehardened);
        CLBL_STATIC_ASSERT(decltype(rehardened_again(1, 'c')){} == QUALI_FLAGS(const, volatile));

        auto rehardened_v = harden<auto_(int, char) volatile>(hnormal);
        CLBL_STATIC_ASSERT(decltype(rehardened_v(1, 'c')){} == QUALI_FLAGS(volatile));

        auto rehardened_again_v = harden<auto_(int, char) const volatile>(rehardened_v);
        CLBL_STATIC_ASSERT(decltype(rehardened_again_v(1, 'c')){} == QUALI_FLAGS(const, volatile));
    }
    {
        const auto overloaded_object = foo{};

        //auto normal = fwrap(overloaded_object);
        const auto c = fwrap(overloaded_object);
        //volatile auto v = fwrap(overloaded_object);
        const volatile auto cv = fwrap(overloaded_object);

        //should cause compile error if called: auto hnormal = harden<auto_(int, char)>(normal);
        const auto hc = harden<auto_(int, char) const>(c);
        //should cause compile error if called: volatile auto hv = harden<auto_(int, char) volatile>(v);
        const volatile auto hcv = harden<auto_(int, char) const volatile>(cv);

        auto stdc = convert_to<std::function>(hc);
        auto stdcv = convert_to<std::function>(hcv);

        CLBL_STATIC_ASSERT(decltype(stdc(1, 'c')){} == QUALI_FLAGS(const));
        CLBL_STATIC_ASSERT(decltype(stdcv(1, 'c')){} == QUALI_FLAGS(const, volatile));

        CLBL_STATIC_ASSERT(decltype(call_std_func(stdc)){} == QUALI_FLAGS(const));
        CLBL_STATIC_ASSERT(decltype(call_std_func(stdcv)){} == QUALI_FLAGS(const, volatile));
    }
    {
        volatile auto overloaded_object = foo{};

        //auto normal = fwrap(&overloaded_object);
        //const auto c = fwrap(&overloaded_object);
        volatile auto v = fwrap(overloaded_object);
        const volatile auto cv = fwrap(overloaded_object);

        //should cause compile error if called: auto hnormal = harden<auto_(int, char)>(normal);
        //should cause compile error if called: const auto hc = harden<auto_(int, char) const>(c);
        volatile auto hv = harden<auto_(int, char) volatile>(v);
        const volatile auto hcv = harden<auto_(int, char) const volatile>(cv);

        auto stdv = convert_to<std::function>(hv);
        auto stdcv = convert_to<std::function>(hcv);

        CLBL_STATIC_ASSERT(decltype(stdv(1, 'c')){} == QUALI_FLAGS(volatile));
        CLBL_STATIC_ASSERT(decltype(stdcv(1, 'c')){} == QUALI_FLAGS(const, volatile));

        CLBL_STATIC_ASSERT(decltype(call_std_func(stdv)){} == QUALI_FLAGS(volatile));
        CLBL_STATIC_ASSERT(decltype(call_std_func(stdcv)){} == QUALI_FLAGS(const, volatile));
    }

    return 0;
}