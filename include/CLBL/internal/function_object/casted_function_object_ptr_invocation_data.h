/*
@file
Defines `clbl::internal::casted_function_object_ptr_invocation_data`.

Copyright Barrett Adair 2015
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)

*/

#ifndef CLBL_CASTED_FUNCTION_OBJECT_PTR_INVOCATION_DATA_H
#define CLBL_CASTED_FUNCTION_OBJECT_PTR_INVOCATION_DATA_H

namespace clbl { namespace internal {

template<typename TPtr, typename UnderlyingType, typename TMemberFnPtr>
struct casted_function_object_ptr_invocation_data {
    static constexpr auto pmf = 
        static_cast<TMemberFnPtr>(&UnderlyingType::operator());
    TPtr object_ptr;

    using this_t = casted_function_object_ptr_invocation_data<
                        TPtr, UnderlyingType, TMemberFnPtr>;

    inline
    casted_function_object_ptr_invocation_data(this_t&) = default;

    inline
    casted_function_object_ptr_invocation_data(const this_t&) = default;

    inline
    casted_function_object_ptr_invocation_data(this_t&&) = default;

    inline
    casted_function_object_ptr_invocation_data(volatile this_t& other)
        : object_ptr( other.object_ptr ) {}

    inline
    casted_function_object_ptr_invocation_data(const volatile this_t& other)
        : object_ptr( other.object_ptr ) {}

    inline
    casted_function_object_ptr_invocation_data(no_const_no_ref<TPtr>& o)
        : object_ptr( o ) {}

    inline casted_function_object_ptr_invocation_data(const no_const_no_ref<TPtr>& o)
        : object_ptr( o ) {}

    inline casted_function_object_ptr_invocation_data(no_const_no_ref<TPtr>&& o)
        : object_ptr( std::move(o) ) {}
};

}}

#endif