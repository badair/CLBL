/*!
@file
Defines `clbl::no_ref`.

@copyright Barrett Adair 2015
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)

*/

#ifndef CLBL_NOREF_H
#define CLBL_NOREF_H

#include <type_traits>

namespace clbl {
	
    template<typename T>
    using no_ref = std::remove_reference_t<T>;

    template<typename T>
    using no_const = std::remove_const_t<T>;

    template<typename T>
    using no_const_no_ref = std::remove_const_t<std::remove_reference_t<T> >;

    template<typename T>
    using no_volatile = std::remove_volatile_t<T>;

    template<typename T, typename U>
    constexpr auto is_same = std::is_same<T, U>::value;

    template<typename From, typename To>
    constexpr auto can_convert = std::is_convertible<From, To>::value;
}

#endif