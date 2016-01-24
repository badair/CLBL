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
#include <CLBL/cv.h>
#include <CLBL/forward.h>
#include <CLBL/harden_cast.h>
#include <CLBL/internal/member_function/slim_member_function_bound_to_object_ptr_invocation_data.h>

namespace clbl {

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
        typename TMemberFnPtr,
        TMemberFnPtr Pmf>
    struct slim_member_function_bound_to_object_ptr_wrapper {
        
        using mf = pmf<TMemberFnPtr>;

        using decay_type = typename mf::decay_type;
        using return_type = typename mf::return_type;

        using arg_types = typename mf::template unpack_args<std::tuple>;
        using clbl_tag = pmf_ptr_tag;
        using creator = Creator;
        using forwarding_glue = typename mf::forwarding_glue;

        using invocation_data_type = 
                slim_member_function_bound_to_object_ptr_invocation_data<
                    TPtr, TMemberFnPtr, Pmf>;

        using this_t = slim_member_function_bound_to_object_ptr_wrapper<
                Creator, QFlags, UnderlyingType, TPtr, TMemberFnPtr, Pmf>;

        using type = typename mf::decay_to_function;
        using underlying_type = UnderlyingType;

        template<qualify_flags Flags>
        using add_qualifiers = slim_member_function_bound_to_object_ptr_wrapper<
                Creator, QFlags | Flags, UnderlyingType, TPtr, TMemberFnPtr, Pmf>;

        static constexpr auto q_flags = QFlags;
        static constexpr auto is_ambiguous = false;

        invocation_data_type data;

        slim_member_function_bound_to_object_ptr_wrapper(TPtr&& o_ptr)
            : data{ std::move(o_ptr) } {}

        inline
        slim_member_function_bound_to_object_ptr_wrapper(const TPtr& o_ptr)
            : data{ o_ptr } {}
        
        inline
        slim_member_function_bound_to_object_ptr_wrapper(TPtr& o_ptr)
            : data{ o_ptr } {}

        inline
        slim_member_function_bound_to_object_ptr_wrapper(this_t& other) = default;

        inline
        slim_member_function_bound_to_object_ptr_wrapper(const this_t& other) = default;

        inline
        slim_member_function_bound_to_object_ptr_wrapper(this_t&& other) = default;

        inline
        slim_member_function_bound_to_object_ptr_wrapper(const volatile this_t& other)
            : data { other.data } {}

        template<typename... Fargs>
        inline return_type
        operator()(Fargs&&... a) {
            return (harden_cast<q_flags>(
                *data.object_ptr).*invocation_data_type::pmf)(static_cast<Fargs&&>(a)...);
        }

        template<typename... Fargs>
        inline return_type
        operator()(Fargs&&... a) const {
            return (harden_cast<qflags::const_ | q_flags>(
                *data.object_ptr).*invocation_data_type::pmf)(static_cast<Fargs&&>(a)...);
        }

        template<typename... Fargs>
        inline return_type
        operator()(Fargs&&... a) volatile {
            return (harden_cast<qflags::volatile_ | q_flags>(
                *data.object_ptr).*invocation_data_type::pmf)(static_cast<Fargs&&>(a)...);
        }

        template<typename... Fargs>
        inline return_type
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
}

#endif