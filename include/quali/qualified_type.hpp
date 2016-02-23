/*!
@file
Defines `quali::qualified_type`.

@copyright Barrett Adair 2015
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)

*/

#ifndef QUALI_APPLY_QUALIFIERS_HPP
#define QUALI_APPLY_QUALIFIERS_HPP

namespace quali {

    template<quali::flags QFlags>
    struct qualifiers {
        static_assert(QFlags < 0, "Unknown cv-ness");
    };

    template<>
    struct qualifiers<quali::default_> {
        template<typename T>
        using apply = T;
    };

    template<>
    struct qualifiers<quali::const_> {
        template<typename T>
        using apply = std::add_const_t<T>;
    };

    template<>
    struct qualifiers<quali::volatile_> {
        template<typename T>
        using apply = std::add_volatile_t<T>;
    };

    template<>
    struct qualifiers<quali::const_ | quali::volatile_> {
        template<typename T>
        using apply = std::add_cv_t<T>;
    };

    template<>
    struct qualifiers<quali::lvalue_reference_> {
        template<typename T>
        using apply = std::add_lvalue_reference_t<T>;
    };

    template<>
    struct qualifiers<quali::const_ | quali::lvalue_reference_> {
        template<typename T>
        using apply = std::add_lvalue_reference_t<std::add_const_t<T> >;
    };

    template<>
    struct qualifiers<quali::volatile_ | quali::lvalue_reference_> {
        template<typename T>
        using apply = std::add_lvalue_reference_t<std::add_volatile_t<T> >;
    };

    template<>
    struct qualifiers<quali::const_ | quali::volatile_ | quali::lvalue_reference_> {
        template<typename T>
        using apply = std::add_lvalue_reference_t<std::add_cv_t<T> >;
    };

    template<>
    struct qualifiers<quali::rvalue_reference_> {
        template<typename T>
        using apply = std::add_rvalue_reference_t<T>;
    };

    template<>
    struct qualifiers<quali::const_ | quali::rvalue_reference_> {
        template<typename T>
        using apply = std::add_rvalue_reference_t<std::add_const_t<T> >;
    };

    template<>
    struct qualifiers<quali::volatile_ | quali::rvalue_reference_> {
        template<typename T>
        using apply = std::add_rvalue_reference_t<std::add_volatile_t<T> >;
    };

    template<>
    struct qualifiers<quali::const_ | quali::volatile_ | quali::rvalue_reference_> {
        template<typename T>
        using apply = std::add_rvalue_reference_t<std::add_cv_t<T> >;
    };
    
    //treat quali::rvalue_reference_ | quali::lvalue_reference_ as quali::rvalue_reference_
    template<>
    struct qualifiers<quali::rvalue_reference_ | quali::lvalue_reference_> {
        template<typename T>
        using apply = std::add_rvalue_reference_t<T>;
    };

    template<>
    struct qualifiers<quali::const_ | quali::rvalue_reference_ | quali::lvalue_reference_> {
        template<typename T>
        using apply = std::add_rvalue_reference_t<std::add_const_t<T> >;
    };

    // todo this is wrong - use the new features in qflags.hpp to
    // prevent this case
    template<>
    struct qualifiers<quali::volatile_ | quali::rvalue_reference_ | quali::lvalue_reference_> {
        template<typename T>
        using apply = std::add_rvalue_reference_t<std::add_volatile_t<T> >;
    };

    template<>
    struct qualifiers<quali::const_ | quali::volatile_ | quali::rvalue_reference_ | quali::lvalue_reference_> {
        template<typename T>
        using apply = std::add_rvalue_reference_t<std::add_cv_t<T> >;
    };


    namespace detail {
        template<typename T, quali::flags Flags>
        struct qualified_type_helper {
            QUALI_ASSERT_VALID_REFERENCE_FLAGS(Flags);
            using type = typename qualifiers<Flags>::template apply<T>;
        };
    }

    template<typename T, quali::flags Flags>
    using qualified_type = typename detail::qualified_type_helper<T, Flags>::type;
}

#endif