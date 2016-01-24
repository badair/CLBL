/*!
@file
Defines `clbl::internal::slim_member_function_bound_to_object_ptr_invocation_data`.

@copyright Barrett Adair 2015
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)

*/

#ifndef CLBL_SLIM_MEMBER_FUNCTION_BOUND_TO_OBJECT_PTR_INVOCATION_DATA_H
#define CLBL_SLIM_MEMBER_FUNCTION_BOUND_TO_OBJECT_PTR_INVOCATION_DATA_H

namespace clbl {

    template<typename TPtr, typename TMemberFnPtr, TMemberFnPtr Pmf>
    struct slim_member_function_bound_to_object_ptr_invocation_data {
        static constexpr auto pmf = Pmf;
        TPtr object_ptr;

        using this_t = slim_member_function_bound_to_object_ptr_invocation_data<TPtr, TMemberFnPtr, Pmf>;

        inline
        slim_member_function_bound_to_object_ptr_invocation_data(this_t&) = default;

        inline
        slim_member_function_bound_to_object_ptr_invocation_data(const this_t&) = default;

        inline
        slim_member_function_bound_to_object_ptr_invocation_data(this_t&&) = default;

        inline
        slim_member_function_bound_to_object_ptr_invocation_data(volatile this_t& other)
            : object_ptr( other.object_ptr ) {}

        inline
        slim_member_function_bound_to_object_ptr_invocation_data(const volatile this_t& other)
            : object_ptr( other.object_ptr ) {}

        inline
        slim_member_function_bound_to_object_ptr_invocation_data(no_const<TPtr>& o)
            : object_ptr( o ) {}

        inline
        slim_member_function_bound_to_object_ptr_invocation_data(const TPtr& o)
            : object_ptr( o ) {}

        inline
        slim_member_function_bound_to_object_ptr_invocation_data(TPtr&& o)
            : object_ptr( std::move(o) ) {}
    };
}

#endif