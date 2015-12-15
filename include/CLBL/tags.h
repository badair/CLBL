#ifndef CLBL_TAGS_H
#define CLBL_TAGS_H

#include <type_traits>

//for blank macro arguments - useful for cv/ref qualifier spam
#define CLBL_NOTHING_ARGS(x)
#define CLBL_NOTHING CLBL_NOTHING_ARGS(x)

namespace clbl {

    struct dummy {
        using underlying_type = dummy;
    };

    struct ambiguous_return {};
    struct ambiguous_args {};

    //todo - tag dispatch into safe_caller to extract deep constness and call member
    //todo - use tag dispatch in harden specializations

    struct default_harden_tag {};
    



    struct unknown_tag {};
    struct free_fn_tag {};
    struct pmf_tag {};
    struct pmf_ptr_tag {};
    struct fn_obj_tag {};
    struct ambi_fn_obj_tag {};
    struct fn_obj_ptr_tag {};
    struct ambi_fn_obj_ptr_tag {};



    // TODO use these?
    struct ptr_call_semantics {};
    struct value_call_semantics {};
    struct pmf_call_semantics {};
    struct ptr_pmf_call_semantics {};

    using qualify_flags = short;
    namespace qflags {
        constexpr qualify_flags default_ = 0;
        constexpr qualify_flags const_ = 1;
        constexpr qualify_flags volatile_ = 2;
        constexpr qualify_flags const_volatile_ = const_ | volatile_;
        constexpr qualify_flags lvalue_reference_ = 4;
        constexpr qualify_flags rvalue_reference_ = 8;



    }
}

#endif