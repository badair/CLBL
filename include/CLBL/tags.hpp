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

namespace clbl {

    /*
    dummy is used whenever we need a type, but don't care what it is
    */
    struct dummy {};

    struct callable_dummy {
        void operator()(){}
    };

    struct auto_ {};
    struct copy_ {};
    
    struct invalid_type{};

    template<typename Category, typename Arg1, typename Arg2>
    struct key {
        using category = Category;
        using arg1 = Arg1;
        using arg2 = Arg2;
    };

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

    struct function_wrapper_tag{};
    struct function_object_wrapper_tag {};
    struct bound_pmf_tag{};
}

#endif