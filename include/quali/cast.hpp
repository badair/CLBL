/*!
@file
Defines `quali::cast`.

@copyright Barrett Adair 2015
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)

*/

#ifndef QUALI_CAST_HPP
#define QUALI_CAST_HPP

namespace quali {
    
    template<typename T>
    struct cv_of;

    template<quali::flags QFlags, typename Object>
    inline constexpr qualified_type<
                typename std::remove_reference<Object>::type,
                guarantee_reference<QFlags>::value
            >
    cast(Object&& o) {
        return static_cast<qualified_type<
            typename std::remove_reference<Object>::type,
            guarantee_reference<QFlags>::value
        >>(o);
    }

    template<typename Object>
    inline constexpr qualified_type<
                typename std::remove_reference<Object>::type,
                cv_of<Object>::value | ref_of<Object, force_ref>::value
            >
    cast(Object&& o) {
        return static_cast<Object&&>(o);
    }
}

#endif