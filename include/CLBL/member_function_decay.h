#ifndef CLBL_MEMBER_FUNCTION_DECAY_H
#define CLBL_MEMBER_FUNCTION_DECAY_H

//#include "CLBL/abominable_function_decay.h"
#include<type_traits>

//todo: experiment with hana-style dispatch with a tuple for all cv metafunctions for faster compile times
#define __SPECIALIZE_MEMBER_FUNCTION_DECAY(qualifiers) \
    template<typename T, typename Return, typename... Args> \
    struct member_function_decay_t<Return(T::*)(Args...) qualifiers> { using type = Return(T::*)(Args...); }

#define __SPECIALIZE_MEMBER_FUNCTION_DECAY_ELLIPSES(qualifiers) \
    template<typename T, typename Return, typename... Args> \
    struct member_function_decay_t<Return(T::*)(Args...,...) qualifiers> { using type = Return(T::*)(Args...,...); }

namespace clbl {

    //primary template "fails" silently
    template<typename Other> struct member_function_decay_t { using type = Other;/*abominable_function_decay<Other>;*/ };

    template<typename T, typename Return, typename... Args>
    struct member_function_decay_t<Return(T::*)(Args...)> { using type = Return(T::*)(Args...); };

    template<typename T, typename Return, typename... Args>
    struct member_function_decay_t<Return(T::*)(Args..., ...)> { using type = Return(T::*)(Args..., ...); };

    __SPECIALIZE_MEMBER_FUNCTION_DECAY(&);
    __SPECIALIZE_MEMBER_FUNCTION_DECAY(&&);
    __SPECIALIZE_MEMBER_FUNCTION_DECAY(const);
    __SPECIALIZE_MEMBER_FUNCTION_DECAY(volatile);
    __SPECIALIZE_MEMBER_FUNCTION_DECAY(const volatile);
    __SPECIALIZE_MEMBER_FUNCTION_DECAY(const &);
    __SPECIALIZE_MEMBER_FUNCTION_DECAY(volatile &);
    __SPECIALIZE_MEMBER_FUNCTION_DECAY(const volatile &);
    __SPECIALIZE_MEMBER_FUNCTION_DECAY(const &&);
    __SPECIALIZE_MEMBER_FUNCTION_DECAY(volatile &&);
    __SPECIALIZE_MEMBER_FUNCTION_DECAY(const volatile &&);

    __SPECIALIZE_MEMBER_FUNCTION_DECAY_ELLIPSES(&);
    __SPECIALIZE_MEMBER_FUNCTION_DECAY_ELLIPSES(&&);
    __SPECIALIZE_MEMBER_FUNCTION_DECAY_ELLIPSES(const);
    __SPECIALIZE_MEMBER_FUNCTION_DECAY_ELLIPSES(volatile);
    __SPECIALIZE_MEMBER_FUNCTION_DECAY_ELLIPSES(const volatile);
    __SPECIALIZE_MEMBER_FUNCTION_DECAY_ELLIPSES(const &);
    __SPECIALIZE_MEMBER_FUNCTION_DECAY_ELLIPSES(volatile &);
    __SPECIALIZE_MEMBER_FUNCTION_DECAY_ELLIPSES(const volatile &);
    __SPECIALIZE_MEMBER_FUNCTION_DECAY_ELLIPSES(const &&);
    __SPECIALIZE_MEMBER_FUNCTION_DECAY_ELLIPSES(volatile &&);
    __SPECIALIZE_MEMBER_FUNCTION_DECAY_ELLIPSES(const volatile &&);

    template<typename T>
    using member_function_decay = typename member_function_decay_t<T>::type;
}

#endif