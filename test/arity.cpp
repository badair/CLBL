/*

Copyright Barrett Adair 2015
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)

*/

#include <CLBL/clbl.hpp>

#ifndef CLBL_STATIC_ASSERT
#undef NDEBUG
#define CLBL_STATIC_ASSERT(...) static_assert(__VA_ARGS__, #__VA_ARGS__)
#endif //#ifndef CLBL_STATIC_ASSERT

struct A {};

struct B {
    void operator()(A s) {}
};

struct C {
    template<typename T>
    T&& operator()(T&& t) { return static_cast<T&&>(t); }
};

struct D {
    template<typename T>
    void operator()(T&& t) { return static_cast<T&&>(t); }

};

struct E {
    void operator()() { }
};

struct F {
    void operator()() { }
    void operator()(int, int, int) { }
};

struct G {
    template<typename... T>
    void operator()(T&&...) { }
};

struct H {
    int operator()(...) { return{}; }
};

struct I {
    int operator()(int = 1, int = 2, int = 3, int = 4) { return{}; }
};

struct J {
    //we can't determine the arity of constrained callables
    template<typename T, std::enable_if_t<std::is_same<T, int&&>::value, int> = 0>
    int operator()(T = 1, T = 2, T = 3, T = 4) { return{}; }
};

class non_default_constructble {
    non_default_constructble() = delete;
};

struct K {
    auto operator()(non_default_constructble n) { return n; }
};

struct L {
    template<typename T>
    decltype(auto) operator()(T&&, T&&, T&&) { return std::remove_reference_t<T>{}; }
};

struct M {
    template<typename T>
    decltype(auto) operator()(T&&) { return 1; }
};


struct N {

private:
     void operator()() {}
};

struct O {

protected:

    void operator()() {}
};

struct child : O {};


struct temp {
    void operator()(int = 0, const char* = "");
};


struct P;

template<typename CRTP>
struct crtp {};

struct P;

struct P : crtp <P> {

    void operator()(int = 0, const char* = "");

protected:

    void operator()(int, int, int);
};

struct Q {

    void operator()(int, int, int, int, int, int, int);

    friend struct P;

protected:
    void operator()(int, int, int);
};

struct R {
    void operator()(int, int, int, int, int, int);
};

struct S {
    void operator()(int, int, int, int, int, std::nullptr_t, int, int);
    void operator()(int, int, double, int, int, int, int, int, int, int, int);
};

struct T {
    void operator()(int, int, int, int, int, int, int, int);
    void operator()(int, int, int, int, int, int, int);
    void operator()(int, int, int, int, int, int);
};

