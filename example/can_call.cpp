/*

Copyright Barrett Adair 2015
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)

*/

#include <CLBL/clbl.hpp>


decltype(auto) A(int* i) { return *i; }

auto B = [](int* i) {return *i;};

auto C = [](auto&& i) -> decltype(*i) {return *i; };

struct D {
    decltype(auto) deref(int* t) { return *t; }
};

struct E {
    auto& operator()(int* t) { return *t; }
};

struct F {
    template <typename T>
    auto operator()(T&& t) -> decltype(*t) { return *t; }
};

struct G {
    template <typename T, std::enable_if_t<std::is_pointer<T>{}, int> = 0>
    decltype(auto) operator()(T&& t) { return *t; }
};

// H is incompatible with can_call - deduced return type is incompatible with
// expression SFINAE when the return expression is ill-formed. For more details, see
// http://stackoverflow.com/questions/17608637/sfinae-and-decltypeauto/17608907#17608907
struct H {
    template <typename T>
    decltype(auto) operator()(T&& t) {
        return *t;
    }
};

int main() {

    auto i = 0;

    // decltyping the integral_constant result here because gcc (or libstdc++?) doesn't 
    // like the constexpr bool conversion
    {
        auto c = clbl::fwrap(A);
        static_assert(decltype(c.can_call(&i)){}, "");
        static_assert(!decltype(c.can_call(i)){}, "");
    } {
        auto c = clbl::fwrap(B);
        static_assert(decltype(c.can_call(&i)){}, "");
        static_assert(!decltype(c.can_call(i)){}, "");
    } {
        auto c = clbl::fwrap(C);
        static_assert(decltype(c.can_call(&i)){}, "");
        static_assert(!decltype(c.can_call(i)){}, "");
    } {
        auto c = clbl::fwrap(&D::deref);
        static_assert(decltype(c.can_call(D{}, &i)){}, "");
        static_assert(!decltype(c.can_call(D{}, i)){}, "");
    } /* todo implement {
      auto c = clbl::fwrap(&D::deref).bind(D{});
      static_assert(decltype(c.can_call(&i)){}, "");
      static_assert(decltype(!c.can_call(i)){}, "");
      }*/ {
          auto c = clbl::fwrap(&D::deref, D{});
          static_assert(decltype(c.can_call(&i)){}, "");
          static_assert(!decltype(c.can_call(i)){}, "");
    } {
        auto c = clbl::fwrap(F{});
        static_assert(decltype(c.can_call(&i)){}, "");
        static_assert(!decltype(c.can_call(i)){}, "");
    } {
        auto c = clbl::fwrap(G{});
        static_assert(decltype(c.can_call(&i)){}, "");
        static_assert(!decltype(c.can_call(i)){}, "");
    } {
        auto c = clbl::fwrap(H{});
        static_assert(decltype(c.can_call(&i)){}, "");
        //fails to compile - see comment on H
        //static_assert(decltype(!c.can_call(i)){}, "");
    }

    return 0;
}