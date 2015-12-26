/*

Copyright Barrett Adair 2015
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)

*/

#ifndef CLBL_CAN_DEREFERENCE_H
#define CLBL_CAN_DEREFERENCE_H

#include <utility>

#include <CLBL/is_valid.h>

namespace clbl {
    namespace detail {
        auto can_dereference_v = is_valid([](auto arg)->decltype(*arg) {});
    }

    /*
    clbl::can_dereference is a type trait that we use to determine whether
    a type is a pointer by checking whether we can dereference it.
    This allows us to treat smart pointers and raw pointers with the same code.
    */
    template<typename T>
    constexpr bool can_dereference = decltype(detail::can_dereference_v(std::declval<T>()))::value;
}

#endif