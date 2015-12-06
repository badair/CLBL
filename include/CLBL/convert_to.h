#include<functional>
#include <type_traits>
#include <boost/hana.hpp>

#include "CLBL/tags.h"
#include "CLBL/forwardable.h"

//sorry to anyone reading this - this is terribly tricky, and is very hard without macros

namespace hana = boost::hana;

namespace clbl {

#define CLBL_UNDERLYING(T) std::remove_cv_t<std::remove_reference_t<T> >

#define __CLBL_SPECIALIZE_MEMBER_FUNCTION_CAST(name, c_qual, v_qual, ref_qual) \
    template<typename, typename Bad> struct name {}; \
    template<typename Callable, typename... Args> \
    struct name<Callable, hana::tuple<Args...> > { \
        using ret = typename CLBL_UNDERLYING(Callable)::return_t; \
        using type = ret(std::remove_reference_t<Callable>::*)(forwardable<Args>...) c_qual v_qual ref_qual; \
    }

    //using these to cast a Callable's operator() to the appropriately qualified overload
    __CLBL_SPECIALIZE_MEMBER_FUNCTION_CAST(get_mem_fn_cast_no_cv, CLBL_NOTHING, CLBL_NOTHING, CLBL_NOTHING);
    __CLBL_SPECIALIZE_MEMBER_FUNCTION_CAST(get_mem_fn_cast_c, const, CLBL_NOTHING, CLBL_NOTHING);
    __CLBL_SPECIALIZE_MEMBER_FUNCTION_CAST(get_mem_fn_cast_v, CLBL_NOTHING, volatile, CLBL_NOTHING);
    __CLBL_SPECIALIZE_MEMBER_FUNCTION_CAST(get_mem_fn_cast_cv, const, volatile, CLBL_NOTHING);

#define __CLBL_SPECIALIZE_CONVERT_TO(not_c, not_v, get_cast, c_qual, v_qual, ref_qual) \
    template<template<class> class TypeErasedFunctionTemplate, typename Callable> \
    inline auto convert_to(Callable&& c) -> std::enable_if_t<not_c CLBL_UNDERLYING(Callable)::clbl_is_const \
                                                            && not_v CLBL_UNDERLYING(Callable)::clbl_is_volatile, \
        TypeErasedFunctionTemplate<typename CLBL_UNDERLYING(Callable)::type> > { \
        static_assert(!std::is_same<typename CLBL_UNDERLYING(Callable)::return_t, ambiguous_return>::value, \
            "Ambiguous signature. You can harden before calling convert_to, but be aware of argument transformations for forwarding."); \
        return TypeErasedFunctionTemplate<typename CLBL_UNDERLYING(Callable)::type>{ \
            func(c, static_cast<typename get_cast<Callable, CLBL_UNDERLYING(Callable)::args_t>::type>( \
                        &std::remove_reference_t<Callable>::operator() \
                    ) \
            ) \
        }; \
    }

    __CLBL_SPECIALIZE_CONVERT_TO(!,            !,            get_mem_fn_cast_no_cv, CLBL_NOTHING, CLBL_NOTHING, CLBL_NOTHING)
    __CLBL_SPECIALIZE_CONVERT_TO(CLBL_NOTHING, !,            get_mem_fn_cast_c,     const,        CLBL_NOTHING, CLBL_NOTHING)
    __CLBL_SPECIALIZE_CONVERT_TO(!,            CLBL_NOTHING, get_mem_fn_cast_v,     CLBL_NOTHING, volatile,     CLBL_NOTHING)
    __CLBL_SPECIALIZE_CONVERT_TO(CLBL_NOTHING, CLBL_NOTHING, get_mem_fn_cast_cv,    const,        volatile,     CLBL_NOTHING)
    //todo ellipses and ref qualifiers
}