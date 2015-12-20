#ifndef CLBL_MEMBER_FUNCTION_WITH_OBJECT_H
#define CLBL_MEMBER_FUNCTION_WITH_OBJECT_H

#include "CLBL/utility.h"
#include "CLBL/tags.h"
#include "CLBL/qualify_flags.h"
#include "CLBL/member_function_decay.h"
#include "CLBL/wrappers/pmf_wrapper.h"

namespace clbl {

    //todo make "slim" version that takes the member_fn as a template argument
    struct member_function_with_object {

        template<qualify_flags Flags, typename T, typename TMemberFnPtr>
        static inline constexpr auto 
        wrap(TMemberFnPtr member_fn_ptr, T&& t) {
            constexpr auto cv_qualifiers = cv<T> | Flags;
            using decayed_fn = member_function_decay<no_ref<TMemberFnPtr> >;
            using wrapper = pmf_wrapper<member_function_with_object, cv_qualifiers, no_ref<T>, no_ref<TMemberFnPtr>, decayed_fn>;
            return wrapper{ member_fn_ptr, std::forward<T>(t) };
        }

        template<qualify_flags Flags, typename Invocation>
        static inline constexpr auto
            wrap_data(Invocation data) {
            return wrap<Flags>(data.pmf, data.object);
        }

        static constexpr bool has_member_function_pointer = true;
    };
}

#endif