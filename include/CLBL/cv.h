/*!
@file
Defines `clbl::cv`.

@copyright Barrett Adair 2015
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)

*/

#ifndef CLBL_CV_H
#define CLBL_CV_H

#include <type_traits>
#include <utility>

#include <CLBL/tags.h>
#include <CLBL/no_ref.h>
#include <CLBL/is_valid.h>
#include <CLBL/qualify_flags.h>
#include <CLBL/can_dereference.h>


namespace clbl {

    /*
    clbl::cv is a variable template (at the bottom of this file)
    that determines the appropriate qualify_flags for a type.
    */

    namespace detail {
        namespace cv_flags_detail {
            struct dummy_clbl_type {
                static constexpr qualify_flags cv_flags = default_;
            };
        }

        static auto has_cv_flags_t = is_valid([](auto c) -> decltype(decltype(c)::cv_flags) {});

        template<typename T>
        static constexpr auto has_cv_flags = decltype(has_cv_flags_t(std::declval<T>()))::value;

        template<typename T>
        inline constexpr qualify_flags cv_flags() {
            using namespace cv_flags_detail;

            constexpr auto can_deref = can_dereference<T>;
            using dereferenceable = std::conditional_t<can_deref, T, dummy*>;
            using dereferenced = decltype(*std::declval<dereferenceable>());
            using cv_type = no_ref<std::conditional_t<can_deref, dereferenced, T> >;
            constexpr auto constness = std::is_const<cv_type>::value ? const_ : default_;
            constexpr auto volatileness = std::is_volatile<cv_type>::value ? volatile_ : default_;
            using clbl_type = std::conditional_t<has_cv_flags<cv_type>, cv_type, dummy_clbl_type>;
            return constness | volatileness | clbl_type::cv_flags;
        }
    }

    template<typename T>
    constexpr qualify_flags cv = detail::cv_flags<T>();
}

#endif