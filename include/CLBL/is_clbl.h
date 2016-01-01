/*!
@file
Defines `clbl::is_clbl`.

@copyright Barrett Adair 2015
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)

*/

#ifndef CLBL_IS_CLBL_H
#define CLBL_IS_CLBL_H

#include <CLBL/is_valid.h>

namespace clbl {

    namespace detail {
        auto has_clbl_tag_v = is_valid([](auto arg)-> typename decltype(arg)::clbl_tag{});
    }

    /*
    clbl::is_clbl is a type trait to determine whether a type
    is a CLBL wrapper.
    */
    template<typename T>
    constexpr auto is_clbl = decltype(detail::has_clbl_tag_v(std::declval<T>()))::value;
}

#endif