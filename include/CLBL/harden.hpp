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

    template<typename Callable>
    inline constexpr decltype(auto)
    operator()(Callable&& c) const {
        
        using present = std::integral_constant<quali::flags,
            quali::cv_of<no_ref<Callable>>{} 
            | quali::ref_of<Callable>{}
        >;

        using resolved_flags = std::integral_constant<quali::flags,
            quali::collapse<
                quali::remove_cv<requested>::value,
                present::value | quali::remove_reference<requested>::value
            >::value
        >;

        using C = no_ref<Callable>;
        using underlying_type = no_ref<typename C::underlying_type>;
        using hardened_callable = typename C::template add_qualifiers<resolved_flags::value>;

        using actual_return_type = std::conditional_t<
            is_auto{},
            typename dummy_mf::template result_of_invoke_with_args<hardened_callable>,
            typename dummy_mf::return_type
        >;

        using actual_dummy_mf = pmf<typename dummy_mf::template
                apply_return<actual_return_type> >;

        using requested_pmf_type = pmf<typename actual_dummy_mf::template
                apply_class<underlying_type>
        >;

        using result_type = typename requested_pmf_type::template prepend_flags_and_unpack_args_to_template<
             hardened_callable::template apply_signature,
             resolved_flags::value
         >;

        return result_type{static_cast<Callable&&>(c).data};
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