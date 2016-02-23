/*!
@file
Defines `clbl::no_ref`.

@copyright Barrett Adair 2015
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)

*/

#ifndef CLBL_NOREF_H
#define CLBL_NOREF_H

#include <type_traits>

namespace clbl {
	
template<typename T>
using no_ref = typename std::remove_reference<T>::type;

template<typename T>
using no_const = typename std::remove_const<T>::type;

template<typename T>
using no_const_no_ref = 
    typename std::remove_const<
        typename std::remove_reference<T>::type
    >::type;

template<typename T>
using no_volatile = typename std::remove_volatile<T>::type;

template<typename T>
struct not_is_class {
    static constexpr const bool value = !std::is_class<T>::value;
};

//C++17 std::disjunction shim - short-circuiting template instantiation 
template<typename...>
struct disjunction
    : std::false_type {};

template<typename T>
struct disjunction<T>
    : T {};

template<typename T, typename... Ts>
struct disjunction<T, Ts...>
    : std::conditional_t<T::value != false, T, disjunction<Ts...>>  { };

template<class...>
struct conjunction
    : std::true_type { };

template<class T>
struct conjunction<T>
    : T { };

template<class T, class... Ts>
struct conjunction<T, Ts...>
    : std::conditional_t<T::value != false, conjunction<Ts...>, T>  {};

template<typename T>
using call_operator = decltype(&T::operator());

}

#endif