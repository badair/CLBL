/*!
@file
Defines `clbl::pmf`.

@copyright Barrett Adair 2015
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)

*/

#ifndef CLBL_PMF_H
#define CLBL_PMF_H

#include <CLBL/tags.hpp>
#include <CLBL/forward.hpp>
#include <CLBL/qualified_type.hpp>
#include <CLBL/harden_cast.hpp>
#include <CLBL/constraints.hpp>
#include <CLBL/qflags.hpp>

#define CLBL_QTOK_default_
#define CLBL_QTOK_lvalue_reference_ &
#define CLBL_QTOK_rvalue_reference_ &&
#define CLBL_QTOK_const_ const
#define CLBL_QTOK_volatile_ volatile
#define CLBL_QTOK(name) CLBL_PP_CAT(CLBL_QTOK_, name)
#define CLBL_QVAL(name) qflags::name
#define CLBL_APPLY_PMF_QUALIFIERS_STRUCT CLBL_PP_CAT(apply_pmf_qualifiers_, __LINE__)

//todo - get rid of unused template aliases in clbl::pmf - most of it isn't used, 
//and a lot of it doesn't even make sense

#define CLBL_SPCLZ_APPLY_PMF_QUAL_1(QUAL, NAME1)                                     \
template<>                                                                           \
struct CLBL_APPLY_PMF_QUALIFIERS_STRUCT <CLBL_QVAL(NAME1)> {                         \
    template<qualify_flags Flags, typename Return, typename T, typename... Args>     \
    using type = typename std::conditional<                                          \
        (Flags & CLBL_QVAL(NAME1)) == CLBL_QVAL(NAME1),                              \
        Return(T::*)(Args...) QUAL,                                                  \
        Return(T::*)(Args...) CLBL_QTOK(NAME1)                                       \
    >::type;                                                                         \
}                                                                                    \
/**/

#define CLBL_SPCLZ_APPLY_PMF_QUAL_2(QUAL, NAME1, NAME2)                              \
template<>                                                                           \
struct CLBL_APPLY_PMF_QUALIFIERS_STRUCT <                                            \
    CLBL_QVAL(NAME1) | CLBL_QVAL(NAME2)> {                                           \
    template<qualify_flags Flags, typename Return, typename T, typename... Args>     \
    using type = typename std::conditional<                                          \
        (Flags & (CLBL_QVAL(NAME1) | CLBL_QVAL(NAME2)))                              \
            == (CLBL_QVAL(NAME1) | CLBL_QVAL(NAME2)),                                \
        Return(T::*)(Args...) QUAL,                                                  \
        Return(T::*)(Args...) CLBL_QTOK(NAME1) CLBL_QTOK(NAME2)                      \
    >::type;                                                                         \
}                                                                                    \
/**/

#define CLBL_SPCLZ_APPLY_PMF_QUAL_3(QUAL, NAME1, NAME2, NAME3)                       \
template<>                                                                           \
struct CLBL_APPLY_PMF_QUALIFIERS_STRUCT <                                            \
    CLBL_QVAL(NAME1)                                                                 \
    | CLBL_QVAL(NAME2)                                                               \
    | CLBL_QVAL(NAME3)> {                                                            \
    template<qualify_flags Flags, typename Return, typename T, typename... Args>     \
    using type = typename std::conditional<                                          \
        (Flags & (CLBL_QVAL(NAME1)                                                   \
                    | CLBL_QVAL(NAME2)                                               \
                    | CLBL_QVAL(NAME3)                                               \
                )                                                                    \
         )                                                                           \
            == (CLBL_QVAL(NAME1)                                                     \
                | CLBL_QVAL(NAME2)                                                   \
                | CLBL_QVAL(NAME3)                                                   \
               ),                                                                    \
        Return(T::*)(Args...) QUAL,                                                  \
        Return(T::*)(Args...) CLBL_QTOK(NAME1) CLBL_QTOK(NAME2) CLBL_QTOK(NAME3)     \
    >::type;                                                                         \
}                                                                                    \
/**/

#define CLBL_APPLY_PMF_QUAL(QUAL)                                                    \
template<qualify_flags Applied = qflags::default_>                                   \
struct CLBL_APPLY_PMF_QUALIFIERS_STRUCT {                                            \
    template<qualify_flags Flags, typename Return, typename T, typename... Args>     \
    using type = Return(T::*)(Args...) QUAL;                                         \
};                                                                                   \
                                                                                     \
CLBL_SPCLZ_APPLY_PMF_QUAL_1(QUAL, const_);                                           \
CLBL_SPCLZ_APPLY_PMF_QUAL_1(QUAL, volatile_);                                        \
CLBL_SPCLZ_APPLY_PMF_QUAL_1(QUAL, lvalue_reference_);                                \
CLBL_SPCLZ_APPLY_PMF_QUAL_1(QUAL, rvalue_reference_);                                \
CLBL_SPCLZ_APPLY_PMF_QUAL_2(QUAL, const_, volatile_);                                \
CLBL_SPCLZ_APPLY_PMF_QUAL_2(QUAL, const_, lvalue_reference_);                        \
CLBL_SPCLZ_APPLY_PMF_QUAL_2(QUAL, const_, rvalue_reference_);                        \
CLBL_SPCLZ_APPLY_PMF_QUAL_2(QUAL, volatile_, lvalue_reference_);                     \
CLBL_SPCLZ_APPLY_PMF_QUAL_2(QUAL, volatile_, rvalue_reference_);                     \
CLBL_SPCLZ_APPLY_PMF_QUAL_3(QUAL, const_, volatile_, lvalue_reference_);             \
CLBL_SPCLZ_APPLY_PMF_QUAL_3(QUAL, const_, volatile_, rvalue_reference_)              \
/**/

