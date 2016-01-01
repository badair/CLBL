/*!
@file
Defines `clbl::is_reference_wrapper`.

@copyright Barrett Adair 2015
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)

*/

#ifndef CLBL_IS_REFERENCE_WRAPPER_H
#define CLBL_IS_REFERENCE_WRAPPER_H

namespace clbl {

    namespace detail {

        template<typename T>
        struct is_reference_wrapper_t : std::false_type {};

        template <typename T>
        struct is_reference_wrapper_t<std::reference_wrapper<T> > : std::true_type {};
    }

    template<typename T>
	constexpr bool is_reference_wrapper = detail::is_reference_wrapper_t<T>::value;
}

#endif