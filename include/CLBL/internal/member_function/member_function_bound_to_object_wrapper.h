/*!
@file
Defines `clbl::member_function_with_object`.

@copyright Barrett Adair 2015
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)

*/

#ifndef CLBL_MEMBER_FUNCTION_BOUND_TO_OBJECT_WRAPPER_H
#define CLBL_MEMBER_FUNCTION_BOUND_TO_OBJECT_WRAPPER_H

#include <CLBL/pmf.h>
#include <CLBL/constraints.h>
#include <CLBL/internal/member_function/member_function_bound_to_object_invocation_data.h>

namespace clbl { namespace internal {

//! wraps a PMF and an object with which to call it.
template<
    typename Creator,
    qualify_flags QFlags,
    typename T, 
    typename Pmf>
struct member_function_bound_to_object_wrapper {

    using mf = pmf<Pmf>;
    using arg_types = typename mf::template unpack_args<std::tuple>;
    using clbl_tag = pmf_tag;
    using creator = Creator;
    using forwarding_glue = typename mf::forwarding_glue;
    using underlying_type = qualified_type<T, QFlags>;

    using invocation_data_type = 
            member_function_bound_to_object_invocation_data<
                underlying_type, Pmf>;

    using this_t = member_function_bound_to_object_wrapper<
                        Creator, QFlags, T, Pmf>;

    using return_type = typename mf::return_type;
    using type = typename mf::decay_to_function;
    

    template<qualify_flags Flags>
    using add_qualifiers = member_function_bound_to_object_wrapper<
                        Creator, QFlags | Flags, T, Pmf>;

    static constexpr auto q_flags = QFlags;
    static constexpr auto is_ambiguous = false;

    invocation_data_type data;

    inline
    member_function_bound_to_object_wrapper(Pmf f_ptr, T&& o)
        : data{ f_ptr, std::move(o) }
    {}

    inline
    member_function_bound_to_object_wrapper(Pmf f_ptr, no_const_no_ref<T>& o)
        : data{ f_ptr, o }
    {}

    inline
    member_function_bound_to_object_wrapper(Pmf f_ptr, const no_const_no_ref<T>& o)
        : data{ f_ptr, o } {}

    inline
    member_function_bound_to_object_wrapper(this_t& other) = default;

    inline
    member_function_bound_to_object_wrapper(const this_t& other) = default;

    inline
    member_function_bound_to_object_wrapper(this_t&& other) = default;

    inline
    member_function_bound_to_object_wrapper(volatile this_t& other)
        : data(other.data) {}

    inline
    member_function_bound_to_object_wrapper(const volatile this_t& other)
        : data(other.data) {}

    template<typename... Fargs>
    inline return_type
    operator()(Fargs&&... a) {
        return (harden_cast<q_flags>(data.object)
                 .*data.pmf)(static_cast<Fargs&&>(a)...);
    }

    template<typename... Fargs>
    inline return_type
    operator()(Fargs&&... a) const {
        return (harden_cast<qflags::const_ | q_flags>(data.object)
                 .*data.pmf)(static_cast<Fargs&&>(a)...);
    }

    template<typename... Fargs>
    inline return_type
    operator()(Fargs&&... a) volatile {
        return (harden_cast<qflags::volatile_ | q_flags>(data.object)
                 .*data.pmf)(static_cast<Fargs&&>(a)...);
    }

    template<typename... Fargs>
    inline return_type
    operator()(Fargs&&... a) const volatile {
        return (harden_cast<qflags::const_ | qflags::volatile_ | q_flags>(data.object)
                 .*data.pmf)(static_cast<Fargs&&>(a)...);
    }

    template<
        typename U = underlying_type,
        CLBL_REQUIRES_(is_clbl<U>)
    >
    static inline constexpr auto
    copy_invocation(U& c) {
        return no_ref<decltype(c.data.object)>::copy_invocation(
            harden_cast<q_flags>(c.data.object)
        );
    }

    template<
        typename U = underlying_type,
        CLBL_REQUIRES_(is_clbl<U>)
    >
    static inline constexpr auto
    copy_invocation(const U& c) {
        return no_ref<decltype(c.data.object)>::copy_invocation(
            harden_cast<qflags::const_ | q_flags>(c.data.object)
        );
    }

    template<
        typename U = underlying_type,
        CLBL_REQUIRES_(is_clbl<U>)
    >
    static inline constexpr auto
    copy_invocation(volatile U& c) {
        return no_ref<decltype(c.data.object)>::copy_invocation(
            harden_cast<qflags::volatile_ | q_flags>(c.data.object)
        );
    }

    template<
        typename U = underlying_type,
        CLBL_REQUIRES_(is_clbl<U>)
    >
    static inline constexpr auto
    copy_invocation(const volatile U& c) {
        return no_ref<decltype(data.object)>::copy_invocation(
            harden_cast<qflags::const_ | qflags::volatile_ | q_flags>(c.data.object)
        );
    }

    template<
        typename U = underlying_type,
        CLBL_REQUIRES_(!is_clbl<U>)
    >
    static inline constexpr auto
    copy_invocation(this_t& c) {
        return c;
    }

    template<
        typename U = underlying_type,
        CLBL_REQUIRES_(!is_clbl<U>)
    >
    static inline constexpr auto copy_invocation(const this_t& c) {
        return add_qualifiers<qflags::const_>{c.data.pmf, c.data.object};
    }

    template<
        typename U = underlying_type,
        CLBL_REQUIRES_(!is_clbl<U>)
    >
    static inline constexpr auto copy_invocation(volatile this_t& c) {
        return add_qualifiers<qflags::volatile_>{c.data.pmf, c.data.object};
    }

    template<
        typename U = underlying_type,
        CLBL_REQUIRES_(!is_clbl<U>)
    >
    static inline constexpr auto copy_invocation(const volatile this_t& c) {
        return add_qualifiers<qflags::const_ | qflags::volatile_>{
            c.data.pmf,
            c.data.object
        };
    }
};

}}

#endif