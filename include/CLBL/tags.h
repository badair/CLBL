/*!
@file
Defines tag structs used throughout CLBL.

@copyright Barrett Adair 2015
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)

*/

#ifndef CLBL_TAGS_H
#define CLBL_TAGS_H

#include <type_traits>

/*
CLBL_NO_CV represents an empty macro argument,
which is really useful for spamming qualifier
permutations with the preprocessor
*/

#define CLBL_NOTHING_ARGS(x)
#define CLBL_NO_CV CLBL_NOTHING_ARGS(x)

namespace clbl {

    /*
    dummy is used whenever we need a type, but don't care what it is
    */
    struct dummy {};
    struct auto_ {};
    struct copy_ {};
    
    template<typename T>
    struct universal_reference{ universal_reference() = delete; };
    struct ambiguous_return { ambiguous_return() = delete; };
    struct ambiguous_args { ambiguous_args() = delete; };

    using ambiguous_type = ambiguous_return(ambiguous_args);
    
    //todo - update tags for every wrapper and write tests
    struct free_fn_tag {};
    struct free_fn_ref_tag{};
    struct pmf_tag {};
    struct pmf_ptr_tag {};
    struct fn_obj_tag {};
    struct ambi_fn_obj_tag {};
    struct fn_obj_ptr_tag {};
    struct ambi_fn_obj_ptr_tag {};
}

#endif