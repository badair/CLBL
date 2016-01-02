/*!
@file
Defines `clbl::`.

@copyright Barrett Adair 2015
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)

*/

#ifndef CLBL_INVOCATION_DATA_H
#define CLBL_INVOCATION_DATA_H

#include <utility>
#include <type_traits>
#include <CLBL/tags.h>
#include <CLBL/no_ref.h>

namespace clbl {

    /*
    the types in this file are the types that make up the "data" member variable in
    clbl wrappers. They contain different arrangements of 1 or 2 of the following:

         - objects
         - pointers to objects
         - function pointers
         - member function pointers
    */

    template<typename TPtr>
    struct ptr_invocation_data {
        TPtr ptr;

        using my_type = ptr_invocation_data<TPtr>;

        inline ptr_invocation_data(my_type&) = default;
        inline ptr_invocation_data(const my_type&) = default;
        inline ptr_invocation_data(my_type&&) = default;

        inline ptr_invocation_data(volatile my_type& other)
            : ptr( other.ptr )
        {}

        inline ptr_invocation_data(const volatile my_type& other)
            : ptr( other.ptr )
        {}

        inline ptr_invocation_data(std::remove_const_t<TPtr>& p)
            : ptr( p )
        {}

        inline ptr_invocation_data(const TPtr& p)
            : ptr( p )

        {}
        inline ptr_invocation_data(TPtr&& p)
            : ptr( std::forward<TPtr>(p) )
        {}
    };

    template<typename T>
    struct reference_invocation_data {
     
        T ref;

        static_assert(std::is_reference<T>::value, "CLBL internal logic error: not a reference type.");

        using my_type = reference_invocation_data<T>;

        inline reference_invocation_data(my_type&) = default;
        inline reference_invocation_data(const my_type&) = default;
        inline reference_invocation_data(my_type&&) = default;

        inline reference_invocation_data(const volatile my_type& other)
            : ref( other.ref )
        {}

        inline reference_invocation_data(const T r)
            : ref( r )
        {}
    };

    template<typename T>
    struct object_invocation_data {
        
        T object;

        using my_type = object_invocation_data<T>;

        inline object_invocation_data(my_type&) = default;
        inline object_invocation_data(const my_type&) = default;
        inline object_invocation_data(my_type&&) = default;

        inline object_invocation_data(volatile my_type& other)
            : object( other.object )
        {}

        inline object_invocation_data(const volatile my_type& other)
            : object( other.object )
        {}

        inline object_invocation_data(std::remove_const_t<T>& o)
            : object( o )
        {}

        inline object_invocation_data(const T& o)
            : object( o )
        {}

        inline object_invocation_data(T&& o)
            : object( std::forward<T>(o) )
        {}
    };

    template<typename T, typename TMemberFnPtr>
    struct pmf_invocation_data {
        TMemberFnPtr pmf;
        T object;

        using my_type = pmf_invocation_data<T, TMemberFnPtr>;

        inline pmf_invocation_data(my_type&) = default;
        inline pmf_invocation_data(const my_type&) = default;
        inline pmf_invocation_data(my_type&&) = default;

        inline pmf_invocation_data(volatile my_type& other)
            : pmf( other.pmf ), object( other.object )
        {}

        inline pmf_invocation_data(const volatile my_type& other)
            : pmf( other.pmf ), object( other.object )
        {}

        inline pmf_invocation_data(TMemberFnPtr p, std::remove_const_t<T>& o)
            : pmf( p ), object( o )
        {}

        inline pmf_invocation_data(TMemberFnPtr p, const T& o)
            : pmf( p ), object( o )

        {}
        inline pmf_invocation_data(TMemberFnPtr p, T&& o)
            : pmf( p ), object( std::forward<T>(o) )
        {}
    };

    template<typename T, typename TMemberFnPtr, TMemberFnPtr Pmf>
    struct pmf_invocation_data_slim {
        static constexpr auto pmf = Pmf;
        T object;

        using my_type = pmf_invocation_data_slim<T, TMemberFnPtr, Pmf>;

        inline pmf_invocation_data_slim(my_type&) = default;
        inline pmf_invocation_data_slim(const my_type&) = default;
        inline pmf_invocation_data_slim(my_type&&) = default;

        inline pmf_invocation_data_slim(volatile my_type& other)
            : object( other.object )
        {}

        inline pmf_invocation_data_slim(const volatile my_type& other)
            : object( other.object )
        {}

        inline pmf_invocation_data_slim(std::remove_const_t<T>& o)
            : object( o )
        {}

        inline pmf_invocation_data_slim(const T& o)
            : object( o )

        {}
        inline pmf_invocation_data_slim(T&& o)
            : object( std::forward<T>(o) )
        {}
    };

