/*!
@file
Defines `clbl::internal::ambiguous_function_object_wrapper`.

@copyright Barrett Adair 2015
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)

*/

#ifndef CLBL_AMBIGUOUS_FUNCTION_OBJECT_WRAPPER_H
#define CLBL_AMBIGUOUS_FUNCTION_OBJECT_WRAPPER_H

namespace clbl { namespace internal {

/*!
Wraps a callable object whose operator() is overloaded/templated.
You may still call this wrapper as you would call the original object,
but you must disambiguate by calling clbl::harden before using any of
the additional features in CLBL.
*/
template<typename Creator, qualify_flags QFlags, typename T>
struct ambiguous_function_object_wrapper {

    using arg_types = ambiguous_args;
    using clbl_tag = ambi_fn_obj_tag;
    using creator = Creator;
    using forwarding_glue = ambiguous_return(ambiguous_args);
    using invocation_data_type = T;
    using this_t = ambiguous_function_object_wrapper<Creator, QFlags, T>;
    using return_type = ambiguous_return;
    using type = ambiguous_return(ambiguous_args);
    using underlying_type = T;

    template<qualify_flags Flags>
    using add_qualifiers = ambiguous_function_object_wrapper<Creator, QFlags | Flags, T>;

    static constexpr auto q_flags = QFlags;
    static constexpr auto is_ambiguous = true;

    invocation_data_type data;

    inline constexpr
    ambiguous_function_object_wrapper(const no_const_no_ref<T>& o)
        : data( o )
    {}

    inline constexpr
    ambiguous_function_object_wrapper(no_const_no_ref<T>& o)
        : data( o )
    {}

    inline constexpr
    ambiguous_function_object_wrapper(no_const_no_ref<T>&& o)
        : data( std::move(o) )
    {}

    inline constexpr
    ambiguous_function_object_wrapper(this_t&) = default;

    inline constexpr
    ambiguous_function_object_wrapper(this_t&&) = default;

    inline constexpr
    ambiguous_function_object_wrapper(const this_t&) = default;
    
    inline constexpr
    ambiguous_function_object_wrapper(volatile this_t& other)
        : data( other.data )
    {}

    inline constexpr
    ambiguous_function_object_wrapper(const volatile this_t& other)
        : data( other.data )
    {}

    template<typename... Fargs>
    inline CLBL_CXX14_CONSTEXPR decltype(auto)
    operator()(Fargs&&... a) {
        return harden_cast<q_flags>(data)
                (static_cast<Fargs&&>(a)...);
    }

    template<typename... Fargs>
    inline constexpr decltype(auto)
    operator()(Fargs&&... a) const {
        return harden_cast<qflags::const_ | q_flags>(data)
                (static_cast<Fargs&&>(a)...);
    }

    template<typename... Fargs>
    inline CLBL_CXX14_CONSTEXPR decltype(auto)
    operator()(Fargs&&... a) volatile {
        return harden_cast<qflags::volatile_ | q_flags>(data)
                (static_cast<Fargs&&>(a)...);
    }

    template<typename... Fargs>
    inline constexpr decltype(auto)
    operator()(Fargs&&... a) const volatile {
        return harden_cast<qflags::const_ | qflags::volatile_ | q_flags>
                (data)(static_cast<Fargs&&>(a)...);
    }

    static inline constexpr auto
    copy_invocation(this_t& c) {
        return c;
    }

    static inline constexpr auto
    copy_invocation(const this_t& c) {
        return add_qualifiers<qflags::const_ | q_flags>{c.data};
    }

    static inline constexpr auto
    copy_invocation(volatile this_t& c) {
        return add_qualifiers<qflags::volatile_ | q_flags>{c.data};
    }

    static inline constexpr auto
    copy_invocation(const volatile this_t& c) {
        return add_qualifiers<qflags::const_ | qflags::volatile_ | q_flags>{c.data};
    }
};

}}

#endif