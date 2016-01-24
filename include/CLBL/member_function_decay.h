/*!
@file
Defines `clbl::member_function_decay`.

@copyright Barrett Adair 2015
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)

*/

#ifndef CLBL_MEMBER_FUNCTION_DECAY_H
#define CLBL_MEMBER_FUNCTION_DECAY_H

#include<type_traits>

/*
we use the member_function_decay metafunction to strip
qualifiers from PMFs, which allows us to use partial 
template specializations to break down signatures
*/

#define CLBL_SPECIALIZE_MEMBER_FUNCTION_DECAY(qualifiers) \
    template<typename T, typename Return, typename... Args> \
    struct member_function_decay_t<Return(T::*)(Args...) qualifiers> { using type = Return(T::*)(Args...); }

#define CLBL_SPECIALIZE_MEMBER_FUNCTION_DECAY_ELLIPSES(qualifiers) \
    template<typename T, typename Return, typename... Args> \
    struct member_function_decay_t<Return(T::*)(Args...,...) qualifiers> { using type = Return(T::*)(Args...,...); }

namespace clbl {

    //primary template fails silently
    template<typename Other> struct member_function_decay_t { using type = Other; };

    template<typename T, typename Return, typename... Args>
    struct member_function_decay_t<Return(T::*)(Args...)> { using type = Return(T::*)(Args...); };

    template<typename T, typename Return, typename... Args>
    struct member_function_decay_t<Return(T::*)(Args..., ...)> { using type = Return(T::*)(Args..., ...); };

    CLBL_SPECIALIZE_MEMBER_FUNCTION_DECAY(&);
    CLBL_SPECIALIZE_MEMBER_FUNCTION_DECAY(&&);
    CLBL_SPECIALIZE_MEMBER_FUNCTION_DECAY(const);
    CLBL_SPECIALIZE_MEMBER_FUNCTION_DECAY(volatile);
    CLBL_SPECIALIZE_MEMBER_FUNCTION_DECAY(const volatile);
    CLBL_SPECIALIZE_MEMBER_FUNCTION_DECAY(const &);
    CLBL_SPECIALIZE_MEMBER_FUNCTION_DECAY(volatile &);
    CLBL_SPECIALIZE_MEMBER_FUNCTION_DECAY(const volatile &);
    CLBL_SPECIALIZE_MEMBER_FUNCTION_DECAY(const &&);
    CLBL_SPECIALIZE_MEMBER_FUNCTION_DECAY(volatile &&);
    CLBL_SPECIALIZE_MEMBER_FUNCTION_DECAY(const volatile &&);

    CLBL_SPECIALIZE_MEMBER_FUNCTION_DECAY_ELLIPSES(&);
    CLBL_SPECIALIZE_MEMBER_FUNCTION_DECAY_ELLIPSES(&&);
    CLBL_SPECIALIZE_MEMBER_FUNCTION_DECAY_ELLIPSES(const);
    CLBL_SPECIALIZE_MEMBER_FUNCTION_DECAY_ELLIPSES(volatile);
    CLBL_SPECIALIZE_MEMBER_FUNCTION_DECAY_ELLIPSES(const volatile);
    CLBL_SPECIALIZE_MEMBER_FUNCTION_DECAY_ELLIPSES(const &);
    CLBL_SPECIALIZE_MEMBER_FUNCTION_DECAY_ELLIPSES(volatile &);
    CLBL_SPECIALIZE_MEMBER_FUNCTION_DECAY_ELLIPSES(const volatile &);
    CLBL_SPECIALIZE_MEMBER_FUNCTION_DECAY_ELLIPSES(const &&);
    CLBL_SPECIALIZE_MEMBER_FUNCTION_DECAY_ELLIPSES(volatile &&);
    CLBL_SPECIALIZE_MEMBER_FUNCTION_DECAY_ELLIPSES(const volatile &&);

    template<typename T>
    using member_function_decay = typename member_function_decay_t<T>::type;
}

#endif