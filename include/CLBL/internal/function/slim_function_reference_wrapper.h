/*!
@file
Defines `clbl::internal::slim_function_reference_wrapper`.

@copyright Barrett Adair 2015
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)

*/

#ifndef CLBL_SLIM_FUNCTION_REFERENCE_WRAPPER_H
#define CLBL_SLIM_FUNCTION_REFERENCE_WRAPPER_H

namespace clbl { namespace internal {

template<typename, typename Failure, Failure>
struct slim_function_reference_wrapper {
    static_assert(sizeof(Failure) < 0, "Not a function.");
};

template<typename Creator, typename Return, typename... Args, Return(&FunctionReference)(Args...)>
struct slim_function_reference_wrapper<Creator, Return(Args...), FunctionReference> {

    using arg_types = std::tuple<Args...>;
    using clbl_tag = free_fn_ref_tag;
    using creator = Creator;
    using forwarding_glue = Return(forward<Args>...);

    struct invocation_data_type {
            using type = Return(&)(Args...);
            static constexpr auto ptr = FunctionReference;
    };

    using this_t = slim_function_reference_wrapper<Creator, Return(Args...), FunctionReference>;
    using return_type = Return;
    using type = Return(Args...);
    using underlying_type = this_t;
    
    template<qualify_flags>
    using add_qualifiers = this_t;

    static constexpr auto q_flags = qflags::default_;
    static constexpr auto is_ambiguous = false;
    static constexpr auto data = invocation_data_type{};

    //todo volatile copy ctors, which means default copy/move

    template<typename... Fargs>
    inline Return operator()(Fargs&&... a) const volatile {
        return invocation_data_type::ptr(static_cast<Fargs&&>(a)...);
    }

    static inline constexpr auto copy_invocation(const this_t&) {
        return invocation_data_type::ptr;
    }

    static inline constexpr auto copy_invocation(const volatile this_t&) {
        return invocation_data_type::ptr;
    }
};

}}

#endif