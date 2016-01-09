/*!
@file
Defines `clbl::internal::slim_member_function_bound_to_object_wrapper`.

@copyright Barrett Adair 2015
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)

*/

#ifndef CLBL_SLIM_MEMBER_FUNCTION_BOUND_TO_OBJECT_WRAPPER_H
#define CLBL_SLIM_MEMBER_FUNCTION_BOUND_TO_OBJECT_WRAPPER_H

#include <CLBL/internal/member_function/slim_member_function_bound_to_object_invocation_data.h>

namespace clbl { namespace internal {

/*!
Wraps a PMF and an object with which to call it. It is identical
to object_bound_member_function_wrapper except that the PMF is not
stored in an object instance, but passed as a template arg instead.
*/
template<typename, qualify_flags, typename, typename Failure, Failure, typename>
struct slim_member_function_bound_to_object_wrapper {
    static_assert(sizeof(Failure) < 0, "Not a member function.");
};

template<
    typename Creator,
    qualify_flags QFlags,
    typename T,
    typename Pmf,
    Pmf Value,
    typename Return,
    typename... Args>
struct slim_member_function_bound_to_object_wrapper<
        Creator,
        QFlags,
        T,
        Pmf,
        Value,
        Return(std::remove_cv_t<T>::*)(Args...)> {

    using decayed_member_fn_ptr = Return(std::remove_cv_t<T>::*)(Args...);

    using arg_types = std::tuple<Args...>;
    using clbl_tag = pmf_tag;
    using creator = Creator;
    using forwarding_glue = Return(forward<Args>...);

    using invocation_data_type = 
            slim_member_function_bound_to_object_invocation_data<qualified_type<T, QFlags>, Pmf, Value>;

    using this_t = slim_member_function_bound_to_object_wrapper<
                        Creator, QFlags, T, Pmf, Value, decayed_member_fn_ptr>;
    using return_type = Return;
    using type = Return(Args...);
    using underlying_type = qualified_type<T, QFlags>;

    template<qualify_flags Flags>
    using add_qualifiers = slim_member_function_bound_to_object_wrapper<
                        Creator, QFlags | Flags, T, Pmf, Value, decayed_member_fn_ptr>;

    static constexpr auto q_flags = QFlags;
    static constexpr auto is_ambiguous = false;

    invocation_data_type data;

    inline
    slim_member_function_bound_to_object_wrapper(no_const_no_ref<T>&& o)
        : data{ std::move(o) } {}

    inline
    slim_member_function_bound_to_object_wrapper(const no_const_no_ref<T>& o)
        : data{ o } {}

    inline
    slim_member_function_bound_to_object_wrapper(no_const_no_ref<T>& o)
        : data{ o } {}

    inline
    slim_member_function_bound_to_object_wrapper(this_t& other) = default;

    inline
    slim_member_function_bound_to_object_wrapper(const this_t& other) = default;

    inline
    slim_member_function_bound_to_object_wrapper(this_t&& other) = default;

    inline
    slim_member_function_bound_to_object_wrapper(const volatile this_t& other)
        : data(other.data) {}

    template<typename... Fargs>
    inline Return
    operator()(Fargs&&... a) {
        return (harden_cast<q_flags>(data.object)
                .*invocation_data_type::pmf)(static_cast<Fargs&&>(a)...);
    }

    template<typename... Fargs>
    inline Return
    operator()(Fargs&&... a) const {
        return (harden_cast<qflags::const_ | q_flags>(data.object)
                .*invocation_data_type::pmf)(static_cast<Fargs&&>(a)...);
    }

    template<typename... Fargs>
    inline Return
    operator()(Fargs&&... a) volatile {
        return (harden_cast<qflags::volatile_ | q_flags>(data.object)
                .*invocation_data_type::pmf)(static_cast<Fargs&&>(a)...);
    }

    template<typename... Fargs>
    inline Return
    operator()(Fargs&&... a) const volatile {
        return (harden_cast<qflags::const_ | qflags::volatile_ | q_flags>(data.object)
                .*invocation_data_type::pmf)(static_cast<Fargs&&>(a)...);
    }

    template<typename U = underlying_type, std::enable_if_t<
        is_clbl<U>, dummy>* = nullptr>
    static inline constexpr auto
    copy_invocation(U& c) {
        return no_ref<decltype(c.data.object)>::copy_invocation(
            harden_cast<q_flags>(c.data.object)
        );
    }

    template<typename U = underlying_type, std::enable_if_t<
        is_clbl<U>, dummy>* = nullptr>
    static inline constexpr auto
    copy_invocation(const U& c) {
        return no_ref<decltype(c.data.object)>::copy_invocation(
            harden_cast<qflags::const_ | q_flags>(c.data.object)
        );
    }

    template<typename U = underlying_type, std::enable_if_t<
        is_clbl<U>, dummy>* = nullptr>
    static inline constexpr auto
    copy_invocation(volatile U& c) {
        return no_ref<decltype(c.data.object)>::copy_invocation(
            harden_cast<qflags::volatile_ | q_flags>(c.data.object)
        );
    }

    template<typename U = underlying_type, std::enable_if_t<
        is_clbl<U>, dummy>* = nullptr>
    static inline constexpr auto
    copy_invocation(const volatile U& c) {
        return no_ref<decltype(data.object)>::copy_invocation(
            harden_cast<qflags::const_ | qflags::volatile_ | q_flags>(c.data.object)
        );
    }

    template<typename U = underlying_type, std::enable_if_t<
        !is_clbl<U>, dummy>* = nullptr>
    static inline constexpr auto
    copy_invocation(this_t& c) {
        return c;
    }

    template<typename U = underlying_type, std::enable_if_t<
        !is_clbl<U>, dummy>* = nullptr>
    static inline constexpr auto
    copy_invocation(const this_t& c) {
        return add_qualifiers<qflags::const_>{c.data.object};
    }

    template<typename U = underlying_type, std::enable_if_t<
        !is_clbl<U>, dummy>* = nullptr>
    static inline constexpr auto
    copy_invocation(volatile this_t& c) {
        return add_qualifiers<qflags::volatile_>{c.data.object};
    }

    template<typename U = underlying_type, std::enable_if_t<
        !is_clbl<U>, dummy>* = nullptr>
    static inline constexpr auto
    copy_invocation(const volatile this_t& c) {
        return add_qualifiers<qflags::const_ | qflags::volatile_>{c.data.object};
    }
};

}}

#endif