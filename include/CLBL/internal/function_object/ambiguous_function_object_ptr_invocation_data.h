/*!
@file
Defines `clbl::ambiguous_function_object_ptr_invocation_data`.

@copyright Barrett Adair 2015
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)

*/

#ifndef CLBL_AMBIGUOUS_FUNCTION_OBJECT_PTR_INVOCATION_DATA_H
#define CLBL_AMBIGUOUS_FUNCTION_OBJECT_PTR_INVOCATION_DATA_H

#include <utility>
#include <type_traits>
#include <CLBL/tags.h>
#include <CLBL/type_traits.h>

namespace clbl { namespace internal {

    template<typename TPtr>
    struct ambiguous_function_object_ptr_invocation_data {
        TPtr ptr;

        using this_t = ambiguous_function_object_ptr_invocation_data<TPtr>;

        inline constexpr
        ambiguous_function_object_ptr_invocation_data(this_t&) = default;

        inline constexpr
        ambiguous_function_object_ptr_invocation_data(const this_t&) = default;

        inline constexpr
        ambiguous_function_object_ptr_invocation_data(this_t&&) = default;

        inline constexpr
        ambiguous_function_object_ptr_invocation_data(volatile this_t& other)
            : ptr( other.ptr ) {}

        inline constexpr
        ambiguous_function_object_ptr_invocation_data(const volatile this_t& other)
            : ptr( other.ptr ) {}

        inline constexpr
        ambiguous_function_object_ptr_invocation_data(no_const<TPtr>& p)
            : ptr( p ) {}

        inline constexpr
        ambiguous_function_object_ptr_invocation_data(const TPtr& p)
            : ptr( p ) {}

        inline constexpr
        ambiguous_function_object_ptr_invocation_data(TPtr&& p)
            : ptr( std::move(p) ) {}
    };
}}

#endif