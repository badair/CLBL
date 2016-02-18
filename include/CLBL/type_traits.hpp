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
    using no_ref = typename std::remove_reference<T>::type;

    template<typename T>
    using no_const = typename std::remove_const<T>::type;

    template<typename T>
    using no_const_no_ref = 
        typename std::remove_const<
            typename std::remove_reference<T>::type
        >::type;

    template<typename T>
    using no_volatile = typename std::remove_volatile<T>::type;
}

#endif