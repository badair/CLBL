/*!
@file
Defines `clbl::slim_member_function_bound_to_object_ptr_wrapper`.

@copyright Barrett Adair 2015
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)

*/

#ifndef CLBL_SLIM_MEMBER_FUNCTION_BOUND_TO_OBJECT_PTR_WRAPPER_H
#define CLBL_SLIM_MEMBER_FUNCTION_BOUND_TO_OBJECT_PTR_WRAPPER_H

#include <type_traits>
#include <tuple>
#include <utility>

#include <CLBL/tags.h>
#include <CLBL/pmf.h>
#include <CLBL/type_traits.h>
#include <CLBL/cv_of.h>
#include <CLBL/forward.h>
#include <CLBL/harden_cast.h>

namespace clbl { namespace internal {

    /*!
    Wraps a PMF and a pointer to an object with which to call it.
    It is identical to pmf_ptr_wrapper except that the PMF is not stored
    in an object instance, but passed as a template arg instead, thus
    giving it a "slim" profile in memory.
    */

    template<
        typename Creator,
        qualify_flags QFlags,
        typename UnderlyingType,
        typename TPtr,
        typename Pmf,
        Pmf Value>
    struct slim_member_function_bound_to_object_ptr_wrapper {
        
        using mf = pmf<Pmf>;

        using decay_type = typename mf::decay_type;
        using return_type = typename mf::return_type;

        using arg_types = typename mf::template unpack_args<std::tuple>;
        using clbl_tag = pmf_ptr_tag;
        using creator = Creator;
        using forwarding_glue = typename mf::forwarding_glue;

        struct invocation_data_type {
            static constexpr auto pmf = Value;
            TPtr object_ptr;
        };

        using this_t = slim_member_function_bound_to_object_ptr_wrapper<
                Creator, QFlags, UnderlyingType, TPtr, Pmf, Value>;

        using type = typename mf::decay_to_function;
        using underlying_type = UnderlyingType;

        template<qualify_flags Flags>
        using add_qualifiers = slim_member_function_bound_to_object_ptr_wrapper<
                Creator, QFlags | Flags, UnderlyingType, TPtr, Pmf, Value>;

        static constexpr auto q_flags = QFlags;
        static constexpr auto is_ambiguous = false;

        invocation_data_type data;

        template<typename... Fargs>
        inline CLBL_CXX14_CONSTEXPR decltype(auto)
        operator()(Fargs&&... a) {
            return (harden_cast<q_flags>(
                *data.object_ptr).*invocation_data_type::pmf)(static_cast<Fargs&&>(a)...);
        }

        template<typename... Fargs>
        inline constexpr decltype(auto)
        operator()(Fargs&&... a) const {
            return (harden_cast<qflags::const_ | q_flags>(
                *data.object_ptr).*invocation_data_type::pmf)(static_cast<Fargs&&>(a)...);
        }

        template<typename... Fargs>
        inline CLBL_CXX14_CONSTEXPR decltype(auto)
        operator()(Fargs&&... a) volatile {
            return (harden_cast<qflags::volatile_ | q_flags>(
                *data.object_ptr).*invocation_data_type::pmf)(static_cast<Fargs&&>(a)...);
        }

        template<typename... Fargs>
        inline constexpr decltype(auto)
        operator()(Fargs&&... a) const volatile {
            return (harden_cast<qflags::const_ | qflags::volatile_ | q_flags>(
                *data.object_ptr).*invocation_data_type::pmf)(static_cast<Fargs&&>(a)...);
        }

        static inline constexpr auto
        copy_invocation(this_t& c) {
            return c;
        }

        static inline constexpr auto
        copy_invocation(const this_t& c) {
            return add_qualifiers<qflags::const_>{c};
        }

        static inline constexpr auto
        copy_invocation(volatile this_t& c) {
            return add_qualifiers<qflags::volatile_>{c};
        }

        static inline constexpr auto
        copy_invocation(const volatile this_t& c) {
            return add_qualifiers<qflags::const_ | qflags::volatile_>{c};
        }
    };
}}

#endif