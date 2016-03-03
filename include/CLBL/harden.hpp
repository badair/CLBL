/*!
@file
Defines `clbl::harden`.

@copyright Barrett Adair 2015
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)

*/

#ifndef CLBL_HARDEN_H
#define CLBL_HARDEN_H

#include <functional>
#include <utility>

#include <CLBL/tags.hpp>
#include <CLBL/constraints.hpp>
#include <quali/quali.hpp>
#include <CLBL/fwrap.hpp>
namespace clbl {

namespace detail {

template<typename Abominable>
struct harden_t {
    using dummy_mf = pmf<Abominable dummy::*>;
    using is_auto = std::is_same<typename dummy_mf::return_type, auto_>;
    static constexpr const auto requested = dummy_mf::q_flags;

    //callable_wrapper<function_object_wrapper_base<cv_of<T>::value, Generalized>>
    //callable_wrapper<ambiguous_function_object_wrapper_base<cv_of<T>::value, Generalized>>
    template<typename Callable>
    inline constexpr auto
    operator()(Callable&& c) const {
        
        using  present = std::integral_constant<quali::flags,
            quali::cv_of<Callable>::value | (quali::ref_of<Callable>::value & dummy_mf::ref_flags)
        >;

        using resolved_flags = std::integral_constant<quali::flags,
            quali::collapse_reference<
                present::value | quali::remove_reference<requested>::value,
                quali::remove_cv<requested>::value
            >::value
        >;

        using C = no_ref<Callable>;
        using underlying_type = no_ref<typename C::underlying_type>;

        using actual_return_type = 
        std::conditional_t<
            is_auto::value,
            decltype(dummy_mf::unevaluated_invoke_with_args_declval(
                quali::cast<resolved_flags::value>(static_cast<Callable&&>(c))
            )),
            typename dummy_mf::return_type
        >;

        using actual_dummy_mf = pmf<typename dummy_mf::template
                apply_return<actual_return_type> >;

        using requested_pmf_type = typename actual_dummy_mf::template
                apply_class<underlying_type>;

        using generalized_type = typename C::generalized_object;
        using data_type = typename generalized_type::original_type;

        return internal::callable_wrapper<
            internal::function_object_wrapper_base<
                requested | present::value,
                quali::qualified_type<
                    quali::generalized_object<
                        quali::qualified_type<data_type, requested | present::value>
                    >,
                    requested | present::value
                >,
                requested_pmf_type
            >
        >::transform(static_cast<Callable&&>(c));

        /*
        return internal::callable_wrapper<
            internal::function_object_wrapper_base<
                requested | present::value,
                qualified_type<
                    quali::generalized_object<
                        qualified_type<data_type, requested | present::value>,
                    >,
                    requested | present::value
                >,
                requested_pmf_type
            >
        >{c.data};*/
    }
};

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
template<
    typename AbominableFunctionType,
    typename Callable,
    CLBL_REQUIRES_(
        std::is_same<typename no_ref<Callable>::clbl_tag, function_object_wrapper_tag>::value
    )
>
inline constexpr decltype(auto)
harden(Callable&& c) {
    return detail::harden_t<AbominableFunctionType>{}(static_cast<Callable&&>(c));
}

template<
    typename TODO,
    typename Callable,
    CLBL_REQUIRES_(
        !std::is_same<typename no_ref<Callable>::clbl_tag, function_object_wrapper_tag>::value
    )
>
inline constexpr decltype(auto)
harden(Callable&& c) {
    return quali::cast(static_cast<Callable&&>(c));
}

template<
    typename Callable,
    CLBL_REQUIRES_(!no_ref<Callable>::is_ambiguous)
>
inline constexpr decltype(auto)
harden(Callable&& c) {
    return quali::cast(static_cast<Callable&&>(c));
}

template<
    typename Callable,
    std::enable_if_t<no_ref<Callable>::is_ambiguous, quali::flags> QFlags = quali::default_
>
inline constexpr decltype(auto)
harden(Callable&& c) {
    return quali::cast(static_cast<Callable&&>(c));
}

}

#endif