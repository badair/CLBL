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

#include <CLBL/tags.h>
#include <CLBL/qualify_flags.h>
#include <CLBL/fwrap.h>
#include <CLBL/harden_cast.h>

namespace clbl {

    /*
    clbl::harden is used to disambiguate overloads of operator() in a CLBL wrapper.
    */

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
        wrap_data functions and pass the requested disambiguation, which results
        in a static_cast on operator() - unfortunately, the current C++ standard 
        forbids casting non-type function pointer template arguments, so we just 
        pass the requested type all the way down to the invocation data, where 
        the cast is performed into a static constexpr member (see casted_* structs 
        defined in CLBL/invocation_data.h)
        */
        template<typename TMemberFnPtr, typename C>
        struct disambiguate<TMemberFnPtr, C, typename pointer_to_function_object::ambiguous> {
            template<qualify_flags Flags, typename Invocation>
            static inline constexpr auto
            wrap_data(Invocation&& data) {
                return pointer_to_function_object::casted::template 
                    wrap<Flags, TMemberFnPtr>(data.ptr);
            }
        };

        template<typename TMemberFnPtr, typename C>
        struct disambiguate<TMemberFnPtr, C, typename pointer_to_function_object::casted> {
            template<qualify_flags Flags, typename Invocation>
            static inline constexpr auto
                wrap_data(Invocation&& data) {
                return pointer_to_function_object::casted::template 
                    wrap<Flags, TMemberFnPtr>(data.object_ptr);
            }
        };

        template<typename TMemberFnPtr, typename C>
        struct disambiguate<TMemberFnPtr, C, typename function_object::ambiguous> {
            template<qualify_flags Flags, typename Invocation>
            static inline constexpr auto
                wrap_data(Invocation&& data) {
                return function_object::casted::template 
                    wrap<Flags, TMemberFnPtr>(data.object);
            }
        };

        template<typename TMemberFnPtr, typename C>
        struct disambiguate<TMemberFnPtr, C, typename function_object::casted> {
            template<qualify_flags Flags, typename Invocation>
            static inline constexpr auto
                wrap_data(Invocation&& data) {
                return function_object::casted::template 
                    wrap<Flags, TMemberFnPtr>(data.object);
            }
        };

        template<typename Bad>
        struct harden_t {
            static_assert(sizeof(Bad) < 0, "Not a valid function type.");
        };

        /*
        Chainsawing the Gordian knot by spamming CV permutations with the preprocessor
        */

#define __CLBL_DEFINE_HARDEN_T_OVERLOADS(cv_requested, cv_present) \
        template<typename Callable> \
        inline constexpr auto \
        operator()(cv_present Callable& c) const { \
            constexpr qualify_flags requested = cv<cv_requested dummy>; \
            constexpr qualify_flags present = cv<cv_present dummy>; \
            using C = no_ref<Callable>; \
            using underlying_type = typename C::underlying_type; \
            using return_type = std::conditional_t<std::is_same<Return, auto_>::value, \
                                decltype(harden_cast<(requested | present)>(c)(std::declval<Args>()...)), \
                                Return>; \
            using abominable_fn_type = return_type(Args...) cv_requested; \
            using requested_pmf_type = abominable_fn_type underlying_type::*; \
            using disambiguator = disambiguate<requested_pmf_type, C, typename C::creator>; \
            return disambiguator::template wrap_data<requested | present>(c.data); \
        }
            
#define __CLBL_SPECIALIZE_HARDEN_T(cv_requested) \
        template<typename Return, typename... Args> \
            struct harden_t<Return(Args...) cv_requested> { \
            __CLBL_DEFINE_HARDEN_T_OVERLOADS(cv_requested, __CLBL_NO_CV) \
            __CLBL_DEFINE_HARDEN_T_OVERLOADS(cv_requested, const) \
            __CLBL_DEFINE_HARDEN_T_OVERLOADS(cv_requested, volatile) \
            __CLBL_DEFINE_HARDEN_T_OVERLOADS(cv_requested, const volatile) \
        }

        //ellipses and ref qualifiers not yet implemented...

        __CLBL_SPECIALIZE_HARDEN_T(__CLBL_NO_CV);
        __CLBL_SPECIALIZE_HARDEN_T(const);
        __CLBL_SPECIALIZE_HARDEN_T(volatile);
        __CLBL_SPECIALIZE_HARDEN_T(const volatile);

        template<typename T>
        constexpr harden_t<T> harden_v{};
    }

    template<typename Callable>
    inline constexpr auto harden(Callable&& c) {
        return detail::harden_v<typename no_ref<Callable>::type>(std::forward<Callable>(c));
    }

    template<typename FunctionType, typename Callable>
    inline constexpr auto harden(Callable&& c) {
        return detail::harden_v<FunctionType>(std::forward<Callable>(c));
    }
}

#endif