/*!
@file
Defines `clbl::internal::member_function_bound_to_object_ptr_wrapper`.

@copyright Barrett Adair 2015
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)

*/

#ifndef CLBL_MEMBER_FUNCTION_BOUND_TO_OBJECT_PTR_WRAPPER_H
#define CLBL_MEMBER_FUNCTION_BOUND_TO_OBJECT_PTR_WRAPPER_H

#include <type_traits>
#include <tuple>
#include <utility>
#include <CLBL/cv_of.hpp>
#include <CLBL/pmf.hpp>
#include <CLBL/tags.hpp>
#include <CLBL/forward.hpp>
#include <CLBL/type_traits.hpp>
#include <CLBL/harden_cast.hpp>
#include <CLBL/constraints.hpp>

namespace clbl { namespace internal {

//!Wraps a PMF and a pointer to an object with which to call it.

template<typename Creator,
        qualify_flags QFlags,
        typename UnderlyingType,
        typename TPtr,
        typename Pmf>
struct member_function_bound_to_object_ptr_wrapper {
    
    using mf = pmf<Pmf>;

    using arg_types = typename mf::template unpack_args<std::tuple>;
    using clbl_tag = pmf_ptr_tag;
    using creator = Creator;
    using forwarding_glue = typename mf::forwarding_glue;

    struct invocation_data_type {
        Pmf pmf;
        TPtr object_ptr;
    };

    using this_t = member_function_bound_to_object_ptr_wrapper<
            Creator, QFlags, UnderlyingType, TPtr, Pmf>;

    using return_type = typename mf::return_type;
    using type = typename mf::decay_to_function;
    using underlying_type = UnderlyingType;

    template<qualify_flags Flags>
    using add_qualifiers = member_function_bound_to_object_ptr_wrapper<
            Creator, QFlags | Flags, UnderlyingType, TPtr, Pmf>;

    static constexpr auto q_flags = QFlags;
    static constexpr auto is_ambiguous = false;

    invocation_data_type data;

    template<typename... Fargs>
    inline CLBL_CXX14_CONSTEXPR return_type
    operator()(Fargs&&... a) {
        return (harden_cast<q_flags>(*data.object_ptr)
            .*data.pmf)(static_cast<Fargs&&>(a)...);
    }

    template<typename... Fargs>
    inline constexpr return_type
    operator()(Fargs&&... a) const {
        return (harden_cast<qflags::const_ | q_flags>(*data.object_ptr)
            .*data.pmf)(static_cast<Fargs&&>(a)...);
    }

    template<typename... Fargs>
    inline CLBL_CXX14_CONSTEXPR return_type
    operator()(Fargs&&... a) volatile {
        return (harden_cast<qflags::volatile_ | q_flags>(*data.object_ptr)
            .*data.pmf)(static_cast<Fargs&&>(a)...);
    }

    template<typename... Fargs>
    inline constexpr return_type
    operator()(Fargs&&... a) const volatile {
        return (harden_cast<qflags::const_ | qflags::volatile_ | q_flags>(*data.object_ptr)
            .*data.pmf)(static_cast<Fargs&&>(a)...);
    }

    template<
        typename T = UnderlyingType,
        CLBL_REQUIRES_(is_clbl<T>::value)
    >
    static inline constexpr auto 
    copy_invocation(T& c){
        return no_ref<decltype(*c.data.object_ptr)>::copy_invocation(
            harden_cast<q_flags>(*c.data.object_ptr)
        );
    }

    template<
        typename T = UnderlyingType,
        CLBL_REQUIRES_(is_clbl<T>::value)
    >
    static inline constexpr auto 
    copy_invocation(const T& c) {
        return no_ref<decltype(*c.data.object_ptr)>::copy_invocation(
            harden_cast<qflags::const_ | q_flags>(*c.data.object_ptr)
        );
    }

    template<
        typename T = UnderlyingType,
        CLBL_REQUIRES_(is_clbl<T>::value)
    >
    static inline constexpr auto
    copy_invocation(volatile T& c) {
        return no_ref<decltype(*c.data.object_ptr)>::copy_invocation(
            harden_cast<qflags::volatile_ | q_flags>(*c.data.object_ptr)
        );
    }

    template<
        typename T = UnderlyingType,
        CLBL_REQUIRES_(is_clbl<T>::value)
    >
    static inline constexpr auto
    copy_invocation(const volatile T& c) {
        return no_ref<decltype(*c.data.object_ptr)>::copy_invocation(
            harden_cast<
                qflags::const_ | qflags::volatile_ | q_flags
            >(*c.data.object_ptr)
        );
    }

    template<
        typename T = UnderlyingType,
        CLBL_REQUIRES_(!is_clbl<T>::value)
    >
    static inline constexpr auto
    copy_invocation(this_t& c) {
        return c;
    }

    template<
        typename T = UnderlyingType,
        CLBL_REQUIRES_(!is_clbl<T>::value)
    >
    static inline constexpr auto 
    copy_invocation(const this_t& c) {
        return add_qualifiers<qflags::const_>{
            c.data.pmf,
            c.data.object_ptr
        };
    }

    template<
        typename T = UnderlyingType,
        CLBL_REQUIRES_(!is_clbl<T>::value)
    >
    static inline constexpr auto
    copy_invocation(volatile this_t& c) {
        return add_qualifiers<qflags::volatile_>{
            c.data.pmf,
            c.data.object_ptr
        };
    }

    template<
        typename T = UnderlyingType,
        CLBL_REQUIRES_(!is_clbl<T>::value)
    >
    static inline constexpr auto
    copy_invocation(const volatile this_t& c) {
        return add_qualifiers<qflags::const_ | qflags::volatile_>{
            {c.data.pmf, c.data.object_ptr}
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