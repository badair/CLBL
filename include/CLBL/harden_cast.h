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

#include <CLBL/apply_qualifiers.h>
#include <CLBL/cv.h>
#include <CLBL/qualify_flags.h>

namespace clbl {

    template<typename T>
    static constexpr auto ref_qualifiers = 
        std::is_rvalue_reference<T>::value? rvalue_reference_ : lvalue_reference_;

    
    //! clbl::harden_cast is used internally to force desired qualify_flags on a reference
    template<qualify_flags CvFlags, typename Object>
    inline constexpr apply_qualifiers<std::remove_reference_t<Object>, CvFlags | cv<Object> | ref_qualifiers<Object> >
    harden_cast(Object&& o) {
        return  std::forward<Object>(o);
    }
}

#endif