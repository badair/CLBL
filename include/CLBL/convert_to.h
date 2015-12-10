#include<functional>
#include <type_traits>
#include <boost/hana.hpp>

#include "CLBL/tags.h"
#include "CLBL/forwardable.h"
#include "CLBL/abominable_function_decay.h"
#include "CLBL/harden.h"

//sorry to anyone reading this - this is terribly tricky, and is very hard without macros

namespace hana = boost::hana;

namespace clbl {

    namespace detail {
        template<typename, typename Bad> struct foward_fn_t {
            static_assert("Not a clbl callable wrapper.");
        };

        template<typename Callable, typename... Args>
        struct foward_fn_t<Callable, hana::tuple<Args...> > {
            using ret = typename std::remove_reference_t<Callable>::return_t;
            using type = ret(forward<Args>...);
        };
    }

    template<typename Callable>
    using forward_fn = typename detail::foward_fn_t<Callable, args<Callable> >::type;

    template<typename T, typename FunctionType> 
    struct cv_enforcer {
        static_assert(sizeof(T) < 0, "Not a function type.");
    };

    template<typename Callable, typename Return, typename... Args>
    struct cv_enforcer<Callable, Return(Args...)>{

        Callable callable;

        Return operator()(Args... args) { return callable(args...); }
    };

    //std::function doesn't call const functions on copies of const objects - we want to force this behavior
    template<typename FunctionType, typename Callable>
    auto enforce_cv(Callable&& c) {
        return cv_enforcer<std::remove_reference_t<Callable>, FunctionType>{c};
    }

    template<template<class> class TypeErasedFunctionTemplate, typename Callable>
    inline auto convert_to(Callable&& c) {

        static_assert(!std::is_same<typename std::remove_reference_t<Callable>::return_t, ambiguous_return>::value,
            "Ambiguous signature. You can harden before calling convert_to, but be aware of argument transformations for forwarding.");

        using fwd_fn = forward_fn<Callable>;

        return TypeErasedFunctionTemplate<fwd_fn> { enforce_cv<fwd_fn>(std::forward<Callable>(c)) };
    }
}