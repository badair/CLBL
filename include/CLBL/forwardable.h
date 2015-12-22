/*

Copyright Barrett Adair 2015
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)

*/

#ifndef CLBL_FORWARDABLE_H
#define CLBL_FORWARDABLE_H

namespace clbl {

    namespace detail {
        template<typename T>
        struct forwardable_t { using type = const T&; };

        template<typename T>
        struct forwardable_t<T&&> { using type = T&&; };

        template<typename T>
        struct forwardable_t<T&> { using type = T&; };
    }

    template<typename T>
    using forwardable = typename detail::forwardable_t<T>::type;

    /* RVO makes this unnecessary...
    namespace detail {
        template<typename T>
        struct return_forwardable_t { using type = T&&; };

        template<typename T>
        struct return_forwardable_t<T&> { using type = T&; };

        template<>
        struct return_forwardable_t<void> { using type = void; };
    }

    template<typename T>
    using return_forwardable = typename detail::return_forwardable_t<T>::type;
    */

}

#endif