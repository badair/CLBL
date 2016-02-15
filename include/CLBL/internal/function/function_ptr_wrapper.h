 /*!
@file
Defines `clbl::internal::function_ptr_wrapper`.

@copyright Barrett Adair 2015
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)

*/

#ifndef CLBL_FUNCTION_PTR_WRAPPER_H
#define CLBL_FUNCTION_PTR_WRAPPER_H

namespace clbl { namespace internal {

//! Wraps a free function ptr
template<typename, typename Failure>
struct function_ptr_wrapper {
    static_assert(sizeof(Failure) < 0, "Not a function.");
};

template<typename Creator, typename Return, typename... Args>
struct function_ptr_wrapper<Creator, Return(Args...)> {

    using arg_types = std::tuple<Args...>;
    using clbl_tag = free_fn_tag;
    using creator = Creator;
    using forwarding_glue = Return(forward<Args>...);
    using invocation_data_type = Return(*)(Args...);
    using this_t = function_ptr_wrapper<Creator, Return(Args...)>;
    using return_type = Return;
    using type = Return(Args...);
    using underlying_type = this_t;
    
    template<qualify_flags>
    using add_qualifiers = this_t;

    static constexpr auto q_flags = qflags::default_;
    static constexpr auto is_ambiguous = false;

    const invocation_data_type data;

    template<typename... Fargs>
    inline constexpr Return
    operator()(Fargs&&... a) const {
        return (*data)(static_cast<Fargs&&>(a)...);
    }

    template<typename... Fargs>
    inline constexpr Return
    operator()(Fargs&&... a) const volatile {
        return (*data)(static_cast<Fargs&&>(a)...);
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

}}

#endif