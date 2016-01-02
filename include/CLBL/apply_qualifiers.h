/*!
@file
Defines `clbl::apply_qualifiers`.

@copyright Barrett Adair 2015
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)

*/

#ifndef CLBL_APPLY_QUALIFIERS_H
#define CLBL_APPLY_QUALIFIERS_H

#include <CLBL/qualify_flags.h>
#include <CLBL/tags.h>
#include <CLBL/tags.h>

namespace clbl {

    template<qualify_flags CvFlags>
    struct qualifiers {
        static_assert(CvFlags < 0, "Unknown cv-ness");
    };

    template<>
    struct qualifiers<default_> {
        template<typename T>
        using apply = T;
    };

    template<>
    struct qualifiers<const_> {
        template<typename T>
        using apply = const T;
    };

    template<>
    struct qualifiers<volatile_> {
        template<typename T>
        using apply = volatile T;
    };

    template<>
    struct qualifiers<const_ | volatile_> {
        template<typename T>
        using apply = const volatile T;
    };

    template<>
    struct qualifiers<lvalue_reference_> {
        template<typename T>
        using apply = T&;
    };

    template<>
    struct qualifiers<const_ | lvalue_reference_> {
        template<typename T>
        using apply = const T&;
    };

    template<>
    struct qualifiers<volatile_ | lvalue_reference_> {
        template<typename T>
        using apply = volatile T&;
    };

    template<>
    struct qualifiers<const_ | volatile_ | lvalue_reference_> {
        template<typename T>
        using apply = const volatile T&;
    };

    using no_q = qualifiers<default_>;
    using const_q = qualifiers<const_>;
    using volatile_q = qualifiers<volatile_>;
    using const_volatile_q = qualifiers<const_ | volatile_>;

    using ref_q = qualifiers<default_ | lvalue_reference_>;
    using const_ref_q = qualifiers<const_ | lvalue_reference_>;
    using volatile_ref_q = qualifiers<volatile_ | lvalue_reference_>;
    using const_volatile_ref_q = qualifiers<volatile_ | lvalue_reference_>;

    namespace apply_qualifiers_detail {
        template<typename U, qualify_flags Flags>
        struct apply_qualifiers_t {
            using T = no_ref<U>;
            using type = typename qualifiers<Flags>::template apply<T>;
        };
    }


    //! @typedef clbl::apply_qualifiers
    //! clbl::apply_qualifiers is a template alias that takes a type `T` and 
    //! a `qualify_flags` value. The qualifiers represented by the `qualify_flags`
    //! are added to `T`.
    //! Example
    //! -------
    //! @include apply_qualifiers.cpp
    #ifdef CLBL_DOCUMENTATION_BUILD
    template<typename T, qualify_flags Flags>
    using apply_qualifiers = ...;
    #else

    template<typename T, qualify_flags Flags>
    using apply_qualifiers = typename apply_qualifiers_detail::apply_qualifiers_t<T, Flags>::type;

    #endif
}

#endif