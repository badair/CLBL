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
#include <CLBL/cv_of.h>
#include <CLBL/qflags.h>

namespace clbl {

    struct force_ref{};
    //todo move
    template<typename T, typename E = void>
    struct ref_of { 

        static constexpr const qualify_flags value = 
            std::is_rvalue_reference<T>::value? qflags::rvalue_reference_ 
                : (std::is_lvalue_reference<T>::value ? qflags::lvalue_reference_ 
                    : (std::is_same<E, force_ref>::value? qflags::lvalue_reference_
                            : qflags::default_));
    };
    
    //! clbl::harden_cast is used internally to force desired qualify_flags on a reference
    template<qualify_flags QFlags, typename Object>
    inline constexpr qualified_type<
                no_ref<Object>,
                QFlags | cv_of<Object>::value | ref_of<Object, force_ref>::value
            >
    harden_cast(Object&& o) {
        return static_cast<Object&&>(o);
    }
}

#endif