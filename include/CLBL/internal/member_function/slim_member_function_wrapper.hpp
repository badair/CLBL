/*!
@file
Defines `clbl::slim_member_function_wrapper`.

@copyright Barrett Adair 2015
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)

*/

#ifndef CLBL_SLIM_MEMBER_FUNCTION_WRAPPER_H
#define CLBL_SLIM_MEMBER_FUNCTION_WRAPPER_H

#include <type_traits>
#include <tuple>
#include <utility>

#include <CLBL/cv_of.hpp>
#include <CLBL/pmf.hpp>
#include <CLBL/tags.hpp>
#include <CLBL/forward.hpp>
#include <CLBL/harden_cast.hpp>

namespace clbl { namespace internal {

    template<typename Creator, typename Pmf, Pmf Value>
    struct slim_member_function_wrapper {

        using mf = pmf<no_const_no_ref<Pmf> >;
        using return_type = typename mf::return_type;
        using class_type = typename mf::class_type;

        static constexpr auto q_flags = mf::q_flags;
        using underlying_type = typename mf::invoke_type;

        using arg_types = typename mf::template prepend_args<std::tuple, underlying_type>;
        using clbl_tag = pmf_tag;
        using creator = Creator;
        using forwarding_glue = typename mf::template prepend_args_to_forward_function<underlying_type>;
        using this_t = slim_member_function_wrapper<Creator, Pmf, Value>;
        using type = typename mf::template prepend_args_to_function<underlying_type>;
        
        template<qualify_flags Flags>
        using add_qualifiers = this_t;

        struct invocation_data_type {
            using type = Pmf;
            static constexpr auto ptr = Value;
        };

        static constexpr auto is_ambiguous = false;
        static constexpr auto data = invocation_data_type{};

        template<typename... Fargs>
        inline constexpr decltype(auto)
        operator()(underlying_type object, Fargs&&... a) const {
            return (object.*Value)(static_cast<Fargs&&>(a)...);
        }
        
        template<typename... Fargs>
        inline constexpr decltype(auto)
        operator()(underlying_type object, Fargs&&... a) const volatile {
            return (object.*Value)(static_cast<Fargs&&>(a)...);
        }

        template<typename P, typename... Fargs>
        inline constexpr decltype(auto)
        operator()(ConvertiblePointer<underlying_type, P&&> p, Fargs&&... a) const {
            return ((*p).*Value)(static_cast<Fargs&&>(a)...);
        }
        
        template<typename P, typename... Fargs>
        inline constexpr decltype(auto)
        operator()(ConvertiblePointer<underlying_type, P&&> p, Fargs&&... a) const volatile {
            return ((*p).*Value)(static_cast<Fargs&&>(a)...);
        }

        static inline constexpr auto
        copy_invocation(const this_t& c) {
            return c;
        }

        static inline constexpr auto
        copy_invocation(const volatile this_t& c) {
            return c;
        }

        template<typename... Fargs>
        decltype(auto) bind(Fargs... args) {
            using list_type = typename detail::wrapped_args_tuple<no_ref<Fargs>...>::type;
            return detail::binding_wrapper<this_t, list_type>{*this, list_type{ args... }};
        }
    };
}}

#endif