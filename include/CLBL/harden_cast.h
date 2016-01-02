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

    
    //! clbl::harden_cast is used internally to force desired qualify_flags on a reference
    template<qualify_flags CvFlags, typename Object>
    inline constexpr auto 
        harden_cast(Object&& o) 
            -> apply_qualifiers<Object, CvFlags | cv<Object> | lvalue_reference_> {
        return std::forward<Object>(o);
    }
}

#endif