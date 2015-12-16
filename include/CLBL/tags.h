#ifndef CLBL_TAGS_H
#define CLBL_TAGS_H

#include <type_traits>

//for blank macro arguments - useful for cv/ref qualifier spam
#define CLBL_NOTHING_ARGS(x)
#define CLBL_NOTHING CLBL_NOTHING_ARGS(x)

namespace clbl {

    //dummy is used whenever we need a type, but don't care what it is
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

    // TODO use these to help clean up the _object/_value mess
    struct ptr_call_semantics {};
    struct value_call_semantics {};
    struct pmf_call_semantics {};
    struct ptr_pmf_call_semantics {};
}

#endif