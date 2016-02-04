/*
@file
Defines `clbl::internal::ambiguous_function_object_ptr_wrapper`.

Copyright Barrett Adair 2015
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)

*/

#ifndef CLBL_AMBIGUOUS_FUNCTION_OBJECT_PTR_WRAPPER_H
#define CLBL_AMBIGUOUS_FUNCTION_OBJECT_PTR_WRAPPER_H

#include <CLBL/internal/function_object/ambiguous_function_object_ptr_invocation_data.h>

namespace clbl { namespace internal {

 /*!
Wraps a pointer to a callable object whose operator() is overloaded/templated.
Typically created via clbl::fwrap. You may still call this wrapper as you would
call the original object, but you must disambiguate by calling clbl::harden
before using any of the additional features in CLBL.
*/
template<typename Creator, qualify_flags QFlags, typename FunctionObjectPtr>
struct ambiguous_function_object_ptr_wrapper {

    using arg_types = ambiguous_args;
    using clbl_tag = ambi_fn_obj_ptr_tag;
    using creator = Creator;
    using forwarding_glue = ambiguous_return(ambiguous_args);
    using invocation_data_type = ambiguous_function_object_ptr_invocation_data<FunctionObjectPtr>;
    using this_t = ambiguous_function_object_ptr_wrapper<Creator, QFlags, FunctionObjectPtr>;
    using return_type = ambiguous_return;
    using type = ambiguous_return(ambiguous_args);
    using underlying_type = no_ref<decltype(*std::declval<FunctionObjectPtr>())>;

    template<qualify_flags Flags>
    using add_qualifiers = 
    	ambiguous_function_object_ptr_wrapper<Creator, QFlags | Flags, FunctionObjectPtr>;

    static constexpr auto q_flags = QFlags;
    static constexpr auto is_ambiguous = true;

    invocation_data_type data;

    inline constexpr
    ambiguous_function_object_ptr_wrapper(no_const_no_ref<FunctionObjectPtr>& o_ptr)
        : data{ o_ptr } {}

    inline constexpr
    ambiguous_function_object_ptr_wrapper(const no_const_no_ref<FunctionObjectPtr>& o_ptr)
        : data{ o_ptr } {}

    inline constexpr
    ambiguous_function_object_ptr_wrapper(no_const_no_ref<FunctionObjectPtr>&& o_ptr)
        : data{ std::move(o_ptr) } {}

    inline constexpr
    ambiguous_function_object_ptr_wrapper(this_t& other)= default;

    inline constexpr
    ambiguous_function_object_ptr_wrapper(const this_t& other) = default;

    inline constexpr
    ambiguous_function_object_ptr_wrapper(this_t&& other) = default;
  
    inline constexpr
    ambiguous_function_object_ptr_wrapper(volatile this_t& other)
        : data(other.data) {}

    inline constexpr
    ambiguous_function_object_ptr_wrapper(const volatile this_t& other)
        : data(other.data) {}

    template<typename... Fargs>
    inline  constexpr auto
    operator()(Fargs&&... a) {
        return harden_cast<q_flags>
                (*data.ptr)(static_cast<Fargs&&>(a)...);
    }

    template<typename... Fargs>
    inline constexpr auto
    operator()(Fargs&&... a) const {
        return harden_cast<qflags::const_ | q_flags>
                (*data.ptr)(static_cast<Fargs&&>(a)...);
    }

    template<typename... Fargs>
    inline constexpr auto
    operator()(Fargs&&... a) volatile {
        return harden_cast<qflags::volatile_ | q_flags>
                (*data.ptr)(static_cast<Fargs&&>(a)...);
    }

    template<typename... Fargs>
    inline constexpr auto
    operator()(Fargs&&... a) const volatile {
        return harden_cast<qflags::const_ | qflags::volatile_ | q_flags>
                (*data.ptr)(static_cast<Fargs&&>(a)...);
    }

    static inline constexpr auto
    copy_invocation(this_t& c) {
        return c;
    }

    static inline constexpr auto
    copy_invocation(const this_t& c) {
        return add_qualifiers<qflags::const_>{c.data.ptr};
    }

    static inline constexpr auto
    copy_invocation(volatile this_t& c) {
        return add_qualifiers<qflags::volatile_>{c.data.ptr};
    }

    static inline constexpr auto
    copy_invocation(const volatile this_t& c) {
        return add_qualifiers<qflags::const_ | qflags::volatile_>{c.data.ptr};
    }
};

}}

#endif 