/*!
@file
Defines `clbl::unbound_universal_pmf_wrapper`.

@copyright Barrett Adair 2015
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)

*/

#ifndef CLBL_UNBOUND_UNIVERSAL_PMF_WRAPPER_H
#define CLBL_UNBOUND_UNIVERSAL_PMF_WRAPPER_H

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

    //! wraps a PMF and takes a corresponding perfectly-forwarded object as the first argument.
    template<typename, typename, typename Failure>
    struct unbound_universal_pmf_wrapper {static_assert(sizeof(Failure) < 0, "Not a member function."); };

    template<typename Creator, typename TMemberFnPtr,
    typename T, typename Return, typename... Args>
    struct unbound_universal_pmf_wrapper<Creator, TMemberFnPtr, Return(T::*)(Args...)> {

        using decayed_member_fn_ptr = Return(T::*)(Args...);

        static constexpr auto cv_flags = default_;
        using underlying_type = universal_reference<T>;

        using arg_types = std::tuple<underlying_type, Args...>;
        using clbl_tag = pmf_tag;
        using creator = Creator;
        using forwarding_glue = Return(underlying_type, forward<Args>...);
        using invocation_data_type = const no_ref<TMemberFnPtr>;
        using my_type = unbound_universal_pmf_wrapper<Creator, TMemberFnPtr, decayed_member_fn_ptr>;
        using return_type = Return;
        using type = Return(underlying_type, Args...);
        

        template<qualify_flags Flags>
        using apply_cv = my_type;

        
        static constexpr auto is_ambiguous = false;

        invocation_data_type data;

        inline unbound_universal_pmf_wrapper(const TMemberFnPtr& f_ptr)
            : data{ f_ptr }
        {}

        inline unbound_universal_pmf_wrapper(const my_type& other) = default;
        inline unbound_universal_pmf_wrapper(my_type&& other) = default;

        inline unbound_universal_pmf_wrapper(const volatile my_type& other)
            : data(other.data)
        {}

        template<typename U, typename... Fargs>
        inline Return operator()(U&& object, Fargs&&... a) const {
            return ((object).*data)(a...);
        }

        template<typename U, typename... Fargs>
        inline Return operator()(U&& object, Fargs&&... a) const volatile {
            return ((object).*data)(a...);
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