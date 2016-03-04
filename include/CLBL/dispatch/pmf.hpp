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
#include <quali/quali_macros.hpp>
#include <CLBL/constraints.hpp>
#include <CLBL/type_traits.hpp>
#include <CLBL/has_normal_call_operator.hpp>

#define CLBL_APPLY_PMF_QUALIFIERS_STRUCT CLBL_PP_CAT(apply_pmf_qualifiers_, __LINE__)

//todo - get rid of unused template aliases in clbl::pmf - most of it isn't used, 
//and a lot of it doesn't even make sense

#define CLBL_SPCLZ_APPLY_PMF_QUAL(QUAL, ...)                                         \
template<>                                                                           \
struct CLBL_APPLY_PMF_QUALIFIERS_STRUCT <QUALI(FLAGS, __VA_ARGS__)> {                \
    template<quali::flags Flags, typename Return, typename T, typename... Args>      \
    using type = typename std::conditional<                                          \
        (Flags & QUALI(FLAGS, __VA_ARGS__)) == QUALI(FLAGS, __VA_ARGS__),            \
        Return(T::*)(Args...) QUAL,                                                  \
        Return(T::*)(Args...) QUALI(TOKENS, __VA_ARGS__)                             \
    >::type;                                                                         \
}                                                                                    \
/**/

#define CLBL_APPLY_PMF_QUAL(QUAL, ...)                                               \
template<quali::flags Applied = quali::default_>                                     \
struct CLBL_APPLY_PMF_QUALIFIERS_STRUCT {                                            \
    template<quali::flags Flags, typename Return, typename T, typename... Args>      \
    using type = Return(T::*)(Args...) QUAL;                                         \
};                                                                                   \
                                                                                     \
CLBL_SPCLZ_APPLY_PMF_QUAL(QUAL, &);                                                  \
CLBL_SPCLZ_APPLY_PMF_QUAL(QUAL, &&);                                                 \
CLBL_SPCLZ_APPLY_PMF_QUAL(QUAL, const);                                              \
CLBL_SPCLZ_APPLY_PMF_QUAL(QUAL, volatile);                                           \
CLBL_SPCLZ_APPLY_PMF_QUAL(QUAL, const, &);                                           \
CLBL_SPCLZ_APPLY_PMF_QUAL(QUAL, const, &&);                                          \
CLBL_SPCLZ_APPLY_PMF_QUAL(QUAL, volatile, &);                                        \
CLBL_SPCLZ_APPLY_PMF_QUAL(QUAL, volatile, &&);                                       \
CLBL_SPCLZ_APPLY_PMF_QUAL(QUAL, const, volatile);                                    \
CLBL_SPCLZ_APPLY_PMF_QUAL(QUAL, const, volatile, &);                                 \
CLBL_SPCLZ_APPLY_PMF_QUAL(QUAL, const, volatile, &&)                                 \
/**/

#define CLBL_SPECIALIZE_PMF_DETAIL(QUAL, ...)                                        \
                                                                                     \
CLBL_APPLY_PMF_QUAL(QUAL, __VA_ARGS__);                                              \
                                                                                     \
