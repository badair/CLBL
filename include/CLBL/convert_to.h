#include<functional>
#include <type_traits>

#include "CLBL/tags.h"

namespace clbl {
    template<template<class> class TypeErasedFunctionTemplate, typename Callable>
    inline auto convert_to(Callable&& c) {

        using callable_type = std::remove_cv_t< std::remove_reference_t<Callable> >;

        static_assert(!std::is_same<typename callable_type::return_t, ambiguous_return>::value,
            "Ambiguous signature. You can harden before calling as<>, but be aware of argument transformations for forwarding.");

        return TypeErasedFunctionTemplate<typename callable_type::type>{std::forward<Callable>(c)};
    }

    template<template<class> class TypeErasedFunctionTemplate, typename Callable>
    inline auto convert_to(std::reference_wrapper<Callable> c) {

        using callable_type = std::remove_cv_t< std::remove_reference_t<Callable> >;

        static_assert(!std::is_same<typename callable_type::return_t, ambiguous_return>::value,
            "Ambiguous signature. You can harden before calling as<>, but be aware of argument transformations for forwarding.");

        return TypeErasedFunctionTemplate<typename callable_type::type>{c.get()};
    }
}