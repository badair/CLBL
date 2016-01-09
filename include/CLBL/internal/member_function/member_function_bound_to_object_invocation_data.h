
/*!
@file
Defines `clbl::internal::member_function_bound_to_object_invocation_data`.

@copyright Barrett Adair 2015
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)

*/

#ifndef CLBL_MEMBER_FUNCTION_BOUND_TO_OBJECT_INVOCATION_DATA_H
#define CLBL_MEMBER_FUNCTION_BOUND_TO_OBJECT_INVOCATION_DATA_H

namespace clbl { namespace internal {

template<typename T, typename Pmf>
struct member_function_bound_to_object_invocation_data {
    
    Pmf pmf;
    T object;

    using this_t = member_function_bound_to_object_invocation_data<T, Pmf>;

    inline
    member_function_bound_to_object_invocation_data(this_t&) = default;

    inline
    member_function_bound_to_object_invocation_data(const this_t&) = default;

    inline
    member_function_bound_to_object_invocation_data(this_t&&) = default;

    inline
    member_function_bound_to_object_invocation_data(volatile this_t& other)
        : pmf( other.pmf ), object( other.object ) {}

    inline
    member_function_bound_to_object_invocation_data(const volatile this_t& other)
        : pmf( other.pmf ), object( other.object ) {}

    inline
    member_function_bound_to_object_invocation_data(Pmf p, no_const_no_ref<T>& o)
        : pmf( p ), object( o ) {}

    inline
    member_function_bound_to_object_invocation_data(Pmf p, const no_const_no_ref<T>& o)
        : pmf( p ), object( o ) {}

    inline
    member_function_bound_to_object_invocation_data(Pmf p, T&& o)
        : pmf( p ), object( std::move(o) ) {}
};

}}

#endif