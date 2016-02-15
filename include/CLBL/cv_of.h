/*!
@file
Defines `clbl::cv_of`.

@copyright Barrett Adair 2015
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)

*/

#ifndef CLBL_CV_H
#define CLBL_CV_H

#include <type_traits>
#include <utility>

#include <CLBL/tags.h>
#include <CLBL/type_traits.h>
#include <CLBL/is_valid.h>
#include <CLBL/qflags.h>
#include <CLBL/can_dereference.h>


namespace clbl {

    /*
    clbl::cv_of is a variable template that determines
    the appropriate qualify_flags for a type.
    */

    namespace cv_flags_detail {

        struct dummy_clbl_type {
            static constexpr qualify_flags q_flags = qflags::default_;
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

        static constexpr const auto can_deref = can_dereference<T>::value;

        using dereferenceable = std::conditional_t<can_deref, T, dummy*>;
        using dereferenced = decltype(*std::declval<dereferenceable>());
        using cv_type = no_ref<std::conditional_t<can_deref, dereferenced, T> >;

        static constexpr const auto constness =
            std::is_const<cv_type>::value ? qflags::const_ : qflags::default_;

        static constexpr const auto volatileness =
            std::is_volatile<cv_type>::value ? qflags::volatile_ : qflags::default_;

        using clbl_type = std::conditional_t<
            cv_flags_detail::has_cv_flags<cv_type>::value,
            cv_type,
            cv_flags_detail::dummy_clbl_type
        >;

        static constexpr const auto value =
            constness | volatileness | clbl_type::q_flags;
    };
}

#endif