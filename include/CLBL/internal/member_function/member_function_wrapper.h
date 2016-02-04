/*!
@file
Defines `clbl::member_function_wrapper`.

@copyright Barrett Adair 2015
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)

*/

#ifndef CLBL_MEMBER_FUNCTION_WRAPPER_H
#define CLBL_MEMBER_FUNCTION_WRAPPER_H

#include <type_traits>
#include <tuple>
#include <utility>

#include <CLBL/cv_of.h>
#include <CLBL/pmf.h>
#include <CLBL/tags.h>
#include <CLBL/forward.h>
#include <CLBL/harden_cast.h>
#include <CLBL/constraints.h>

namespace clbl { namespace internal {

    template<typename Creator, typename Pmf>
    struct member_function_wrapper {

        using mf = pmf<Pmf>;
        using return_type = typename mf::return_type;
        using class_type = typename mf::class_type;

        static constexpr auto q_flags = mf::q_flags;
        using underlying_type = typename mf::invoke_type;

        using arg_types = typename mf::template prepend_args<std::tuple, underlying_type>;
        using clbl_tag = pmf_tag;
        using creator = Creator;
        using forwarding_glue = typename mf::template prepend_args_to_forward_function<underlying_type>;
        using invocation_data_type = const no_ref<Pmf>;
        using this_t = member_function_wrapper<Creator, Pmf>;
        using type = typename mf::template prepend_args_to_function<underlying_type>;
        
        template<qualify_flags Ignored>
        using add_qualifiers = member_function_wrapper<Creator, Pmf>;

        static constexpr auto is_ambiguous = false;

        invocation_data_type data;

        inline constexpr
        member_function_wrapper(const Pmf& f_ptr)
            : data{ f_ptr } {}

        inline constexpr
        member_function_wrapper(const this_t& other) = default;

        inline constexpr
        member_function_wrapper(this_t&& other) = default;

        inline constexpr
        member_function_wrapper(const volatile this_t& other)
            : data(other.data) {}

        template<typename... Fargs>
        inline constexpr decltype(auto)
        operator()(underlying_type object, Fargs&&... a) const {
            return (object.*data)(static_cast<Fargs&&>(a)...);
        }
        
        template<typename... Fargs>
        inline constexpr decltype(auto)
        operator()(underlying_type object, Fargs&&... a) const volatile {
            return (object.*data)(static_cast<Fargs&&>(a)...);
        }

        template<typename P, typename... Fargs>
        inline constexpr decltype(auto)
        operator()(ConvertiblePointer<underlying_type, P&&> p, Fargs&&... a) const {
            return ((*p).*data)(static_cast<Fargs&&>(a)...);
        }
        
        template<typename P, typename... Fargs>
        inline constexpr decltype(auto)
        operator()(ConvertiblePointer<underlying_type, P&&> p, Fargs&&... a) const volatile {
            return ((*p).*data)(static_cast<Fargs&&>(a)...);
        }

        static inline constexpr auto
        copy_invocation(const this_t& c) {
            return c;
        }

        static inline constexpr auto
        copy_invocation(const volatile this_t& c) {
            return c;
        }
    };
}}

#endif