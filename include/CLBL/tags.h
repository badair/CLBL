#ifndef CLBL_TAGS_H
#define CLBL_TAGS_H

#include <type_traits>

#define CLBL_NOTHING_ARGS(x)
#define CLBL_NOTHING CLBL_NOTHING_ARGS(x)

namespace clbl {

    struct ambiguous_return {};
    struct ambiguous_args {};

    //todo - tag dispatch into safe_caller to extract deep constness and call member
    //todo - use tag dispatch in harden specializations as well?
    //todo - rename things
    struct free_fn_tag {};
    struct pmf_tag {};
    struct pmf_indirect_tag {};
    struct fn_object_tag {};
    struct ambi_fn_object_tag {};
    struct fn_object_indirect_tag {};
    struct ambi_fn_object_indirect_tag {};
}

#endif