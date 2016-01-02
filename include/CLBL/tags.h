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
__CLBL_NO_CV represents an empty macro argument,
which is really useful for spamming qualifier
permutations with the preprocessor
*/

#define _______CLBL_NOTHING_ARGS(x)
#define __CLBL_NO_CV _______CLBL_NOTHING_ARGS(x)

namespace clbl {

    /*
    dummy is used whenever we need a type, but don't care what it is
    */
    struct dummy {};
    
    struct auto_ {};
    struct ambiguous_return {};
    struct ambiguous_args {};
    using ambiguous_type = ambiguous_return(ambiguous_args);

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