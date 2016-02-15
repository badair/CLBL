/*!
@file
Defines `clbl::has_normal_call_operator` and `clbl::ptr_has_normal_call_operator`.

@copyright Barrett Adair 2015
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)

*/

#ifndef CLBL_HAS_NORMAL_CALL_OPERATOR_H
#define CLBL_HAS_NORMAL_CALL_OPERATOR_H

#include <utility>

#include <CLBL/type_traits.h>
#include <CLBL/is_valid.h>

namespace clbl {

    template<typename T>
    struct has_normal_call_operator
    {
        template<typename U>
        static std::int8_t test(decltype(&no_ref<U>::operator()));

        template<typename U>
        static std::int16_t test(...);

        static constexpr const bool value = 
            sizeof(test<T>(nullptr)) == sizeof(std::int8_t);
    };

    template<typename T>
    struct ptr_has_normal_call_operator
    {
        template<typename U>
        static std::int8_t test(decltype(&no_ref<decltype(*std::declval<U>())>::operator()));

        template<typename>
        static std::int16_t test(...);

        static constexpr const bool value = 
            sizeof(test<T>(nullptr)) == sizeof(std::int8_t);
    };

}

#endif