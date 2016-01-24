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
#include <CLBL/pmf.h>
#include <CLBL/tags.h>
#include <CLBL/forward.h>

namespace clbl {

    //! wraps a PMF and takes a corresponding perfectly-forwarded object as the first argument.

    template<typename Creator, typename TMemberFnPtr>
    struct universal_member_function_wrapper {

        using mf = pmf<TMemberFnPtr>;
        using class_type = typename mf::class_type;
        static constexpr auto q_flags = qflags::default_;
        using underlying_type = universal_reference<class_type>;

        using arg_types = typename mf::template prepend_args<std::tuple, underlying_type>;
        using clbl_tag = pmf_tag;
        using creator = Creator;
        using forwarding_glue = typename mf::template prepend_arg_to_forward_function<underlying_type>;
        using invocation_data_type = const no_ref<TMemberFnPtr>;
        using this_t = universal_member_function_wrapper<Creator, TMemberFnPtr>;
        using return_type = typename mf::return_type;
        using type = typename mf::decay_to_function;
        
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
        inline return_type
        operator()(U&& object, Fargs&&... a) const {
            return ((object).*data)(a...);
        }

        template<typename U, typename... Fargs>
        inline return_type
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