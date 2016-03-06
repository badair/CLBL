 /*!
@file

@copyright Barrett Adair 2015
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)

*/

#ifndef CLBL_CONSTRAINTS_H
#define CLBL_CONSTRAINTS_H

#include <type_traits>
#include <CLBL/tags.hpp>
#include <CLBL/is_clbl.hpp>
#include <quali/quali.hpp>
#include <CLBL/is_reference_wrapper.hpp>
#include <CLBL/has_normal_call_operator.hpp>

// CLBL_REQUIRES_ and CLBL_REQUIRES adapted from Range-v3 here:
// https://github.com/ericniebler/range-v3/blob/6600e6054513202e61a067de48c4a05ca2b11099/include/range/v3/utility/concepts.hpp#L861
// Copyright Eric Niebler 2013-2014

#define CLBL_PP_CAT_(X, Y) X ## Y
#define CLBL_PP_CAT(X, Y)  CLBL_PP_CAT_(X, Y)

#define CLBL_REQUIRES_(...)                                                         \
    int CLBL_PP_CAT(clbl_requires_, __LINE__) = 42,                                 \
    typename std::enable_if<                                                        \
        (CLBL_PP_CAT(clbl_requires_, __LINE__) == 43) || (__VA_ARGS__),             \
        bool                                                                        \
    >::type constraint_success = true                                               \
/**/

#define CLBL_REQUIRES(...)                                                          \
    template<                                                                       \
        int CLBL_PP_CAT(clbl_requires_, __LINE__) = 42,                             \
        typename std::enable_if<                                                    \
            (CLBL_PP_CAT(clbl_requires_, __LINE__) == 43) || (__VA_ARGS__),         \
            bool                                                                    \
        >::type constraint_success = true>                                          \
/**/

namespace clbl {

template<
    typename Target,
    typename Ptr, 
    CLBL_REQUIRES_(std::is_convertible<decltype(*std::declval<Ptr>()), Target>::value)
>
using ConvertiblePointer = Ptr;

template<
    typename Target,
    typename Obj, 
    CLBL_REQUIRES_(std::is_convertible<Obj, Target>::value)
>
using ConvertibleObject = Obj;

template<
    typename T, 
    CLBL_REQUIRES_(!clbl::can_dereference<T>::value && !is_reference_wrapper<T>::value)
>
using ValueType = T;

template<typename T>
using DefaultDereferenceable = typename std::conditional<
    clbl::can_dereference<T>::value,
    T,
    dummy*
>::type;

template<
    typename Ptr, 
    CLBL_REQUIRES_(
        clbl::can_dereference<Ptr>::value 
        && std::is_class<no_ref<decltype(*std::declval<DefaultDereferenceable<Ptr>>())>>::value
    )
>
using DereferenceableObject = Ptr;

template<
    typename Target,
    typename T,
    CLBL_REQUIRES_(
        std::is_convertible<T, Target>::value
        || std::is_convertible<decltype(*std::declval<DefaultDereferenceable<T>>()), Target>::value
    )
>
using GenerallyConvertibleObject = T;

template<typename T>
using CallableWrapper = typename std::enable_if<is_clbl<T>::value, T>::type;

template<typename T>
using default_normal_callable = typename std::conditional<
    has_normal_call_operator<T>::value,
    T,
    callable_dummy
>::type;

}

#endif