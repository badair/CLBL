/*!
@file
Defines `clbl::universal_member_function_wrapper`.

@copyright Barrett Adair 2015
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)

*/

#ifndef CLBL_UNIVERSAL_MEMBER_FUNCTION_WRAPPER_H
#define CLBL_UNIVERSAL_MEMBER_FUNCTION_WRAPPER_H

#include <type_traits>
#include <tuple>
#include <utility>

#include <CLBL/cv.h>
#include <CLBL/tags.h>
#include <CLBL/forward.h>

namespace clbl {

    //! wraps a PMF and takes a corresponding perfectly-forwarded object as the first argument.
    template<typename, typename, typename Failure>
    struct universal_member_function_wrapper {
        static_assert(sizeof(Failure) < 0, "Not a member function.");
    };

    template<typename Creator, typename TMemberFnPtr,
    typename T, typename Return, typename... Args>
    struct universal_member_function_wrapper<Creator, TMemberFnPtr, Return(T::*)(Args...)> {

        using decayed_member_fn_ptr = Return(T::*)(Args...);

        static constexpr auto q_flags = qflags::default_;
        using underlying_type = universal_reference<T>;

        using arg_types = std::tuple<underlying_type, Args...>;
        using clbl_tag = pmf_tag;
        using creator = Creator;
        using forwarding_glue = Return(underlying_type, forward<Args>...);
        using invocation_data_type = const no_ref<TMemberFnPtr>;
        using this_t = universal_member_function_wrapper<Creator, TMemberFnPtr, decayed_member_fn_ptr>;
        using return_type = Return;
        using type = Return(underlying_type, Args...);
        
        template<qualify_flags Flags>
        using add_qualifiers = this_t;

        static constexpr auto is_ambiguous = false;

        invocation_data_type data;

        inline
        universal_member_function_wrapper(const TMemberFnPtr& f_ptr)
            : data{ f_ptr } {}

        inline
        universal_member_function_wrapper(const this_t& other) = default;

        inline
        universal_member_function_wrapper(this_t&& other) = default;

        inline
        universal_member_function_wrapper(const volatile this_t& other)
            : data(other.data) {}

        template<typename U, typename... Fargs>
        inline Return
        operator()(U&& object, Fargs&&... a) const {
            return ((object).*data)(a...);
        }

        template<typename U, typename... Fargs>
        inline Return
        operator()(U&& object, Fargs&&... a) const volatile {
            return ((object).*data)(a...);
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