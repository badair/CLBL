#ifndef CLBL_APPLY_QUALIFIERS_H
#define CLBL_APPLY_QUALIFIERS_H

#include "CLBL/qualify_flags.h"
#include "CLBL/utility.h"

namespace clbl {
    template<qualify_flags CvFlags>
    struct qualifiers {
        static_assert(CvFlags < 0, "Unknown cv-ness");
    };

    template<>
    struct qualifiers<qflags::default_> {
        template<typename T>
        using apply = T;
    };

    template<>
    struct qualifiers<qflags::const_> {
        template<typename T>
        using apply = const T;
    };

    template<>
    struct qualifiers<qflags::volatile_> {
        template<typename T>
        using apply = volatile T;
    };

    template<>
    struct qualifiers<qflags::const_ | qflags::volatile_> {
        template<typename T>
        using apply = const volatile T;
    };

    template<>
    struct qualifiers<qflags::lvalue_reference_> {
        template<typename T>
        using apply = T&;
    };

    template<>
    struct qualifiers<qflags::const_ | qflags::lvalue_reference_> {
        template<typename T>
        using apply = const T&;
    };

    template<>
    struct qualifiers<qflags::volatile_ | qflags::lvalue_reference_> {
        template<typename T>
        using apply = volatile T&;
    };

    template<>
    struct qualifiers<qflags::const_ | qflags::volatile_ | qflags::lvalue_reference_> {
        template<typename T>
        using apply = const volatile T&;
    };

    using no_q = qualifiers<qflags::default_>;
    using const_q = qualifiers<qflags::const_>;
    using volatile_q = qualifiers<qflags::volatile_>;
    using const_volatile_q = qualifiers<qflags::const_ | qflags::volatile_>;

    using ref_q = qualifiers<qflags::default_ | qflags::lvalue_reference_>;
    using const_ref_q = qualifiers<qflags::const_ | qflags::lvalue_reference_>;
    using volatile_ref_q = qualifiers<qflags::volatile_ | qflags::lvalue_reference_>;
    using const_volatile_ref_q = qualifiers<qflags::volatile_ | qflags::lvalue_reference_>;

    namespace apply_qualifiers_detail {

        struct dummy_clbl_type {
            template<qualify_flags Flags>
            using apply_cv = std::nullptr_t;
        };

        template<typename U, qualify_flags Flags>
        struct apply_qualifiers_t {
            using T = no_ref<U>;
            //using clbl_type = std::conditional_t<is_clbl<T>, T, dummy_clbl_type>;
            //using type = std::conditional_t<is_clbl<T>, typename clbl_type::apply_cv<Flags>, typename qualifiers<Flags>::apply<T> >;
            using type = typename qualifiers<Flags>::apply<T>;
        };
    }

    template<typename T, qualify_flags Flags>
    using apply_qualifiers = typename apply_qualifiers_detail::apply_qualifiers_t<T, Flags>::type;
}

#endif