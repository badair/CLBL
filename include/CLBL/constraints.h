 /*!
@file

@copyright Barrett Adair 2015
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)

*/

#ifndef CLBL_CONSTRAINTS_H
#define CLBL_CONSTRAINTS_H

#include <type_traits>

namespace clbl {

template<typename Target, typename Ptr, std::enable_if_t<
    std::is_convertible<decltype(*std::declval<Ptr>()), Target>::value > * = nullptr >
using ConvertiblePointer = Ptr;

}

#endif