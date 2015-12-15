#include<functional>
#include <type_traits>
#include <boost/hana.hpp>

#include "CLBL/tags.h"
#include "CLBL/qualify_flags.h"
#include "CLBL/forwardable.h"
#include "CLBL/harden.h"
#include "CLBL/utility.h"
#include "CLBL/forwarding_glue.h"

namespace hana = boost::hana;

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

    //std::function doesn't call const functions on copies of const objects - we want to force this behavior
    template<typename GlueType, typename Callable>
    constexpr inline auto apply_glue(Callable&& c) {
        using C = no_ref<Callable>;
        return detail::apply_glue_t<C, GlueType>{C::copy_invocation(c)};
    }

    template<template<class> class TypeErasedFunctionTemplate, typename Callable>
    inline auto convert_to(Callable&& c) {

        static_assert(!std::is_same<typename no_ref<Callable>::return_t, ambiguous_return>::value,
            "Ambiguous signature. Please disambiguate by calling clbl::harden before calling clbl::convert_to .");

        using glue = no_ref<Callable>::forwarding_glue;
        return TypeErasedFunctionTemplate<glue> { apply_glue<glue>(std::forward<Callable>(c)) };
    }
}