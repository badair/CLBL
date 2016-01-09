/*!
@file
Defines `clbl::harden_cast`.

@copyright Barrett Adair 2015
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)

*/

#ifndef CLBL_HARDEN_CAST_H
#define CLBL_HARDEN_CAST_H

#include <utility>

#include <CLBL/qualified_type.h>
#include <CLBL/cv.h>
#include <CLBL/qflags.h>

namespace clbl {

    //todo move
    template<typename T>
    static constexpr auto reference_qualifiers = 
    std::is_rvalue_reference<T>::value? qflags::rvalue_reference_ : 
        (std::is_lvalue_reference<T>::value ? qflags::lvalue_reference_ : qflags::default_);
    
    //todo what is this? rename and move
    template<typename T>
    static constexpr auto ref_qualifiers = 
        std::is_rvalue_reference<T>::value? qflags::rvalue_reference_ : qflags::lvalue_reference_;

    
    //! clbl::harden_cast is used internally to force desired qualify_flags on a reference
    template<qualify_flags QFlags, typename Object>
    inline constexpr qualified_type<std::remove_reference_t<Object>, QFlags | cv<Object> | ref_qualifiers<Object> >
    harden_cast(Object&& o) {
        return  static_cast<Object&&>(o);
    }
}

#endif