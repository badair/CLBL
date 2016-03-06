 /*!
@file

@copyright Barrett Adair 2015
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)

*/

#ifndef CLBL_CAN_CALL_WITH_HPP
#define CLBL_CAN_CALL_WITH_HPP

#include <cstdint>

namespace clbl {

    template<typename Callable, typename... Args>
    struct can_call_with
    {
        template<typename U>
        static std::int8_t test(decltype(std::declval<U>()(std::declval<Args>()...))*);

        template<typename>
        static std::int16_t test(...);

        static constexpr const bool value =
            sizeof(test<Callable>(nullptr)) == sizeof(std::int8_t);
    };

}

#endif