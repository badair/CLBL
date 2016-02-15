/*!
@file
Defines `clbl::convert_to`.

@copyright Barrett Adair 2015
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)

*/


#include <functional>
#include <type_traits>
#include <utility>

#include <CLBL/tags.h>
#include <CLBL/type_traits.h>
#include <CLBL/harden.h>
#include <CLBL/qflags.h>
#include <CLBL/invocation_copy.h>
#include <CLBL/forwarding_glue.h>


namespace clbl {
    
    /*
    std::function doesn't call CV-qualified overloads of function
    objects when it is initialized with a copy, because CV doesn't stick
    to copies. We elimiate overloads except the requested one by copying
    only the desired invocation, which lives inside a lambda. This preserves
    the desired behavior by giving std::function no choice but to call the
    CV-qualified version, if desired. 
    
    We "glue" the CLBL wrapper to std::function by (ultimately) using
    clbl::forward to prevent copies while the arguments travel from
    std::function -> invocation lambda -> CLBL wrapper -> original callable
    */

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

    template<typename GlueType, typename Callable>
    constexpr inline auto apply_glue(Callable&& c) {
        using C = no_ref<Callable>;
        return detail::apply_glue_t<C, GlueType>{C::copy_invocation(c)};
    }

    template<template<class> class TypeErasedFunctionTemplate, typename Callable>
    inline auto convert_to(Callable&& c) {

        static_assert(!no_ref<Callable>::is_ambiguous,
            "Ambiguous signature. Please disambiguate by calling clbl::harden before calling clbl::convert_to.");

        using glue = typename no_ref<Callable>::forwarding_glue;
        return TypeErasedFunctionTemplate<glue> { apply_glue<glue>(static_cast<Callable&&>(c)) };
    }
}