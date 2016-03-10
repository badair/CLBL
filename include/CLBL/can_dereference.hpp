/*!
@file
Defines `clbl::can_dereference`.

@copyright Barrett Adair 2015
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)

*/

#ifndef CLBL_CAN_DEREFERENCE_HPP
#define CLBL_CAN_DEREFERENCE_HPP

#include <CLBL/is_reference_wrapper.hpp>

namespace clbl {

    /*
    clbl::can_dereference is a type trait that we use to determine whether
    a type is a pointer by checking whether we can dereference it.
    This allows us to treat smart pointers and raw pointers with the same code.
    */
    namespace detail {

        template<typename T>
        struct can_dereference_t
        {
            template<typename>
            struct check {};

            template<typename U>
            static std::int8_t test(check<typename std::remove_reference<decltype(*std::declval<U>())>::type>*);

            template<typename>
            static std::int16_t test(...);

            static constexpr const bool value = 
                sizeof(test<T>(nullptr)) == sizeof(std::int8_t);
        };

        template<typename T>
        struct is_class_after_dereference
        {
            template<typename>
            struct check {};

            template<typename U>
            static typename std::enable_if<
                std::is_class<typename std::remove_reference<decltype(*std::declval<U>())>::type>{}, std::int8_t
            >::type test(std::nullptr_t);

            template<typename>
            static std::int16_t test(...);

            static constexpr const bool value = 
                sizeof(test<T>(nullptr)) == sizeof(std::int8_t);
        };
    }

    template<typename T>
    using can_dereference = std::integral_constant<bool,
        detail::can_dereference_t<T>::value
    >;

    template<typename T>
    using can_dereference_object_when_volatile = std::integral_constant<bool,
        detail::is_class_after_dereference<typename std::add_volatile<T>::type>::value
    >;

    template<typename T>
    using can_dereference_object_only_when_non_volatile = std::integral_constant<bool,
        !detail::is_class_after_dereference<typename std::add_volatile<T>::type>::value
        && detail::is_class_after_dereference<typename std::remove_volatile<T>::type>::value
    >;

    template<typename T>
    using cannot_dereference_object = std::integral_constant<bool,
        !is_reference_wrapper<T>::value && (!detail::can_dereference_t<T>::value
        || !detail::is_class_after_dereference<T>::value)
    >;
}

#endif