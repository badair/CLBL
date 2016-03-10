/*!
@file

@copyright Barrett Adair 2015
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)

*/

#include <quali/quali_macros.hpp>
#include <CLBL/clbl.hpp>
#include <cassert>
#include <string>
#include <functional>

using namespace quali;

template<typename A, typename B, typename C>
struct assert_same
    : std::integral_constant<bool, std::is_same<A, B>{} && std::is_same<A, C>{}>
{};

#define ASSERT_GET_TEMPLATE_RETURN(...) \
static_assert(assert_same< \
    decltype(std::declval<clbl::generalized_object<T>>().template get<QUALI_FLAGS(__VA_ARGS__)>()), \
    decltype(std::declval<clbl::generalized_object<T*>>().template get<QUALI_FLAGS(__VA_ARGS__)>()), \
    decltype(std::declval<clbl::generalized_object<std::reference_wrapper<T>>>().template get<QUALI_FLAGS(__VA_ARGS__)>()) \
>{}, "") \
/**/

template<typename T>
struct assert_generalizations {

    static_assert(assert_same<
        typename clbl::generalized_object<T>::type,
        typename clbl::generalized_object<T*>::type,
        typename clbl::generalized_object<std::reference_wrapper<T>>::type
    >{}, "");

    ASSERT_GET_TEMPLATE_RETURN(QUALI_DEFAULT);
    ASSERT_GET_TEMPLATE_RETURN(&);
    //pointer types and ref-wrapped types are not moved, which is why this fails:
    //ASSERT_GET_TEMPLATE_RETURN(&&);
    ASSERT_GET_TEMPLATE_RETURN(const);
    ASSERT_GET_TEMPLATE_RETURN(const, &);
    ASSERT_GET_TEMPLATE_RETURN(volatile);
    ASSERT_GET_TEMPLATE_RETURN(volatile, &);
    ASSERT_GET_TEMPLATE_RETURN(const, volatile);
    ASSERT_GET_TEMPLATE_RETURN(const, volatile, &);

    template<typename U>
    using generalize_call_result = decltype(clbl::generalize<U>(std::declval<U>()));

    static_assert(assert_same<
        generalize_call_result<T&>,
        generalize_call_result<T*&>,
        generalize_call_result<std::reference_wrapper<T>&>
    >{}, "");

    /* TODO
    static_assert(assert_same<
    generalize_call_result<T const &>,
    generalize_call_result<T* const &>,
    generalize_call_result<std::reference_wrapper<T> const &>
    >{}, "");

    static_assert(assert_same<
    generalize_call_result<T>,
    generalize_call_result<T*>,
    generalize_call_result<std::reference_wrapper<T>>
    >{}, "");
    */
};


template<typename T>
assert_generalizations<T>
test() { return{}; }

struct foo {};

int main() {
    test<foo>();
    test<foo const>();
    test<foo volatile>();
    test<foo const volatile>();

    return 0;
}