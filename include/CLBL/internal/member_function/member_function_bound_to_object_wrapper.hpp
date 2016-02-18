/*!
@file
Defines `clbl::member_function_with_object`.

@copyright Barrett Adair 2015
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)

*/

#ifndef CLBL_MEMBER_FUNCTION_BOUND_TO_OBJECT_WRAPPER_H
#define CLBL_MEMBER_FUNCTION_BOUND_TO_OBJECT_WRAPPER_H

#include <CLBL/pmf.hpp>
#include <CLBL/constraints.hpp>

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

    struct invocation_data_type {
        Pmf pmf;
        underlying_type object;
    };

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

    template<typename... Fargs>
    inline CLBL_CXX14_CONSTEXPR decltype(auto)
    operator()(Fargs&&... a) {
        return (harden_cast<q_flags>(data.object)
                 .*data.pmf)(static_cast<Fargs&&>(a)...);
    }

    template<typename... Fargs>
    inline constexpr decltype(auto)
    operator()(Fargs&&... a) const {
        return (harden_cast<qflags::const_ | q_flags>(data.object)
                 .*data.pmf)(static_cast<Fargs&&>(a)...);
    }

    template<typename... Fargs>
    inline CLBL_CXX14_CONSTEXPR decltype(auto)
    operator()(Fargs&&... a) volatile {
        return (harden_cast<qflags::volatile_ | q_flags>(data.object)
                 .*data.pmf)(static_cast<Fargs&&>(a)...);
    }

    template<typename... Fargs>
    inline constexpr decltype(auto)
    operator()(Fargs&&... a) const volatile {
        return (
            harden_cast<qflags::const_ | qflags::volatile_ | q_flags> (
                data.object
            ).*data.pmf
        )(static_cast<Fargs&&>(a)...);
    }

    template<
        typename U = underlying_type,
        CLBL_REQUIRES_(is_clbl<U>::value)
    >
    static inline constexpr auto
    copy_invocation(U& c) {
        return no_ref<decltype(c.data.object)>::copy_invocation(
            harden_cast<q_flags>(c.data.object)
        );
    }

    template<
        typename U = underlying_type,
        CLBL_REQUIRES_(is_clbl<U>::value)
    >
    static inline constexpr auto
    copy_invocation(const U& c) {
        return no_ref<decltype(c.data.object)>::copy_invocation(
            harden_cast<qflags::const_ | q_flags>(c.data.object)
        );
    }

    template<
        typename U = underlying_type,
        CLBL_REQUIRES_(is_clbl<U>::value)
    >
    static inline constexpr auto
    copy_invocation(volatile U& c) {
        return no_ref<decltype(c.data.object)>::copy_invocation(
            harden_cast<qflags::volatile_ | q_flags>(c.data.object)
        );
    }

    template<
        typename U = underlying_type,
        CLBL_REQUIRES_(is_clbl<U>::value)
    >
    static inline constexpr auto
    copy_invocation(const volatile U& c) {
        return no_ref<decltype(data.object)>::copy_invocation(
            harden_cast<qflags::const_ | qflags::volatile_ | q_flags>(c.data.object)
        );
    }

    template<
        typename U = underlying_type,
        CLBL_REQUIRES_(!is_clbl<U>::value)
    >
    static inline constexpr auto
    copy_invocation(this_t& c) {
        return c;
    }

    template<
        typename U = underlying_type,
        CLBL_REQUIRES_(!is_clbl<U>::value)
    >
    static inline constexpr auto copy_invocation(const this_t& c) {
        return add_qualifiers<qflags::const_>{{c.data.pmf, c.data.object}};
    }

    template<
        typename U = underlying_type,
        CLBL_REQUIRES_(!is_clbl<U>::value)
    >
    static inline constexpr auto copy_invocation(volatile this_t& c) {
        return add_qualifiers<qflags::volatile_>{{c.data.pmf, c.data.object}};
    }

    template<
        typename U = underlying_type,
        CLBL_REQUIRES_(!is_clbl<U>::value)
    >
    static inline constexpr auto copy_invocation(const volatile this_t& c) {
        return add_qualifiers<qflags::const_ | qflags::volatile_>{
            {c.data.pmf, c.data.object}
        };
    }

    template<typename... Fargs>
    decltype(auto) bind(Fargs... args) {
        using list_type = typename detail::wrapped_args_tuple<no_ref<Fargs>...>::type;
        return detail::binding_wrapper<this_t, list_type>{*this, list_type{ args... }};
    }
};

}}

#endif