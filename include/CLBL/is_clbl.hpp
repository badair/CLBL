/*!
@file
Defines `clbl::is_clbl`.

@copyright Barrett Adair 2015
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)

*/

#ifndef CLBL_IS_CLBL_H
#define CLBL_IS_CLBL_H

#include <cstdint>
#include <type_traits>

namespace clbl {

    /* todo - move this to CLBL/type_traits.hpp
    clbl::is_clbl is a type trait to determine whether a type
    is a CLBL wrapper.
    */
    template<typename T>
    struct is_clbl
    {
        template<typename>
        struct check {};

        template<typename U>
        static std::int8_t test(check<typename std::remove_cv_t<std::remove_reference_t<U>>::clbl_tag>*);

        template<typename>
        static std::int16_t test(...);

        static constexpr const bool value = 
            sizeof(test<T>(nullptr)) == sizeof(std::int8_t);
    };
}

#endif