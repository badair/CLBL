/*!
@file
Defines `clbl::harden`.

@copyright Barrett Adair 2015
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)

*/

#ifndef CLBL_HARDEN_H
#define CLBL_HARDEN_H

#ifndef CLBL_EXCLUDE_FUNCTIONAL
#include <functional>
#endif

#include <utility>

#include <CLBL/tags.h>
#include <CLBL/qflags.h>
#include <CLBL/fwrap.h> //todo include factory headers individually
#include <CLBL/harden_cast.h>

namespace clbl {

namespace detail {

    /*
    disambiguate defaults to original creator's wrap_data function - the 
    resulting wrappers already handle CV disambiguation, thanks to 
    CV flags

    TODO - disallow invalid function signatures for CV-only disambiguation
    */
    template<typename, typename, typename Creator>
    struct disambiguate : Creator {};

    /*
    These specializations "override" the *::ambiguous and *::casted creators'
    wrap_data functions and pass the requested disambiguation, which ultimately
    results in a static_cast on operator() - unfortunately, the current C++
    standard forbids casting non-type function pointer template arguments, so we
    just pass the requested type all the way down to the invocation data level,
    where the cast will be performed while initializing a static constexpr member
    (see casted_* structs defined in CLBL/invocation_data.h TODO update this comment)
    */
    template<typename TMemberFnPtr, typename C>
    struct disambiguate<TMemberFnPtr, C, typename function_object_ptr_wrapper_factory::ambiguous> {
        template<qualify_flags Flags, typename Invocation>
        static inline constexpr auto
        wrap_data(Invocation&& data) {
            return function_object_ptr_wrapper_factory::casted::template 
                wrap<Flags, TMemberFnPtr>(data.ptr);
        }
    };

    template<typename TMemberFnPtr, typename C>
    struct disambiguate<TMemberFnPtr, C, typename function_object_ptr_wrapper_factory::casted> {
        template<qualify_flags Flags, typename Invocation>
        static inline constexpr auto
            wrap_data(Invocation&& data) {
            return function_object_ptr_wrapper_factory::casted::template 
                wrap<Flags, TMemberFnPtr>(data.object_ptr);
        }
    };

    template<typename TMemberFnPtr, typename C>
    struct disambiguate<TMemberFnPtr, C, typename function_object_wrapper_factory::ambiguous> {
        template<qualify_flags Flags, typename Invocation>
        static inline constexpr auto
            wrap_data(Invocation&& data) {
            return function_object_wrapper_factory::casted::template 
                wrap<Flags, TMemberFnPtr>(data);
        }
    };

    template<typename TMemberFnPtr, typename C>
    struct disambiguate<TMemberFnPtr, C, typename function_object_wrapper_factory::casted> {
        template<qualify_flags Flags, typename Invocation>
        static inline constexpr auto
            wrap_data(Invocation&& data) {
            return function_object_wrapper_factory::casted::template 
                wrap<Flags, TMemberFnPtr>(data.object);
        }
    };

    template<typename Bad>
    struct harden_t {
        static_assert(sizeof(Bad) < 0, "Not a valid function type.");
    };

    /*
    using the preprocessor to spam template specializations of all 
    CV permutations - there is no other way
    */
        
//todo utilize clbl::pmf::define to speed up compile times by avoiding redundant specializations

#define CLBL_SPECIALIZE_HARDEN_T(cv_requested) \
    template<typename Return, typename... Args> \
        struct harden_t<Return(Args...) cv_requested> { \
            template<typename Callable> \
            inline constexpr auto \
            operator()(Callable&& c) const { \
                constexpr qualify_flags requested = cv_of<cv_requested dummy>; \
                constexpr qualify_flags present = cv_of<Callable>; \
                using C = no_ref<Callable>; \
                using underlying_type = typename C::underlying_type; \
                using return_type = std::conditional_t<std::is_same<Return, auto_>::value, \
                                    decltype(harden_cast<(requested | present)>(static_cast<Callable&&>(c))(std::declval<Args>()...)), \
                                    Return>; \
                using abominable_fn_type = return_type(Args...) cv_requested; \
                using requested_pmf_type = abominable_fn_type underlying_type::*; \
                using disambiguator = disambiguate<requested_pmf_type, C, typename C::creator>; \
                return disambiguator::template wrap_data<requested | present>(c.data); \
            } \
    }

    //TODO use clbl::pmf::define for ref qualifiers

    CLBL_SPECIALIZE_HARDEN_T(CLBL_NO_CV);
    CLBL_SPECIALIZE_HARDEN_T(const);
    CLBL_SPECIALIZE_HARDEN_T(volatile);
    CLBL_SPECIALIZE_HARDEN_T(const volatile);

    template<typename T>
    constexpr harden_t<T> harden_v{};
}

//! @addtogroup cv-correctness CV Correctness - full example
//! Example
//! -------
//! @include example/cv_correctness.cpp


//! @addtogroup harden Disambiguating operator()
//! `clbl::harden` lets you explicitly select a single overload of
//! an object's `operator()`, while hiding all other overloads. 
//! Example
//! -------
//! @include example/harden.cpp

//! clbl::harden lets you explicitly disambiguate overloads of `operator()`
//! by passing an abominable function type as a template argument.
template<typename AbominableFunctionType, typename Callable>
inline constexpr auto harden(Callable&& c) {
    return detail::harden_v<AbominableFunctionType>(static_cast<Callable&&>(c));
}

template<typename Callable, std::enable_if_t<!no_ref<Callable>::is_ambiguous, dummy>* = nullptr >
inline constexpr auto harden(Callable&& c) {
    return detail::harden_v<typename no_ref<Callable>::type>(static_cast<Callable&&>(c));
}

template<typename Callable, std::enable_if_t<no_ref<Callable>::is_ambiguous, qualify_flags> QFlags = qflags::default_>
inline constexpr auto harden(Callable&& c) {
    return no_ref<Callable>::creator::template wrap_data<QFlags | cv_of<no_ref<Callable> > >(c.data);
}

template<qualify_flags QFlags, typename Callable, std::enable_if_t<no_ref<Callable>::is_ambiguous, dummy>* = nullptr >
inline constexpr auto harden(Callable&& c) {
    return no_ref<Callable>::creator::template wrap_data<QFlags | cv_of<no_ref<Callable> > >(c.data);
}
}

#endif