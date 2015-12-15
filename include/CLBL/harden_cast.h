#ifndef CLBL_HARDEN_CAST_H
#define CLBL_HARDEN_CAST_H

#include "CLBL/apply_qualifiers.h"
#include "CLBL/cv.h"
#include "CLBL/qualify_flags.h"

namespace clbl {

    template<qualify_flags CvFlags, typename Object>
    inline constexpr auto 
        harden_cast(Object&& o) 
            -> apply_qualifiers<Object, CvFlags | cv<Object> | qflags::lvalue_reference_> {
        return std::forward<Object>(o);
    }
}

#endif