    template<typename TPtr, typename TMemberFnPtr>
    struct indirect_pmf_invocation_data {
        TMemberFnPtr pmf;
        TPtr object_ptr;

        using my_type = indirect_pmf_invocation_data<TPtr, TMemberFnPtr>;

        inline indirect_pmf_invocation_data(my_type&) = default;
        inline indirect_pmf_invocation_data(const my_type&) = default;
        inline indirect_pmf_invocation_data(my_type&&) = default;

        inline indirect_pmf_invocation_data(volatile my_type& other)
            : pmf( other.pmf ), object_ptr( other.object_ptr )
        {}

        inline indirect_pmf_invocation_data(const volatile my_type& other)
            : pmf( other.pmf ), object_ptr( other.object_ptr )
        {}

        inline indirect_pmf_invocation_data(TMemberFnPtr p, std::remove_const_t<TPtr>& o)
            : pmf( p ), object_ptr( o )
        {}

        inline indirect_pmf_invocation_data(TMemberFnPtr p, const TPtr& o)
            : pmf( p ), object_ptr( o )
        {}

        inline indirect_pmf_invocation_data(TMemberFnPtr p, TPtr&& o)
            : pmf( p ), object_ptr( std::forward<TPtr>(o) )
        {}
    };

    template<typename TPtr, typename TMemberFnPtr, TMemberFnPtr Pmf>
    struct indirect_pmf_invocation_data_slim {
        static constexpr auto pmf = Pmf;
        TPtr object_ptr;

        using my_type = indirect_pmf_invocation_data_slim<TPtr, TMemberFnPtr, Pmf>;

        inline indirect_pmf_invocation_data_slim(my_type&) = default;
        inline indirect_pmf_invocation_data_slim(const my_type&) = default;
        inline indirect_pmf_invocation_data_slim(my_type&&) = default;

        inline indirect_pmf_invocation_data_slim(volatile my_type& other)
            : object_ptr( other.object_ptr )
        {}

        inline indirect_pmf_invocation_data_slim(const volatile my_type& other)
            : object_ptr( other.object_ptr )
        {}

        inline indirect_pmf_invocation_data_slim(std::remove_const_t<TPtr>& o)
            : object_ptr( o )
        {}

        inline indirect_pmf_invocation_data_slim(const TPtr& o)
            : object_ptr( o )

        {}
        inline indirect_pmf_invocation_data_slim(TPtr&& o)
            : object_ptr( std::forward<TPtr>(o) )
        {}
    };

    template<typename TPtr, typename UnderlyingType, typename TMemberFnPtr>
    struct object_pointer_casted_invocation_data {
        static constexpr auto pmf = static_cast<TMemberFnPtr>(&UnderlyingType::operator());
        TPtr object_ptr;

        using my_type = object_pointer_casted_invocation_data<TPtr, UnderlyingType, TMemberFnPtr>;

        inline object_pointer_casted_invocation_data(my_type&) = default;
        inline object_pointer_casted_invocation_data(const my_type&) = default;
        inline object_pointer_casted_invocation_data(my_type&&) = default;

        inline object_pointer_casted_invocation_data(volatile my_type& other)
            : object_ptr( other.object_ptr )
        {}

        inline object_pointer_casted_invocation_data(const volatile my_type& other)
            : object_ptr( other.object_ptr )
        {}

        inline object_pointer_casted_invocation_data(std::remove_const_t<TPtr>& o)
            : object_ptr( o )
        {}

        inline object_pointer_casted_invocation_data(const TPtr& o)
            : object_ptr( o )

        {}
        inline object_pointer_casted_invocation_data(TPtr&& o)
            : object_ptr( std::forward<TPtr>(o) )
        {}
    };

    template<typename T, typename TMemberFnPtr>
    struct object_casted_invocation_data {
        static constexpr auto pmf = static_cast<TMemberFnPtr>(&no_ref<T>::operator());
        T object;

        using my_type = object_casted_invocation_data<T, TMemberFnPtr>;

        inline object_casted_invocation_data(my_type&) = default;
        inline object_casted_invocation_data(const my_type&) = default;
        inline object_casted_invocation_data(my_type&&) = default;

        inline object_casted_invocation_data(volatile my_type& other)
            : object( other.object )
        {}

        inline object_casted_invocation_data(const volatile my_type& other)
            : object( other.object )
        {}

        inline object_casted_invocation_data(std::remove_const_t<T>& o)
            : object( o )
        {}

        inline object_casted_invocation_data(const T& o)
            : object( o )

        {}
        inline object_casted_invocation_data(T&& o)
            : object( std::forward<T>(o) )
        {}
    };
}

#endif