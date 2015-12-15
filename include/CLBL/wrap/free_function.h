#ifndef CLBL_FREE_FUNCTION_H
#define CLBL_FREE_FUNCTION_H

#include "CLBL/member_function_decay.h"
#include "CLBL/utility.h"
#include "CLBL/wrappers/free_fn_wrapper.h"

namespace clbl {

    struct free_function {

        template<qualify_flags Flags, typename T>
        static inline constexpr auto
        wrap(T&& t, dummy d = dummy{}) {
            using function_type = no_ref<std::remove_pointer_t<T> >;
            using wrapper = free_fn_wrapper<free_function, function_type>;
            return wrapper{ std::forward<T>(t) };
        }

        static constexpr bool has_member_function_pointer = false;
    };

}

#endif