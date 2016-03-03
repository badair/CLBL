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

namespace detail {
    
    template<typename T, typename = void>
    struct basic_generalized_object {
        using type = T;
    };

    template<typename T>
    struct basic_generalized_object<dereferenceable_object<T>, void> {
        using type = std::remove_reference_t<std::remove_pointer_t<T>>;
    };

    template<typename T>
    struct basic_generalized_object<std::reference_wrapper<T>, void> {
        using type = T;
    };

    struct dummy_quali_type {
        static constexpr flags q_flags = default_;
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
        typename detail::basic_generalized_object<T>::type
    >::type;

    static constexpr const auto constness =
        std::is_const<cv_type>::value ? const_ : default_;

    static constexpr const auto volatileness =
        std::is_volatile<cv_type>::value ? volatile_ : default_;

    using quali_type = std::conditional_t<
        detail::has_cv_flags<cv_type>::value,
        cv_type,
        detail::dummy_quali_type
    >;

    static constexpr const auto value =
        constness | volatileness | quali_type::q_flags;
};

}

#endif