template<typename Return, typename T, typename... Args>                              \
struct pmf<Return(T::*)(Args...) QUAL> {                                             \
    static constexpr const auto cv_flags = quali::cv_of<dummy QUAL>::value;          \
    static constexpr const auto ref_flags = quali::ref_of<dummy QUAL>::value;        \
    static constexpr const auto q_flags = QUALI(FLAGS, __VA_ARGS__);                 \
    static constexpr const bool is_ref_qualified = !(ref_flags | quali::default_);   \
    static constexpr const bool is_valid = true;                                     \
    static constexpr const bool value = is_valid;                                    \
    static constexpr const bool has_varargs = false;                                 \
    static constexpr const bool is_ambiguous = false;                                \
    using return_type = Return;                                                      \
    using arg_types = std::tuple<Args...>;                                           \
    using type = Return(T::*)(Args...) QUAL;                                         \
    using constructor_type = type;                                                   \
    using decay_type = Return(T::*)(Args...);                                        \
    using decay_to_function = Return(Args...);                                       \
    using abominable_type = Return(Args...) QUAL;                                    \
    using forwarding_glue = Return(forward<Args>...);                                \
    using class_type = T;                                                            \
    using dispatch_type = pmf;                                                       \
    using unqualified = pmf<Return(T::*)(Args...)>;                                  \
                                                                                     \
    using invoke_type =                                                              \
        quali::qualified_type<T, quali::guarantee_reference<cv_flags>::value>;       \
                                                                                     \
    template<typename U>                                                             \
    using can_invoke_with = std::is_convertible<                                     \
        typename std::remove_pointer<U>::type,                                       \
        invoke_type                                                                  \
    >;                                                                               \
                                                                                     \
    template<quali::flags Added>                                                     \
    using add_qualifiers = pmf<                                                      \
        typename CLBL_APPLY_PMF_QUALIFIERS_STRUCT<                                   \
            Added                                                                    \
        >::template type<cv_flags, Return, T, Args...>                               \
    >;                                                                               \
                                                                                     \
    using remove_reference = pmf<                                                    \
        typename unqualified::template add_qualifiers<cv_flags>                      \
    >;                                                                               \
                                                                                     \
    using remove_const = pmf<                                                        \
        typename unqualified::template add_qualifiers<                               \
            ref_flags | quali::remove_const<cv_flags>::value                         \
        >                                                                            \
    >;                                                                               \
                                                                                     \
    using remove_volatile = pmf<                                                     \
        typename unqualified::template add_qualifiers<                               \
            ref_flags | quali::remove_volatile<cv_flags>::value                      \
        >                                                                            \
    >;                                                                               \
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
    template<typename... PrependArgs>                                                \
    using prepend_args_to_function = Return(PrependArgs..., Args...);                \
                                                                                     \
    template<typename... PrependArgs>                                                \
    using prepend_arg_to_function = Return(PrependArgs..., Args...);                 \
                                                                                     \
    template<typename Pre>                                                           \
    using prepend_arg_to_forward_function = Return(Pre, forward<Args>...);           \
                                                                                     \
    template<typename... Pre>                                                        \
    using prepend_args_to_forward_function = Return(Pre..., forward<Args>...);       \
                                                                                     \
    template<typename AdaptReturn>                                                   \
    using unpack_args_to_function = AdaptReturn(Args...);                            \
                                                                                     \
    template<typename AdaptReturn>                                                   \
    using unpack_args_to_forward_function = AdaptReturn(forward<Args>...);           \
                                                                                     \
    template<typename AdaptReturn, typename AdaptClass>                              \
    using unpack_args_to_member_function = AdaptReturn(AdaptClass::*)(Args...);      \
};                                                                                   \
                                                                                     \
template<typename Return, typename T, typename... Args>                              \
struct pmf<Return(T::*)(Args..., ...) QUAL>                                          \
    : public pmf<Return(T::*)(Args...)>{                                             \
    static constexpr const bool has_varargs = true;                                  \
}                                                                                    \
/**/

namespace clbl {

template<typename T>
struct pmf {
    static constexpr const bool is_valid = false;
    static constexpr const bool value = is_valid;
    static constexpr const bool is_ambiguous = true;
    using dispatch_type = pmf;
    using return_type = dummy;
    static constexpr auto q_flags = quali::default_;
    static constexpr auto ref_flags = quali::default_;
};

#define CLBL_SPECIALIZE_PMF(...) \
    CLBL_SPECIALIZE_PMF_DETAIL(QUALI(TOKENS, __VA_ARGS__), __VA_ARGS__)

CLBL_SPECIALIZE_PMF(QUALI_DEFAULT);
CLBL_SPECIALIZE_PMF(&);
CLBL_SPECIALIZE_PMF(&&);
CLBL_SPECIALIZE_PMF(const);
CLBL_SPECIALIZE_PMF(volatile);
CLBL_SPECIALIZE_PMF(const, volatile);
CLBL_SPECIALIZE_PMF(const, &);
CLBL_SPECIALIZE_PMF(volatile, &);
CLBL_SPECIALIZE_PMF(const, volatile, &);
CLBL_SPECIALIZE_PMF(const, &&);
CLBL_SPECIALIZE_PMF(volatile, &&);
CLBL_SPECIALIZE_PMF(const, volatile, &&);

template<typename T, T Value>
struct pmf<std::integral_constant<T, Value> >
    : public pmf<T> {
    using constructor_type = std::integral_constant<T, Value>;
    using dispatch_type = pmf;
};

template<typename T>
struct pmf < pmf<T> > : pmf<T> {};

}

#undef CLBL_APPLY_PMF_QUALIFIERS_STRUCT
#undef CLBL_SPCLZ_APPLY_PMF_QUAL
#undef CLBL_APPLY_PMF_QUAL
#undef CLBL_SPECIALIZE_PMF_DETAIL
#undef CLBL_SPECIALIZE_PMF

#endif