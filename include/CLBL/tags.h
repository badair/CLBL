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
    struct dummy {
        using underlying_type = dummy;
    };
    
    struct auto_ {};
    struct ambiguous_return {};
    struct ambiguous_args {};

    struct unknown_tag {};
    struct free_fn_tag {};
    struct pmf_tag {};
    struct pmf_ptr_tag {};
    struct fn_obj_tag {};
    struct ambi_fn_obj_tag {};
    struct fn_obj_ptr_tag {};
    struct ambi_fn_obj_ptr_tag {};
}

#endif