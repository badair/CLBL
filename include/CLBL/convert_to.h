#include<functional>
#include <type_traits>

#include "CLBL/tags.h"
#include "CLBL/qualify_flags.h"
#include "CLBL/forwardable.h"
#include "CLBL/harden.h"
#include "CLBL/utility.h"

namespace clbl {

    namespace detail {

        template<typename, typename BadGlueType>
        struct apply_glue_t {
            static_assert(sizeof(BadGlueType) < 0, "Invalid template arguments.");
        };

        template<typename Callable, typename Return, typename... GlueArgs>
        struct apply_glue_t<Callable, Return(GlueArgs...)> {
            invocation_copy<Callable> invocation;
            inline Return operator()(GlueArgs... args) { return invocation(args...); }
        };
    }

    /*
    std::function doesn't call cv-qualified overloads because
    it takes a copy. We elimiate overloads except the hardened one
    by copying the desired invocation to achieve the desired behavior
    of calling the const-qualified version
    */
    template<typename GlueType, typename Callable>
    constexpr inline auto apply_glue(Callable&& c) {
        using C = no_ref<Callable>;
        return detail::apply_glue_t<C, GlueType>{C::copy_invocation(c)};
    }

    template<template<class> class TypeErasedFunctionTemplate, typename Callable>
    inline auto convert_to(Callable&& c) {

        static_assert(!std::is_same<typename no_ref<Callable>::return_t, ambiguous_return>::value,
            "Ambiguous signature. Please disambiguate by calling clbl::harden before calling clbl::convert_to.");

        using glue = typename no_ref<Callable>::forwarding_glue;
        return TypeErasedFunctionTemplate<glue> { apply_glue<glue>(std::forward<Callable>(c)) };
    }
}