#define CLBL_SPECIALIZE_PMF(QUAL)                                                    \
                                                                                     \
CLBL_APPLY_PMF_QUAL(QUAL);                                                           \
                                                                                     \
template<typename Return, typename T, typename... Args>                              \
struct pmf<Return(T::*)(Args...) QUAL> {                                             \
    static constexpr auto cv_flags = cv_of<dummy QUAL>::value;                       \
    static constexpr auto ref_flags = ref_of<dummy QUAL>::value;                     \
    static constexpr auto q_flags = cv_flags | ref_flags;                            \
    static constexpr bool is_ref_qualified = !(ref_flags | qflags::default_);        \
    using return_type = Return;                                                      \
    using arg_types = std::tuple<Args...>;                                           \
    using type = Return(T::*)(Args...) QUAL;                                         \
    using decay_type = Return(T::*)(Args...);                                        \
    using decay_to_function = Return(Args...);                                       \
    using abominable_type = Return(Args...) QUAL;                                    \
    using forwarding_glue = Return(forward<Args>...);                                \
    using class_type = T;                                                            \
    using invoke_type =                                                              \
            qualified_type<T, qflags::guarantee_reference<cv_flags>::value>;         \
                                                                                     \
    template<qualify_flags Applied>                                                  \
    using apply_qualifiers = typename CLBL_APPLY_PMF_QUALIFIERS_STRUCT<              \
        Applied>::template type<q_flags, Return, T, Args...>;                        \
                                                                                     \
    template<typename Callable>                                                      \
    static auto unevaluated_invoke_with_args_declval(Callable&& c)                   \
        -> decltype(std::declval<Callable>()(std::declval<Args>()...));              \
                                                                                     \
    template<typename U>                                                             \
    using apply_class = Return(U::*)(Args...) QUAL;                                  \
                                                                                     \
    template<typename NewReturn>                                                     \
    using apply_return = NewReturn(T::*)(Args...) QUAL;                              \
                                                                                     \
    template<template<class...> class U>                                             \
    using unpack_args = U<Args...>;                                                  \
                                                                                     \
    template<template<class...> class U, template<class> class K>                    \
    using unpack_args_on = U<K<Args>...>;                                            \
                                                                                     \
    template<template<class...> class U, typename PrependArg>                        \
    using prepend_arg = U<PrependArg, Args...>;                                      \
                                                                                     \
    template<template<class...> class U, typename... PrependArgs>                    \
    using prepend_args = U<PrependArgs..., Args...>;                                 \
                                                                                     \
    template<template<class...> class U, template<class> class K, typename... P>     \
    using prepend_args_on = U<P..., forward<Args>...>;                               \
                                                                                     \
    template<typename AdaptReturn, typename... PrependArgs>                          \
    using prepend_args_to_function = AdaptReturn(PrependArgs..., Args...);           \
                                                                                     \
    template<typename Ret, typename Pre>                                             \
    using prepend_arg_to_forward_function = Ret(Pre, forward<Args>...);              \
                                                                                     \
    template<typename Ret, typename... Pre>                                          \
    using prepend_args_to_forward_function = Ret(Pre..., forward<Args>...);          \
                                                                                     \
    template<typename AdaptReturn>                                                   \
    using unpack_args_to_function = AdaptReturn(Args...);                            \
                                                                                     \
    template<typename AdaptReturn>                                                   \
    using unpack_args_to_forward_function = AdaptReturn(forward<Args>...);           \
                                                                                     \
    template<typename AdaptReturn, typename AdaptClass>                              \
    using unpack_args_to_member_function = AdaptReturn(AdaptClass::*)(Args...);      \
}                                                                                    \
/**/

namespace clbl {

template<typename T>
struct pmf {
    static_assert(sizeof(T) < 0, "invalid member function type");  
};

CLBL_SPECIALIZE_PMF(CLBL_NO_CV);
CLBL_SPECIALIZE_PMF(&);
CLBL_SPECIALIZE_PMF(&&);
CLBL_SPECIALIZE_PMF(const);
CLBL_SPECIALIZE_PMF(volatile);
CLBL_SPECIALIZE_PMF(const volatile);
CLBL_SPECIALIZE_PMF(const &);
CLBL_SPECIALIZE_PMF(volatile &);
CLBL_SPECIALIZE_PMF(const volatile &);
CLBL_SPECIALIZE_PMF(const &&);
CLBL_SPECIALIZE_PMF(volatile &&);
CLBL_SPECIALIZE_PMF(const volatile &&);

}

#endif