int main() {
    CLBL_STATIC_ASSERT(clbl::arity<A>::max == -1);
    CLBL_STATIC_ASSERT(clbl::arity<A>::min == -1);

    CLBL_STATIC_ASSERT(clbl::arity<B>::max == 1);
    CLBL_STATIC_ASSERT(clbl::arity<B>::min == 1);

    CLBL_STATIC_ASSERT(clbl::arity<C>::max == 1);
    CLBL_STATIC_ASSERT(clbl::arity<C>::min == 1);

    CLBL_STATIC_ASSERT(clbl::arity<D>::max == 1);
    CLBL_STATIC_ASSERT(clbl::arity<D>::min == 1);

    CLBL_STATIC_ASSERT(clbl::arity<E>::max == 0);
    CLBL_STATIC_ASSERT(clbl::arity<E>::min == 0);

    CLBL_STATIC_ASSERT(clbl::arity<F>::min == 0);
    CLBL_STATIC_ASSERT(clbl::arity<F>::max == 3);

    CLBL_STATIC_ASSERT(clbl::arity<G, 10>::min == 0);
    CLBL_STATIC_ASSERT(clbl::arity<G, 10>::max == 10);

    CLBL_STATIC_ASSERT(clbl::arity<G>::min == 0);
    CLBL_STATIC_ASSERT(clbl::arity<G>::max == clbl::arity<G>::search_limit);
    CLBL_STATIC_ASSERT(clbl::arity<H>::template search<42>::max == 42);
    CLBL_STATIC_ASSERT(clbl::arity<H>::template search<42>::min == 0);
    CLBL_STATIC_ASSERT(clbl::arity<H>::template search<42>::search_limit == 42);

    CLBL_STATIC_ASSERT(clbl::arity<H>::min == 0);
    CLBL_STATIC_ASSERT(clbl::arity<H>::max == clbl::arity<H>::search_limit);
    CLBL_STATIC_ASSERT(clbl::arity<H>::template search<27>::max == 27);

    CLBL_STATIC_ASSERT(clbl::arity<I>::min == 0);
    CLBL_STATIC_ASSERT(clbl::arity<I>::max == 4);

    // commenting this case out because this behavior
    // is technically unspecified by CLBL, because
    // constraints mean we have no guarantees
    //CLBL_STATIC_ASSERT(clbl::arity<J>::min == -1);
    //CLBL_STATIC_ASSERT(clbl::arity<J>::max == -1);

    CLBL_STATIC_ASSERT(clbl::arity<K>::min == 1);
    CLBL_STATIC_ASSERT(clbl::arity<K>::max == 1);

    /* why does this fail?
    CLBL_STATIC_ASSERT(clbl::arity<L>::min == 1);
    CLBL_STATIC_ASSERT(clbl::arity<L>::max == 1);
    */

    CLBL_STATIC_ASSERT(clbl::arity<M>::min == 1);
    CLBL_STATIC_ASSERT(clbl::arity<M>::max == 1);

    CLBL_STATIC_ASSERT(clbl::arity<N>::min == -1);
    CLBL_STATIC_ASSERT(clbl::arity<N>::max == -1);

    CLBL_STATIC_ASSERT(clbl::arity<O>::min == -1);
    CLBL_STATIC_ASSERT(clbl::arity<O>::max == -1);

    CLBL_STATIC_ASSERT(clbl::arity<P>::min == 0);
    CLBL_STATIC_ASSERT(clbl::arity<P>::max == 2);

    CLBL_STATIC_ASSERT(clbl::arity<Q>::min == 7);
    CLBL_STATIC_ASSERT(clbl::arity<Q>::max == 7);

    CLBL_STATIC_ASSERT(clbl::arity<R>::min == 6);
    CLBL_STATIC_ASSERT(clbl::arity<R>::max == 6);

    //limited to 7 arguments unless specifed
    CLBL_STATIC_ASSERT(clbl::arity<S>::min == -1);
    CLBL_STATIC_ASSERT(clbl::arity<S>::max == -1);
    using bigger_search = clbl::arity<S, 20>;
    CLBL_STATIC_ASSERT(std::is_same < bigger_search, typename clbl::arity<S>::template search<20>>{});
    CLBL_STATIC_ASSERT(bigger_search::min == 8);
    CLBL_STATIC_ASSERT(bigger_search::max == 11);

    CLBL_STATIC_ASSERT(clbl::arity<T>::min == 6);
    CLBL_STATIC_ASSERT(clbl::arity<T>::max == 7);

    //doesn't work - no call operator
    //CLBL_STATIC_ASSERT(decltype(clbl::fwrap(A{}))::arity::max == -1);
    //CLBL_STATIC_ASSERT(decltype(clbl::fwrap(A{}))::arity::min == -1);

    /*TODO make this work 
    CLBL_STATIC_ASSERT(decltype(clbl::fwrap(B{}))::arity::max == 1);
    CLBL_STATIC_ASSERT(decltype(clbl::fwrap(B{}))::arity::min == 1);

    CLBL_STATIC_ASSERT(decltype(clbl::fwrap(C{}))::arity::max == 1);
    CLBL_STATIC_ASSERT(decltype(clbl::fwrap(C{}))::arity::min == 1);

    CLBL_STATIC_ASSERT(decltype(clbl::fwrap(D{}))::arity::max == 1);
    CLBL_STATIC_ASSERT(decltype(clbl::fwrap(D{}))::arity::min == 1);

    CLBL_STATIC_ASSERT(decltype(clbl::fwrap(E{}))::arity::max == 0);
    CLBL_STATIC_ASSERT(decltype(clbl::fwrap(E{}))::arity::min == 0);

    CLBL_STATIC_ASSERT(decltype(clbl::fwrap(F{}))::arity::min == 0);
    CLBL_STATIC_ASSERT(decltype(clbl::fwrap(F{}))::arity::max == 3);

    CLBL_STATIC_ASSERT(clbl::arity<G, 10>::min == 0);
    CLBL_STATIC_ASSERT(clbl::arity<G, 10>::max == 10);

    CLBL_STATIC_ASSERT(decltype(clbl::fwrap(G{}))::arity::min == 0);
    CLBL_STATIC_ASSERT(decltype(clbl::fwrap(G{}))::arity::max == decltype(clbl::fwrap(G{}))::arity::search_limit);
    CLBL_STATIC_ASSERT(decltype(clbl::fwrap(H{}))::arity::template search<42>::max == 42);
    CLBL_STATIC_ASSERT(decltype(clbl::fwrap(H{}))::arity::template search<42>::min == 0);
    CLBL_STATIC_ASSERT(decltype(clbl::fwrap(H{}))::arity::template search<42>::search_limit == 42);

    CLBL_STATIC_ASSERT(decltype(clbl::fwrap(H{}))::arity::min == 0);
    CLBL_STATIC_ASSERT(decltype(clbl::fwrap(H{}))::arity::max == decltype(clbl::fwrap(H{}))::arity::search_limit);
    CLBL_STATIC_ASSERT(decltype(clbl::fwrap(H{}))::arity::template search<27>::max == 27);

    CLBL_STATIC_ASSERT(decltype(clbl::fwrap(I{}))::arity::min == 0);
    CLBL_STATIC_ASSERT(decltype(clbl::fwrap(I{}))::arity::max == 4);

    // commenting this case out because this behavior
    // is technically unspecified by CLBL, because
    // constraints mean we have no guarantees
    //CLBL_STATIC_ASSERT(decltype(clbl::fwrap(J{}))::arity::min == -1);
    //CLBL_STATIC_ASSERT(decltype(clbl::fwrap(J{}))::arity::max == -1);

    CLBL_STATIC_ASSERT(decltype(clbl::fwrap(K{}))::arity::min == 1);
    CLBL_STATIC_ASSERT(decltype(clbl::fwrap(K{}))::arity::max == 1);

    CLBL_STATIC_ASSERT(decltype(clbl::fwrap(M{}))::arity::min == 1);
    CLBL_STATIC_ASSERT(decltype(clbl::fwrap(M{}))::arity::max == 1);

    CLBL_STATIC_ASSERT(decltype(clbl::fwrap(N{}))::arity::min == -1);
    CLBL_STATIC_ASSERT(decltype(clbl::fwrap(N{}))::arity::max == -1);

    CLBL_STATIC_ASSERT(decltype(clbl::fwrap(O{}))::arity::min == -1);
    CLBL_STATIC_ASSERT(decltype(clbl::fwrap(O{}))::arity::max == -1);

    CLBL_STATIC_ASSERT(decltype(clbl::fwrap(P{}))::arity::min == 0);
    CLBL_STATIC_ASSERT(decltype(clbl::fwrap(P{}))::arity::max == 2);

    CLBL_STATIC_ASSERT(decltype(clbl::fwrap(Q{}))::arity::min == 7);
    CLBL_STATIC_ASSERT(decltype(clbl::fwrap(Q{}))::arity::max == 7);

    CLBL_STATIC_ASSERT(decltype(clbl::fwrap(R{}))::arity::min == 6);
    CLBL_STATIC_ASSERT(decltype(clbl::fwrap(R{}))::arity::max == 6);

    //limited to 7 arguments unless specifed
    CLBL_STATIC_ASSERT(decltype(clbl::fwrap(S{}))::arity::min == -1);
    CLBL_STATIC_ASSERT(decltype(clbl::fwrap(S{}))::arity::max == -1);
    using bigger_search = clbl::arity<S, 20>;
    CLBL_STATIC_ASSERT(std::is_same < bigger_search, typename decltype(clbl::fwrap(S{}))::arity::template search<20>>{});
    CLBL_STATIC_ASSERT(bigger_search::min == 8);
    CLBL_STATIC_ASSERT(bigger_search::max == 11);

    CLBL_STATIC_ASSERT(decltype(clbl::fwrap(T{}))::arity::min == 6);
    CLBL_STATIC_ASSERT(decltype(clbl::fwrap(T{}))::arity::max == 7);
    */
    return 0;
}