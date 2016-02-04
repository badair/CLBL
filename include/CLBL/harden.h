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

    //todo forward invocation_data instead of its by-value members

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

template<typename Abominable>
struct harden_t {
    using dummy_mf = pmf<Abominable dummy::*>;
    using is_auto = std::is_same<typename dummy_mf::return_type, auto_>;

    template<typename Callable>
    inline constexpr auto
    operator()(Callable&& c) const {
        constexpr qualify_flags requested = dummy_mf::q_flags;
        constexpr qualify_flags present =
                    cv_of<Callable> |(ref_of<Callable> & dummy_mf::ref_flags);

        constexpr qualify_flags resolved_flags = 
                    qflags::collapse_reference<
                        present | qflags::remove_reference<requested>,
                        qflags::remove_cv<requested> >;

        using C = no_ref<Callable>;
        using underlying_type = typename C::underlying_type;

        using actual_return_type = 
        std::conditional_t<
            is_auto::value,
            decltype(dummy_mf::unevaluated_invoke_with_args_declval(
                harden_cast<resolved_flags>(static_cast<Callable&&>(c))
            )),
            typename dummy_mf::return_type
        >;

        using actual_dummy_mf = pmf<typename dummy_mf::template
                apply_return<actual_return_type> >;

        using requested_pmf_type = typename actual_dummy_mf::template
                apply_class<underlying_type>;

        using disambiguator = 
                disambiguate<requested_pmf_type, C, typename C::creator>;

        return disambiguator::template
                wrap_data<requested | present>(c.data);
    }
};

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