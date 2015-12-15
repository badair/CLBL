#ifndef CLBL_CV_H
#define CLBL_CV_H

#include "CLBL/qualify_flags.h"
#include "CLBL/utility.h"

namespace clbl {

    namespace detail {

        namespace cv_flags_detail {
            struct dummy_clbl_type {
                static constexpr qualify_flags cv_flags = qflags::default_;
            };
        }

        template<typename T>
        inline constexpr qualify_flags cv_flags() {
            constexpr auto can_deref = can_dereference<T>;
            using derferenceable = std::conditional_t<can_deref, T, int*>;
            using cv_type = no_ref<std::conditional_t<can_deref, decltype(*std::declval<derferenceable>()), T> >;
            constexpr auto constness = std::is_const<cv_type>::value ? qflags::const_ : qflags::default_;
            constexpr auto volatileness = std::is_volatile<cv_type>::value ? qflags::volatile_ : qflags::default_;
            using clbl_type = std::conditional_t<already_has_cv_flags<cv_type>, cv_type, cv_flags_detail::dummy_clbl_type>;
            return constness | volatileness | clbl_type::cv_flags;
        }
    }

    template<typename T>
    constexpr qualify_flags cv = detail::cv_flags<T>();
}

#endif