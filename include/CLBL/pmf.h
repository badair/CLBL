#ifndef CLBL_PMF_H
#define CLBL_PMF_H

#include <CLBL/tags.h>
#include <CLBL/forward.h>
#include <CLBL/qualified_type.h>
#include <CLBL/harden_cast.h>
#include <CLBL/qflags.h>

namespace clbl {

template<typename T>
struct pmf {
    static_assert(sizeof(T) < 0, "invalid member function type");
};
    
#define CLBL_SPECIALIZE_PMF(QUAL)                                                    \
                                                                                     \
    template<typename Return, typename T, typename... Args>                          \
    struct pmf<Return(T::*)(Args...) QUAL> {                                         \
                                                                                     \
        static constexpr auto flags = cv_of<dummy QUAL> | ref_of<dummy QUAL>;        \
        static constexpr auto cv_flags = cv_of<dummy QUAL>;                          \
        static constexpr auto ref_flags = ref_of<dummy QUAL>;                        \
        using return_type = Return;                                                  \
        using arg_types = std::tuple<Args...>;                                       \
        using type = Return(T::*)(Args...) QUAL;                                     \
        using decay_type = Return(T::*)(Args...);                                    \
        using decay_to_function = Return(Args...);                                   \
        using abominable_type = Return(Args...) QUAL;                                \
        using forwarding_glue = Return(forward<Args>...);                            \
        using class_type = T;                                                        \
                                                                                     \
        template<typename Callable>                                                  \
        static auto unevaluated_invoke_with_args_declval(Callable&& c)               \
            -> decltype(std::declval<Callable>()(std::declval<Args>()...));          \
                                                                                     \
        template<typename U>                                                         \
        using apply_class = Return(U::*)(Args...) QUAL;                              \
                                                                                     \
        template<typename NewReturn>                                                 \
        using apply_return = NewReturn(T::*)(Args...) QUAL;                          \
                                                                                     \
        template<template<class...> class U>                                         \
        using unpack_args = U<Args...>;                                              \
                                                                                     \
        template<template<class...> class U, template<class> class K>                \
        using unpack_args_on = U<K<Args>...>;                                        \
                                                                                     \
        template<template<class...> class U, typename PrependArg>                    \
        using prepend_arg = U<PrependArg, Args...>;                                  \
                                                                                     \
        template<template<class...> class U, typename... PrependArgs>                \
        using prepend_args = U<PrependArgs..., Args...>;                             \
                                                                                     \
        template<template<class...> class U, template<class> class K, typename... P> \
        using prepend_args_on = U<P..., forward<Args>...>;                           \
                                                                                     \
        template<typename AdaptReturn, typename... PrependArgs>                      \
        using prepend_args_to_function = AdaptReturn(PrependArgs..., Args...);       \
                                                                                     \
        template<typename Ret, typename Pre>                                         \
        using prepend_arg_to_forward_function = Ret(Pre, forward<Args>...);          \
                                                                                     \
        template<typename Ret, typename... Pre>                                      \
        using prepend_args_to_forward_function = Ret(Pre..., forward<Args>...);      \
                                                                                     \
        template<typename AdaptReturn>                                               \
        using unpack_args_to_function = AdaptReturn(Args...);                        \
                                                                                     \
        template<typename AdaptReturn>                                               \
        using unpack_args_to_forward_function = AdaptReturn(forward<Args>...);       \
                                                                                     \
        template<typename AdaptReturn, typename AdaptClass>                          \
        using unpack_args_to_member_function = AdaptReturn(AdaptClass::*)(Args...);  \
    }

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