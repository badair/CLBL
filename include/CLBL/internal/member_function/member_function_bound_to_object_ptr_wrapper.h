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
#include <CLBL/cv.h>
#include <CLBL/pmf.h>
#include <CLBL/tags.h>
#include <CLBL/forward.h>
#include <CLBL/type_traits.h>
#include <CLBL/harden_cast.h>
#include <CLBL/internal/member_function/member_function_bound_to_object_ptr_invocation_data.h>

namespace clbl { namespace internal {

//!Wraps a PMF and a pointer to an object with which to call it.

template<typename Creator,
        qualify_flags QFlags,
        typename UnderlyingType,
        typename TPtr,
        typename TMemberFnPtr>
struct member_function_bound_to_object_ptr_wrapper {
    
    using mf = pmf<TMemberFnPtr>;

    using arg_types = typename mf::template unpack_args<std::tuple>;
    using clbl_tag = pmf_ptr_tag;
    using creator = Creator;
    using forwarding_glue = typename mf::forwarding_glue;
    using invocation_data_type = 
            member_function_bound_to_object_ptr_invocation_data<TPtr, TMemberFnPtr>;

    using this_t = member_function_bound_to_object_ptr_wrapper<
            Creator, QFlags, UnderlyingType, TPtr, TMemberFnPtr>;

    using return_type = typename mf::return_type;
    using type = typename mf::decay_to_function;
    using underlying_type = UnderlyingType;

    template<qualify_flags Flags>
    using add_qualifiers = member_function_bound_to_object_ptr_wrapper<
            Creator, QFlags | Flags, UnderlyingType, TPtr, TMemberFnPtr>;

    static constexpr auto q_flags = QFlags;
    static constexpr auto is_ambiguous = false;

    invocation_data_type data;

    inline
    member_function_bound_to_object_ptr_wrapper(TMemberFnPtr f_ptr, no_const_no_ref<TPtr>&& o_ptr)
        : data{ f_ptr, static_cast<TPtr&&>(o_ptr) } {}

    inline
    member_function_bound_to_object_ptr_wrapper(TMemberFnPtr f_ptr, no_const_no_ref<TPtr>& o_ptr)
        : data{ f_ptr, o_ptr } {}

    inline
    member_function_bound_to_object_ptr_wrapper(TMemberFnPtr f_ptr, const no_const_no_ref<TPtr>&  o_ptr)
        : data{ f_ptr, o_ptr } {}

    inline
    member_function_bound_to_object_ptr_wrapper(this_t& other) = default;

    inline
    member_function_bound_to_object_ptr_wrapper(const this_t& other) = default;

    inline
    member_function_bound_to_object_ptr_wrapper(this_t&& other) = default;

    template<typename... Fargs>
    inline return_type
    operator()(Fargs&&... a) {
        return (harden_cast<q_flags>(*data.object_ptr)
            .*data.pmf)(static_cast<Fargs&&>(a)...);
    }

    template<typename... Fargs>
    inline return_type
    operator()(Fargs&&... a) const {
        return (harden_cast<qflags::const_ | q_flags>(*data.object_ptr)
            .*data.pmf)(static_cast<Fargs&&>(a)...);
    }

    template<typename... Fargs>
    inline return_type
    operator()(Fargs&&... a) volatile {
        return (harden_cast<qflags::volatile_ | q_flags>(*data.object_ptr)
            .*data.pmf)(static_cast<Fargs&&>(a)...);
    }

    template<typename... Fargs>
    inline return_type
    operator()(Fargs&&... a) const volatile {
        return (harden_cast<qflags::const_ | qflags::volatile_ | q_flags>(*data.object_ptr)
            .*data.pmf)(static_cast<Fargs&&>(a)...);
    }

    template<typename T = UnderlyingType, std::enable_if_t<
        is_clbl<T>, dummy>* = nullptr>
    static inline constexpr auto 
    copy_invocation(T& c){
        return no_ref<decltype(*c.data.object_ptr)>::copy_invocation(
            harden_cast<q_flags>(*c.data.object_ptr)
        );
    }

    template<typename T = UnderlyingType, std::enable_if_t<
        is_clbl<T>, dummy>* = nullptr>
    static inline constexpr auto 
    copy_invocation(const T& c) {
        return no_ref<decltype(*c.data.object_ptr)>::copy_invocation(
            harden_cast<qflags::const_ | q_flags>(*c.data.object_ptr)
        );
    }

    template<typename T = UnderlyingType, std::enable_if_t<
        is_clbl<T>, dummy>* = nullptr>
    static inline constexpr auto
    copy_invocation(volatile T& c) {
        return no_ref<decltype(*c.data.object_ptr)>::copy_invocation(
            harden_cast<qflags::volatile_ | q_flags>(*c.data.object_ptr)
        );
    }

    template<typename T = UnderlyingType, std::enable_if_t<
        is_clbl<T>, dummy>* = nullptr>
    static inline constexpr auto
    copy_invocation(const volatile T& c) {
        return no_ref<decltype(*c.data.object_ptr)>::copy_invocation(
            harden_cast<
                qflags::const_ | qflags::volatile_ | q_flags
            >(*c.data.object_ptr)
        );
    }

    template<typename T = UnderlyingType, std::enable_if_t<
        !is_clbl<T>, dummy>* = nullptr>
    static inline constexpr auto
    copy_invocation(this_t& c) {
        return c;
    }

    template<typename T = UnderlyingType, std::enable_if_t<
        !is_clbl<T>, dummy>* = nullptr>
    static inline constexpr auto 
    copy_invocation(const this_t& c) {
        return add_qualifiers<qflags::const_>{
            c.data.pmf,
            c.data.object_ptr
        };
    }

    template<typename T = UnderlyingType, std::enable_if_t<
        !is_clbl<T>, dummy>* = nullptr>
    static inline constexpr auto
    copy_invocation(volatile this_t& c) {
        return add_qualifiers<qflags::volatile_>{
            c.data.pmf,
            c.data.object_ptr
        };
    }

    template<typename T = UnderlyingType, std::enable_if_t<
        !is_clbl<T>, dummy>* = nullptr>
    static inline constexpr auto
    copy_invocation(const volatile this_t& c) {
        return add_qualifiers<qflags::const_ | qflags::volatile_>{
                c.data.pmf,
                c.data.object_ptr
        };
    }
};

}}

#endif