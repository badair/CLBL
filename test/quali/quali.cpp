/*!
@copyright Barrett Adair 2015
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)

*/

#include <quali/quali_macros.hpp>

#ifndef QUALI_STATIC_ASSERT
#define QUALI_STATIC_ASSERT(...) static_assert(__VA_ARGS__, #__VA_ARGS__)
#endif

#ifndef QUALI_ASSERT_IS_SAME
#define QUALI_ASSERT_IS_SAME(...) QUALI_STATIC_ASSERT(std::is_same<__VA_ARGS__>{})
#endif

using namespace quali;

int main() {

    {
        //default
        using T = int;
        using F = QUALI_INTC(QUALI_DEFAULT);
        QUALI_ASSERT_IS_SAME(T, qualify<T, default_>);
        QUALI_ASSERT_IS_SAME(T, T QUALI_TOKENS(QUALI_DEFAULT));
        QUALI_STATIC_ASSERT(F{} == QUALI_FLAGS(QUALI_DEFAULT));
        QUALI_STATIC_ASSERT(F{} == default_);
        QUALI_STATIC_ASSERT(F{} == (default_ | default_));
        QUALI_STATIC_ASSERT(F{} == remove_reference<F{}>{});
        QUALI_STATIC_ASSERT(F{} == remove_const<F{}>{});
        QUALI_STATIC_ASSERT(F{} == remove_volatile<F{}>{});
        QUALI_STATIC_ASSERT(F{} == remove_cv<F{}>{});
        QUALI_STATIC_ASSERT(F{} == cv_of<T>{});
        QUALI_STATIC_ASSERT(F{} == ref_of<T>{});
    } {
        //const
        using T = const int;
        using F = QUALI_INTC(const);
        QUALI_ASSERT_IS_SAME(T, qualify<int, F{}>);
        QUALI_ASSERT_IS_SAME(T, QUALI_TOKENS(const) int);
        QUALI_STATIC_ASSERT(F{} == QUALI_FLAGS(const));
        QUALI_STATIC_ASSERT(F{} == const_);
        QUALI_STATIC_ASSERT(F{} == (const_ | default_));
        QUALI_STATIC_ASSERT(F{} == (const_ | const_));
        QUALI_STATIC_ASSERT(F{} == add_const<default_>{});
        QUALI_STATIC_ASSERT(F{} == add_const<F{}>{});
        QUALI_STATIC_ASSERT(F{} == cv_of<T>{});
        QUALI_STATIC_ASSERT(default_ == remove_const<F{}>{});
        QUALI_STATIC_ASSERT(default_ == ref_of<T>{});
    } {
        //volatile
        using T = volatile int;
        using F = QUALI_INTC(volatile);
        QUALI_ASSERT_IS_SAME(T, qualify<int, F{}>);
        QUALI_ASSERT_IS_SAME(T, QUALI_TOKENS(volatile) int);
        QUALI_STATIC_ASSERT(F{} == QUALI_FLAGS(volatile));
        QUALI_STATIC_ASSERT(F{} == volatile_);
        QUALI_STATIC_ASSERT(F{} == (volatile_ | default_));
        QUALI_STATIC_ASSERT(F{} == (volatile_ | volatile_));
        QUALI_STATIC_ASSERT(F{} == add_volatile<default_>{});
        QUALI_STATIC_ASSERT(F{} == add_volatile<F{}>{});
        QUALI_STATIC_ASSERT(F{} == cv_of<T>{});
        QUALI_STATIC_ASSERT(default_ == remove_volatile<default_>{});
        QUALI_STATIC_ASSERT(default_ == remove_volatile<F{}>{});
        QUALI_STATIC_ASSERT(default_ == ref_of<T>{});
    } {
        //const volatile
        using T = const volatile int;
        using F = QUALI_INTC(const, volatile);
        QUALI_ASSERT_IS_SAME(T, qualify<int, F{}>);
        QUALI_ASSERT_IS_SAME(T, QUALI_TOKENS(const, volatile) int);
        QUALI_STATIC_ASSERT(F{} == QUALI_FLAGS(const, volatile));
        QUALI_STATIC_ASSERT(F{} == (const_ | volatile_));
        QUALI_STATIC_ASSERT(F{} == add_const<volatile_>{});
        QUALI_STATIC_ASSERT(F{} == add_const<F{}>{});
        QUALI_STATIC_ASSERT(F{} == add_volatile<const_>{});
        QUALI_STATIC_ASSERT(F{} == add_volatile<F{}>{});
        QUALI_STATIC_ASSERT(F{} == add_cv<default_>{});
        QUALI_STATIC_ASSERT(F{} == add_cv<F{}>{});
        QUALI_STATIC_ASSERT(F{} == add_cv<volatile_>{});
        QUALI_STATIC_ASSERT(F{} == add_cv<const_>{});
        QUALI_STATIC_ASSERT(F{} == cv_of<T>{});
        QUALI_STATIC_ASSERT(const_ == remove_volatile<F{}>{});
        QUALI_STATIC_ASSERT(volatile_ == remove_const<F{}>{});
        QUALI_STATIC_ASSERT(default_ == ref_of<T>{});
        QUALI_STATIC_ASSERT(default_ == remove_cv<F{}>{});
        QUALI_STATIC_ASSERT(default_ == remove_cv<F{}>{});
        
    } {
        //&
        using T = int &;
        using F = QUALI_INTC(&);
        QUALI_ASSERT_IS_SAME(T, qualify<int, F{}>);
        QUALI_ASSERT_IS_SAME(T, int QUALI_TOKENS(&));
        QUALI_STATIC_ASSERT(F{} == QUALI_FLAGS(&));
        QUALI_STATIC_ASSERT(F{} == lref_);
        QUALI_STATIC_ASSERT(F{} == add_lvalue_reference<default_>{});
        QUALI_STATIC_ASSERT(F{} == add_lvalue_reference<F{}>{});
        QUALI_STATIC_ASSERT(F{} == ref_of<T>{});
        QUALI_STATIC_ASSERT(default_ == remove_reference<F{}>{});
        QUALI_STATIC_ASSERT(default_ == cv_of<T>{});
    } {
        //&&
        using T = int &&;
        using F = QUALI_INTC(&&);
        QUALI_ASSERT_IS_SAME(T, qualify<int, F{}>);
        QUALI_ASSERT_IS_SAME(T, int QUALI_TOKENS(&&));
        QUALI_STATIC_ASSERT(F{} == QUALI_FLAGS(&&));
        QUALI_STATIC_ASSERT(F{} == rref_);
        QUALI_STATIC_ASSERT(F{} == add_rvalue_reference<default_>{});
        QUALI_STATIC_ASSERT(F{} == add_rvalue_reference<F{}>{});
        QUALI_STATIC_ASSERT(F{} == ref_of<T>{});
        QUALI_STATIC_ASSERT(default_ == remove_reference<F{}>{});
        QUALI_STATIC_ASSERT(default_ == cv_of<T>{});
    } {
        //const &
        using T = const int &;
        using F = QUALI_INTC(const, &);
        QUALI_ASSERT_IS_SAME(T, qualify<int, F{}>);
        QUALI_ASSERT_IS_SAME(T, int QUALI_TOKENS(const, &));
        QUALI_STATIC_ASSERT(F{} == QUALI_FLAGS(const, &));
        QUALI_STATIC_ASSERT(F{} == (const_ | lref_));
        QUALI_STATIC_ASSERT(F{} == add_rvalue_reference<F{}>{});
        QUALI_STATIC_ASSERT(F{} == add_lvalue_reference <F{}> {});
        QUALI_STATIC_ASSERT(const_ == remove_reference<F{}>{});
        QUALI_STATIC_ASSERT(cv_of<T>{} == const_);
        QUALI_STATIC_ASSERT(ref_of<T>{} == QUALI_FLAGS(&));
    }

    return 0;
}