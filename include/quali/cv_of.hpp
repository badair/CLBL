/*!
@file
Defines `quali::cv_of`.

@copyright Barrett Adair 2015
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)

*/

#ifndef QUALI_CV_OF_HPP
#define QUALI_CV_OF_HPP

namespace quali {

    namespace cv_flags_detail {

        struct dummy_quali_type {
            static constexpr quali::flags q_flags = quali::default_;
        };

        template<typename T>
        struct has_cv_flags
        {
            template<typename>
            struct check {};

            template<typename U>
            static std::int8_t test(check<typename U::q_flags>*);

            template<typename>
            static std::int16_t test(...);

            static constexpr const bool value = 
                sizeof(test<T>(nullptr)) == sizeof(std::int8_t);
        };
    }

    template<typename T>
    struct cv_of {

        using cv_type = typename std::remove_reference<
            typename quali::generalized_object<T>::type
        >::type;

        static constexpr const auto constness =
            std::is_const<cv_type>::value ? quali::const_ : quali::default_;

        static constexpr const auto volatileness =
            std::is_volatile<cv_type>::value ? quali::volatile_ : quali::default_;

        using quali_type = std::conditional_t<
            cv_flags_detail::has_cv_flags<cv_type>::value,
            cv_type,
            cv_flags_detail::dummy_quali_type
        >;

        static constexpr const auto value =
            constness | volatileness | quali_type::q_flags;
    };
}

#endif