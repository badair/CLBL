/*!
@file
Defines `clbl::internal::slim_member_function_bound_to_object_invocation_data`.

@copyright Barrett Adair 2015
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)

*/

#ifndef CLBL_SLIM_MEMBER_FUNCTION_BOUND_TO_OBJECT_INVOCATION_DATA_H
#define CLBL_SLIM_MEMBER_FUNCTION_BOUND_TO_OBJECT_INVOCATION_DATA_H

namespace clbl { namespace internal {

template<typename T, typename Pmf, Pmf Value>
struct slim_member_function_bound_to_object_invocation_data {

    static constexpr auto pmf = Value;
    T object;

    using this_t = slim_member_function_bound_to_object_invocation_data<T, Pmf, Value>;

    inline
    slim_member_function_bound_to_object_invocation_data(this_t&) = default;

    inline
    slim_member_function_bound_to_object_invocation_data(const this_t&) = default;

    inline
    slim_member_function_bound_to_object_invocation_data(this_t&&) = default;

    inline
    slim_member_function_bound_to_object_invocation_data(volatile this_t& other)
        : object( other.object ) {}

    inline
    slim_member_function_bound_to_object_invocation_data(const volatile this_t& other)
        : object( other.object ) {}

    inline
    slim_member_function_bound_to_object_invocation_data(no_const_no_ref<T>& o)
        : object( o ) {}

    inline
    slim_member_function_bound_to_object_invocation_data(const no_const_no_ref<T>& o)
        : object( o ) {}

    inline
    slim_member_function_bound_to_object_invocation_data(no_const_no_ref<T>&& o)
        : object( std::move(o) ) {}
};

}}

#endif