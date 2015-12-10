#ifndef CLBL_TAGS_H
#define CLBL_TAGS_H

#include <type_traits>

//for blank macro arguments - useful for cv/ref qualifier spam
#define CLBL_NOTHING_ARGS(x)
#define CLBL_NOTHING CLBL_NOTHING_ARGS(x)

namespace clbl {

    struct ambiguous_return {};
    struct ambiguous_args {};

    //todo - tag dispatch into safe_caller to extract deep constness and call member
    //todo - use tag dispatch in harden specializations

    struct default_harden_tag {};
    
    struct free_fn_tag {};
    struct pmf_tag {};
    struct pmf_ptr_tag {};
    struct fn_obj_tag {};
    struct ambi_fn_obj_tag {};
    struct fn_obj_ptr_tag {};
    struct ambi_fn_obj_ptr_tag {};

    struct ptr_call_semantics {};
    struct value_call_semantics {};
    struct pmf_call_semantics {};
    struct ptr_pmf_call_semantics {};

    namespace qual {
        struct default_ {};
        struct const_ {};
        struct volatile_ {};
        struct const_volatile_ {};
    }
}

#endif