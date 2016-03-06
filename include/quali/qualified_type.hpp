/*!
@file

@copyright Barrett Adair 2015
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)

*/

#ifndef QUALI_QUALIFY_HPP
#define QUALI_QUALIFY_HPP

namespace quali {

    namespace detail {

        template<typename, quali::flags QFlags>
        struct qualified_type {
            using validated = typename validate_flags<QFlags>::type;
            static_assert(validated::value < 0, "Unexpected quali::flags value.");
        };

        template<typename T>
        struct qualified_type<T, 0 /*default*/> {
            using type = T;
        };

        template<typename T>
        struct qualified_type<T, 1 /*const*/> {
            using type = T const;
        };

        template<typename T>
        struct qualified_type<T, 2 /*volatile*/> {
            using type = T volatile;
        };

        template<typename T>
        struct qualified_type<T, 3 /*const volatile*/> {
            using type = T const volatile;
        };

        template<typename T>
        struct qualified_type<T, 4 /*&*/> {
            using type = typename std::add_lvalue_reference<T>::type;
        };

        template<typename T>
        struct qualified_type<T, 5 /*const &*/> {
            using type = std::add_lvalue_reference_t<T const>;
        };

        template<typename T>
        struct qualified_type<T, 6 /*volatile &*/> {
            using type = std::add_lvalue_reference_t<T volatile>;
        };

        template<typename T>
        struct qualified_type<T, 7 /*const volatile &*/> {
            using type = std::add_lvalue_reference_t<T const volatile>;
        };

        template<typename T>
        struct qualified_type<T, 8 /*&&*/> {
            using type = T&&;
        };

        template<typename T>
        struct qualified_type<T, 9 /*const &&*/> {
            using type = typename std::remove_reference<T>::type const &&;
        };

        template<typename T>
        struct qualified_type<T&, 9 /*const &&*/> {
            using type = T const &;
        };

        template<typename T>
        struct qualified_type<T, 10 /*volatile &&*/> {
            using type = typename std::remove_reference<T>::type volatile &&;
        };

        template<typename T>
        struct qualified_type<T&, 10 /*volatile &&*/> {
            using type = T volatile &;
        };

        template<typename T>
        struct qualified_type<T, 11 /*const volatile &&*/> {
            using type = typename std::remove_reference<T>::type const volatile &&;
        };

        template<typename T>
        struct qualified_type<T&, 11 /*const volatile &&*/> {
            using type = T const volatile &;
        };
    }

    template<typename T, quali::flags Flags>
    using qualify = typename detail::qualified_type<T, Flags>::type;
}

#endif