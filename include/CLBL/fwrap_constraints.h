 /*!
@file

@copyright Barrett Adair 2015
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)

*/

#ifndef CLBL_FWRAP_CONSTRAINTS_H
#define CLBL_FWRAP_CONSTRAINTS_H

namespace clbl {
    template<typename T> 
    struct fwrap_cases {
        
    private:
        
        static constexpr auto can_deref = can_dereference<T>;

        using dereferenceable = std::conditional_t<can_deref, no_ref<T>, dummy*>;

        static constexpr auto is_ambiguous = !has_normal_call_operator<T>;

        static constexpr auto ptr_is_ambiguous = !ptr_has_normal_call_operator<T>;

        static constexpr auto is_function_ptr =
            !std::is_function<no_ref<T> >::value 
            && std::is_function<std::remove_pointer_t<T> >::value;

        /*
        function pointer conversions are handled as a "complex case".
        CLBL doesn't currently handle types that define a function
        pointer conversion operator without also defining an operator(),
        because that would be silly
        */
        static constexpr auto is_function_ref =
            std::is_function<no_ref<T> >::value

            && !std::is_class<no_ref<T> >::value;
        
        static constexpr auto is_member_function_ptr =
            std::is_member_function_pointer<T>::value;
        
    public:

        static constexpr auto is_clbl =
            clbl::is_clbl<no_ref<T> > 
            || clbl::is_clbl<no_ref<decltype(*std::declval<dereferenceable>())> >;

        static constexpr auto is_ptr =
            !is_clbl && can_dereference<no_ref<T> > 
            && !has_normal_call_operator<T>;

        static constexpr auto reference_wrapper_case = is_reference_wrapper<T>;

        static constexpr auto function_ptr_case =
            !is_clbl
            && is_function_ptr;

        static constexpr auto function_ref_case =
            !is_clbl
            && is_function_ref 
            && !has_normal_call_operator<T>;

        static constexpr auto member_function_ptr_case = 
            !is_clbl 
            && !is_function_ptr 
            && is_member_function_ptr;

    private:

        static constexpr auto is_complex_case =
            !is_clbl
            && !reference_wrapper_case 
            && !function_ptr_case 
            && !function_ref_case
            && !member_function_ptr_case
            && std::is_class<std::remove_pointer<no_ref<T> > >::value;

    public:

        static constexpr auto function_object_case =
            is_complex_case
            && !is_ptr
            && !is_ambiguous;

        static constexpr auto ambiguous_function_object_case = 
            is_complex_case
            && !is_ptr
            && is_ambiguous;

        static constexpr auto function_object_ptr_case =
            is_complex_case 
            && is_ptr 
            && !ptr_is_ambiguous;

        static constexpr auto ambiguous_function_object_ptr_case =
            is_complex_case 
            && is_ptr 
            && ptr_is_ambiguous;
    };

template< typename T, std::enable_if_t<
    fwrap_cases<T>::function_ptr_case > * = nullptr >
using BasicFunctionPointer = T;

template< typename T, std::enable_if_t<
    fwrap_cases<T>::function_ref_case > * = nullptr >
using FunctionReference = T;

template< typename T, std::enable_if_t<
    fwrap_cases<T>::member_function_ptr_case > * = nullptr >
using MemberFunctionPointer = T;

template< typename T, std::enable_if_t<
    fwrap_cases<T>::function_object_case > * = nullptr >
using BasicFunctionObject = T;

template< typename T, std::enable_if_t<
    fwrap_cases<T>::function_object_ptr_case > * = nullptr >
using BasicFunctionObjectPtr = T;

template< typename T, std::enable_if_t<
    fwrap_cases<T>::ambiguous_function_object_case > * = nullptr >
using AmbiguousFunctionObject = T;

template< typename T, std::enable_if_t<
    fwrap_cases<T>::ambiguous_function_object_ptr_case > * = nullptr >
using AmbiguousFunctionObjectPtr = T;

template< typename T, std::enable_if_t<
    fwrap_cases<T>::is_clbl > * = nullptr >
using ClblWrapper = T;

template< typename T, std::enable_if_t<
    fwrap_cases<T>::is_ptr > * = nullptr >
using Pointer = T;

template< typename T, std::enable_if_t<
    !fwrap_cases<T>::is_ptr 
    && std::is_class<T>::value> * = nullptr >
using Object = T;

}

#endif