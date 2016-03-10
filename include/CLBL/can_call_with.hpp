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

namespace detail {
    template<typename U, typename... Args>
    struct can_call_with_t {

        struct substitution_failed{};

        template<typename T>
        static decltype(std::declval<T>()(std::declval<Args>()...)) test(std::nullptr_t);

        template<typename T>
        static substitution_failed test(...);

        static constexpr bool value = !std::is_same<
            substitution_failed,
            decltype(can_call_with_t::test<U>(nullptr))
        >{};

        static constexpr int arg_count = sizeof...(Args);
    };

    template<typename U>
    struct can_call_with_t<U, void> {

        template<typename T>
        static decltype(std::declval<T&&>()()) test(std::nullptr_t);

        struct substitution_failed {};

        template<typename T>
        static substitution_failed test(...);

        static constexpr bool value = !std::is_same<
            substitution_failed,
            decltype(can_call_with_t::test<U>(nullptr))
        >{};

        static constexpr int arg_count = 0;
    };
}

template<typename T, typename... Args>
using can_call_with = std::integral_constant<bool, detail::can_call_with_t<T, Args...>::value>;

}

#endif