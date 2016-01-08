/*!
@file
Defines `clbl::unbound_pmf_wrapper`.

@copyright Barrett Adair 2015
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)

*/

#ifndef CLBL_UNBOUND_PMF_WRAPPER_H
#define CLBL_UNBOUND_PMF_WRAPPER_H

#include <type_traits>
#include <tuple>
#include <utility>

#include <CLBL/cv.h>
#include <CLBL/tags.h>
#include <CLBL/forward.h>
#include <CLBL/harden_cast.h>
#include <CLBL/invocation_macros.h>
#include <CLBL/invocation_data.h>

namespace clbl {

    //! wraps a PMF and takes a corresponding object as the first argument.
    template<typename, qualify_flags,
        typename, typename Failure>
    struct unbound_pmf_wrapper {static_assert(sizeof(Failure) < 0, "Not a member function."); };

    template<typename Creator, qualify_flags CvFlags, typename TMemberFnPtr,
    typename T, typename Return, typename... Args>
    struct unbound_pmf_wrapper<Creator, CvFlags, TMemberFnPtr, Return(T::*)(Args...)> {

        using decayed_member_fn_ptr = Return(T::*)(Args...);

        static constexpr auto cv_flags = default_reference<CvFlags>;
        using underlying_type = apply_qualifiers<T, cv_flags>;

        using arg_types = std::tuple<underlying_type, Args...>;
        using clbl_tag = pmf_tag;
        using creator = Creator;
        using forwarding_glue = Return(underlying_type, forward<Args>...);
        using invocation_data_type = const no_ref<TMemberFnPtr>;
        using my_type = unbound_pmf_wrapper<Creator, CvFlags, TMemberFnPtr, decayed_member_fn_ptr>;
        using return_type = Return;
        using type = Return(underlying_type, Args...);
        

        template<qualify_flags Flags>
        using apply_cv = unbound_pmf_wrapper<Creator, CvFlags | Flags, TMemberFnPtr, decayed_member_fn_ptr>;

        
        static constexpr auto is_ambiguous = false;

        invocation_data_type data;

        inline unbound_pmf_wrapper(const TMemberFnPtr& f_ptr)
            : data{ f_ptr }
        {}

        inline unbound_pmf_wrapper(const my_type& other) = default;
        inline unbound_pmf_wrapper(my_type&& other) = default;

        inline unbound_pmf_wrapper(const volatile my_type& other)
            : data(other.data)
        {}

        template<typename... Fargs>
        inline Return operator()(underlying_type object, Fargs&&... a) const {
            return CLBL_UPCAST_AND_CALL_MEMBER_VAL(__CLBL_NO_CV,
                object, data, std::forward<Fargs>(a)...);
        }

        template<typename... Fargs>
        inline Return operator()(underlying_type object, Fargs&&... a) const volatile {
            return CLBL_UPCAST_AND_CALL_MEMBER_VAL(__CLBL_NO_CV,
                object, data, std::forward<Fargs>(a)...);
        }

        static inline constexpr auto copy_invocation(const my_type& c) {
            return c;
        }

        static inline constexpr auto copy_invocation(const volatile my_type& c) {
            return c;
        }
    };
}

#endif