/*
@file
Defines `clbl::casted_function_object_ptr_wrapper`.

Copyright Barrett Adair 2015
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)

*/

#ifndef CLBL_CASTED_FUNCTION_OBJECT_PTR_WRAPPER_H
#define CLBL_CASTED_FUNCTION_OBJECT_PTR_WRAPPER_H

#include <CLBL/internal/function_object/casted_function_object_ptr_invocation_data.h>

namespace clbl { namespace internal {

template<typename Failure, qualify_flags, typename, typename, typename, typename>
struct casted_function_object_ptr_wrapper {
    static_assert(sizeof(Failure) < 0, "Not a member function.");
};

/*!
Wraps a pointer to an ambiguous callable object, but uses a static_cast
on operator() to disambiguate it.
*/
template<typename Creator,
        qualify_flags QFlags,
        typename UnderlyingType,
        typename FunctionObjectPtr, 
        typename Pmf,
        typename Return,
        typename... Args>
struct casted_function_object_ptr_wrapper<
        Creator,
        QFlags,
        UnderlyingType,
        FunctionObjectPtr, 
        Pmf,
        Return(std::remove_cv_t<UnderlyingType>::*)(Args...)> {

    using decayed_member_fn_ptr = 
            Return(std::remove_cv_t<UnderlyingType>::*)(Args...);

    using arg_types = std::tuple<Args...>;
    using clbl_tag = pmf_ptr_tag;
    using creator = Creator;
    using forwarding_glue = Return(forward<Args>...);

    using invocation_data_type = 
            casted_function_object_ptr_invocation_data<
                    FunctionObjectPtr, UnderlyingType, Pmf>;

    using this_t = casted_function_object_ptr_wrapper<
                        Creator,
                        QFlags,
                        UnderlyingType,
                        FunctionObjectPtr,
                        Pmf,
                        decayed_member_fn_ptr>;

    using return_type = Return;
    using type = Return(Args...);
    using underlying_type = UnderlyingType;

    template<qualify_flags Flags>
    using add_qualifiers = casted_function_object_ptr_wrapper<
                        Creator,
                        QFlags | Flags,
                        UnderlyingType,
                        FunctionObjectPtr,
                        Pmf,
                        decayed_member_fn_ptr>;

    static constexpr auto q_flags = QFlags;

    static constexpr auto is_ambiguous = 
            std::is_same<Return(Args...), 
                    ambiguous_return(ambiguous_args)>::value;

    invocation_data_type data;



    inline
    casted_function_object_ptr_wrapper(no_const_no_ref<FunctionObjectPtr>&& o_ptr)
        : data{ std::move(o_ptr) } {}

    inline
    casted_function_object_ptr_wrapper(const no_const_no_ref<FunctionObjectPtr>& o_ptr)
        : data{ o_ptr } {}

    inline
    casted_function_object_ptr_wrapper(no_const_no_ref<FunctionObjectPtr>& o_ptr)
        : data{ o_ptr } {}

    inline //todo shouldn't this be disallowed?
    casted_function_object_ptr_wrapper() = default;

    inline
    casted_function_object_ptr_wrapper(this_t& other) = default;

    inline
    casted_function_object_ptr_wrapper(const this_t& other) = default;

    inline
    casted_function_object_ptr_wrapper(this_t&& other) = default;

    template<typename... Fargs>
    inline Return
    operator()(Fargs&&... a) {
        return (harden_cast<q_flags>(*data.object_ptr)
                .*invocation_data_type::pmf)(static_cast<Fargs&&>(a)...);
    }

    template<typename... Fargs>
    inline Return
    operator()(Fargs&&... a) const {
        return (harden_cast<qflags::const_ | q_flags>(*data.object_ptr)
                .*invocation_data_type::pmf)(static_cast<Fargs&&>(a)...);
    }

    template<typename... Fargs>
    inline Return
    operator()(Fargs&&... a) volatile {
        return (harden_cast<qflags::volatile_ | q_flags>(*data.object_ptr)
                .*invocation_data_type::pmf)(static_cast<Fargs&&>(a)...);
    }

    template<typename... Fargs>
    inline Return
    operator()(Fargs&&... a) const volatile {
        return (harden_cast<qflags::const_ | qflags::volatile_ | q_flags>(*data.object_ptr)
                .*invocation_data_type::pmf)(static_cast<Fargs&&>(a)...);
    }

    template<typename U = UnderlyingType, std::enable_if_t<
        is_clbl<U>, dummy>* = nullptr>
    static inline constexpr auto
    copy_invocation(U& c) {
        return no_ref<decltype(*c.data.object_ptr)>::copy_invocation(
            harden_cast<q_flags>(*c.data.object_ptr)
        );
    }

    template<typename U = UnderlyingType, std::enable_if_t<
        is_clbl<U>, dummy>* = nullptr>
    static inline constexpr auto
    copy_invocation(const U& c) {
        return no_ref<decltype(*c.data.object_ptr)>::copy_invocation(
            harden_cast<qflags::const_ | q_flags>(*c.data.object_ptr)
        );
    }

    template<typename U = UnderlyingType, std::enable_if_t<
        is_clbl<U>, dummy>* = nullptr>
    static inline constexpr auto
    copy_invocation(volatile U& c) {
        return no_ref<decltype(*c.data.object_ptr)>::copy_invocation(
            harden_cast<qflags::volatile_ | q_flags>(*c.data.object_ptr)
        );
    }

    template<typename U = UnderlyingType, std::enable_if_t<
        is_clbl<U>, dummy>* = nullptr>
    static inline constexpr auto
    copy_invocation(const volatile U& c) {
        return no_ref<decltype(*c.data.object_ptr)>::copy_invocation(
            harden_cast<qflags::const_ | qflags::volatile_ | q_flags>(*c.data.object_ptr)
        );
    }

    template<typename U = UnderlyingType, std::enable_if_t<
        !is_clbl<U>, dummy>* = nullptr>
    static inline constexpr auto
    copy_invocation(this_t& c) {
        return c;
    }

    template<typename U = UnderlyingType, std::enable_if_t<
        !is_clbl<U>, dummy>* = nullptr>
    static inline constexpr auto
    copy_invocation(const this_t& c) {
        return add_qualifiers<qflags::const_>{c.data.object_ptr};
    }

    template<typename U = UnderlyingType, std::enable_if_t<
        !is_clbl<U>, dummy>* = nullptr>
    static inline constexpr auto
    copy_invocation(volatile this_t& c) {
        return add_qualifiers<qflags::volatile_>{c.data.object_ptr};
    }

    template<typename U = UnderlyingType, std::enable_if_t<
        !is_clbl<U>, dummy>* = nullptr>
    static inline constexpr auto
    copy_invocation(const volatile this_t& c) {
        return add_qualifiers<qflags::const_ | qflags::volatile_>{c.data.object_ptr};
    }
};

}}

#endif
