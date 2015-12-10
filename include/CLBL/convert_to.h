#include<functional>
#include <type_traits>
#include <boost/hana.hpp>

#include "CLBL/tags.h"
#include "CLBL/forwardable.h"
#include "CLBL/abominable_function_decay.h"
#include "CLBL/harden.h"
#include "CLBL/utility.h"
#include "CLBL/forwarding_glue.h"

//sorry to anyone reading this - this is terribly tricky, and is very hard without macros

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
        return detail::apply_glue_t<no_ref<Callable>, GlueType>{no_ref<Callable>::copy_invocation(c)};
    }

    template<template<class> class TypeErasedFunctionTemplate, typename Callable>
    inline auto convert_to(Callable&& c) {

        static_assert(!std::is_same<typename no_ref<Callable>::return_t, ambiguous_return>::value,
            "Ambiguous signature. You can harden before calling convert_to, but be aware of argument transformations for forwarding.");

        using glue = fowarding_glue<Callable>;

        return TypeErasedFunctionTemplate<glue> { apply_glue<glue>(std::forward<Callable>(c)) };
    }
}