/*!
@file
Defines `clbl::has_normal_call_operator`.

@copyright Barrett Adair 2015
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)

*/

#ifndef CLBL_HAS_NORMAL_CALL_OPERATOR_H
#define CLBL_HAS_NORMAL_CALL_OPERATOR_H

#include <utility>

#include <CLBL/type_traits.hpp>

namespace clbl {

    //bending over backwards for MSVC here

    template<typename T>
    struct has_normal_call_operator
    {
        template<typename N, N Value> struct check{ check(std::nullptr_t){} };

        template<typename U>
        static std::int8_t test(check<decltype(&U::operator()), &U::operator()>);

        template<typename>
        static std::int16_t test(...);

        static constexpr const bool value = 
            sizeof(test<T>(nullptr)) == sizeof(std::int8_t);
    };
}

#endif