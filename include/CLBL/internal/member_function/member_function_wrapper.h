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

#include <CLBL/cv.h>
#include <CLBL/tags.h>
#include <CLBL/forward.h>
#include <CLBL/harden_cast.h>

namespace clbl {

    //! wraps a PMF and takes a corresponding object as the first argument.
    template<typename, qualify_flags,
        typename, typename Failure>
    struct member_function_wrapper {static_assert(sizeof(Failure) < 0, "Not a member function."); };

    template<typename Creator, qualify_flags QFlags, typename TMemberFnPtr,
    typename T, typename Return, typename... Args>
    struct member_function_wrapper<Creator, QFlags, TMemberFnPtr, Return(T::*)(Args...)> {

        using decayed_member_fn_ptr = Return(T::*)(Args...);

        static constexpr auto q_flags = QFlags;
        using underlying_type = qualified_type<T, q_flags>;

        using arg_types = std::tuple<underlying_type, Args...>;
        using clbl_tag = pmf_tag;
        using creator = Creator;
        using forwarding_glue = Return(underlying_type, forward<Args>...);
        using invocation_data_type = const no_ref<TMemberFnPtr>;
        using this_t = member_function_wrapper<Creator, QFlags, TMemberFnPtr, decayed_member_fn_ptr>;
        using return_type = Return;
        using type = Return(underlying_type, Args...);
        
        template<qualify_flags Flags>
        using add_qualifiers = member_function_wrapper<Creator, QFlags | Flags, TMemberFnPtr, decayed_member_fn_ptr>;

        static constexpr auto is_ambiguous = false;

        invocation_data_type data;

        inline
        member_function_wrapper(const TMemberFnPtr& f_ptr)
            : data{ f_ptr } {}

        inline
        member_function_wrapper(const this_t& other) = default;

        inline
        member_function_wrapper(this_t&& other) = default;

        inline
        member_function_wrapper(const volatile this_t& other)
            : data(other.data) {}

        template<qualify_flags Flags = q_flags, typename... Fargs>
        inline std::enable_if_t<Flags == qflags::default_, Return>
        operator()(underlying_type object, Fargs&&... a) const volatile {
            return (object.*data)(static_cast<Fargs&&>(a)...);
        }
        
        template<qualify_flags Flags = q_flags, typename... Fargs>
        inline std::enable_if_t<Flags !=  qflags::default_, Return> 
        operator()(underlying_type object, Fargs&&... a) const volatile {
            return (object.*data)(static_cast<Fargs&&>(a)...);
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
}

#endif