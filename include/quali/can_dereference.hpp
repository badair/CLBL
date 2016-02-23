/*!
@file
Defines `quali::can_dereference`.

@copyright Barrett Adair 2015
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)

*/

#ifndef QUALI_CAN_DEREFERENCE_HPP
#define QUALI_CAN_DEREFERENCE_HPP

namespace quali {

    /*
    quali::can_dereference is a type trait that we use to determine whether
    a type is a pointer by checking whether we can dereference it.
    This allows us to treat smart pointers and raw pointers with the same code.
    */

    template<typename T>
    struct can_dereference
    {
        template<typename>
        struct check {};

        template<typename U>
        static std::int8_t test(check<decltype(*std::declval<U>())>*);

        template<typename>
        static std::int16_t test(...);

        static constexpr const bool value = 
            sizeof(test<T>(nullptr)) == sizeof(std::int8_t);
    };
}

#endif