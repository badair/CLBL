/*!
@file
Defines `clbl::qualified_type`.

@copyright Barrett Adair 2015
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)

*/

#ifndef CLBL_APPLY_QUALIFIERS_H
#define CLBL_APPLY_QUALIFIERS_H

#include <CLBL/qflags.hpp>
#include <CLBL/tags.hpp>
#include <CLBL/tags.hpp>

namespace clbl {

    template<qualify_flags QFlags>
    struct qualifiers {
        static_assert(QFlags < 0, "Unknown cv-ness");
    };

    template<>
    struct qualifiers<qflags::default_> {
        template<typename T>
        using apply = T;
    };

    template<>
    struct qualifiers<qflags::const_> {
        template<typename T>
        using apply = std::add_const_t<T>;
    };

    template<>
    struct qualifiers<qflags::volatile_> {
        template<typename T>
        using apply = std::add_volatile_t<T>;
    };

    template<>
    struct qualifiers<qflags::const_ | qflags::volatile_> {
        template<typename T>
        using apply = std::add_cv_t<T>;
    };

    template<>
    struct qualifiers<qflags::lvalue_reference_> {
        template<typename T>
        using apply = std::add_lvalue_reference_t<T>;
    };

    template<>
    struct qualifiers<qflags::const_ | qflags::lvalue_reference_> {
        template<typename T>
        using apply = std::add_lvalue_reference_t<std::add_const_t<T> >;
    };

    template<>
    struct qualifiers<qflags::volatile_ | qflags::lvalue_reference_> {
        template<typename T>
        using apply = std::add_lvalue_reference_t<std::add_volatile_t<T> >;
    };

    template<>
    struct qualifiers<qflags::const_ | qflags::volatile_ | qflags::lvalue_reference_> {
        template<typename T>
        using apply = std::add_lvalue_reference_t<std::add_cv_t<T> >;
    };

    template<>
    struct qualifiers<qflags::rvalue_reference_> {
        template<typename T>
        using apply = std::add_rvalue_reference_t<T>;
    };

    template<>
    struct qualifiers<qflags::const_ | qflags::rvalue_reference_> {
        template<typename T>
        using apply = std::add_rvalue_reference_t<std::add_const_t<T> >;
    };

    template<>
    struct qualifiers<qflags::volatile_ | qflags::rvalue_reference_> {
        template<typename T>
        using apply = std::add_rvalue_reference_t<std::add_volatile_t<T> >;
    };

    template<>
    struct qualifiers<qflags::const_ | qflags::volatile_ | qflags::rvalue_reference_> {
        template<typename T>
        using apply = std::add_rvalue_reference_t<std::add_cv_t<T> >;
    };
    
    //treat qflags::rvalue_reference_ | qflags::lvalue_reference_ as qflags::rvalue_reference_
    template<>
    struct qualifiers<qflags::rvalue_reference_ | qflags::lvalue_reference_> {
        template<typename T>
        using apply = std::add_rvalue_reference_t<T>;
    };

    template<>
    struct qualifiers<qflags::const_ | qflags::rvalue_reference_ | qflags::lvalue_reference_> {
        template<typename T>
        using apply = std::add_rvalue_reference_t<std::add_const_t<T> >;
    };

    // todo this is wrong - use the new features in qflags.hpp to
    // prevent this case
    template<>
    struct qualifiers<qflags::volatile_ | qflags::rvalue_reference_ | qflags::lvalue_reference_> {
        template<typename T>
        using apply = std::add_rvalue_reference_t<std::add_volatile_t<T> >;
    };

    template<>
    struct qualifiers<qflags::const_ | qflags::volatile_ | qflags::rvalue_reference_ | qflags::lvalue_reference_> {
        template<typename T>
        using apply = std::add_rvalue_reference_t<std::add_cv_t<T> >;
    };
    
    namespace apply_qualifiers_detail {
        template<typename T, qualify_flags Flags>
        struct apply_qualifiers_t {
            using type = typename qualifiers<Flags>::template apply<T>;
        };
    }


    //! @typedef clbl::qualified_type
    //! clbl::qualified_type is a template alias that takes a type `T` and 
    //! a `qualify_flags` value. The qualifiers represented by the `qualify_flags`
    //! are added to `T`.
    //! Example
    //! -------
    //! @include qualified_type.cpp
    #ifdef CLBL_DOCUMENTATION_BUILD
    template<typename T, qualify_flags Flags>
    using qualified_type = ...;
    #else

    template<typename T, qualify_flags Flags>
    using qualified_type = typename apply_qualifiers_detail::apply_qualifiers_t<T, Flags>::type;
    
    #endif
}

#endif