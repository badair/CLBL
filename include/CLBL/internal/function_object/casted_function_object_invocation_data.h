/*!
@file
Defines `clbl::internal::casted_function_object_invocation_data`.

@copyright Barrett Adair 2015
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)

*/
    
#ifndef CLBL_CASTED_FUNCTION_OBJECT_INVOCATION_DATA_H
#define CLBL_CASTED_FUNCTION_OBJECT_INVOCATION_DATA_H

namespace clbl { namespace internal {

    template<typename T, typename Pmf>
    struct casted_function_object_invocation_data {

        static constexpr auto pmf = 
                static_cast<Pmf>(&no_ref<T>::operator());
        T object;

        using this_t = casted_function_object_invocation_data<T, Pmf>;

        inline
        casted_function_object_invocation_data(this_t&) = default;

        inline
        casted_function_object_invocation_data(const this_t&) = default;

        inline
        casted_function_object_invocation_data(this_t&&) = default;

        inline
        casted_function_object_invocation_data(volatile this_t& other)
            : object( other.object ) {}

        inline
        casted_function_object_invocation_data(const volatile this_t& other)
            : object( other.object ) {}

        inline
        casted_function_object_invocation_data(no_const_no_ref<T>& o)
            : object( o ) {}

        inline
        casted_function_object_invocation_data(const no_const_no_ref<T>& o)
            : object( o ) {}

        inline
        casted_function_object_invocation_data(no_const_no_ref<T>&& o)
            : object( std::move(o) ) {}
    };
}}

#endif