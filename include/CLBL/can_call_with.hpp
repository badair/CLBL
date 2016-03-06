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

template<typename U, typename... Args>
struct can_call_with {

    template<typename T>
    static std::int8_t test(typename std::remove_reference<decltype(std::declval<T>()(std::declval<Args>()...))>::type*);

    template<typename T>
    static std::int16_t test(...);

    static constexpr bool value = //true;
        sizeof(decltype(can_call_with::test<U>(nullptr))) == sizeof(std::int8_t) ? (sizeof...(Args)) : 0;

    static constexpr int arg_count = sizeof...(Args);
};

template<typename U>
struct can_call_with<U, void> {

    template<typename T>
    static std::int8_t test(typename std::remove_reference<decltype(std::declval<T>()())>::type*);

    template<typename T>
    static std::int16_t test(...);

    static constexpr bool value = sizeof(can_call_with::test<U>(nullptr)) == sizeof(std::int8_t);
    static constexpr int arg_count = 0;
};

}

#endif