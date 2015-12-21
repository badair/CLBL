#ifndef CLBL_MEMBER_FUNCTION_WITH_POINTER_TO_OBJECT_SLIM_H
#define CLBL_MEMBER_FUNCTION_WITH_POINTER_TO_OBJECT_SLIM_H

#include <CLBL/utility.h>
#include <CLBL/tags.h>
#include <CLBL/qualify_flags.h>
#include <CLBL/member_function_decay.h>
#include <CLBL/wrappers/pmf_ptr_wrapper_slim.h>

namespace clbl {

    //"slim" means it takes the PMF as a template argument
    struct member_function_with_pointer_to_object_slim {

        template<qualify_flags Flags = qflags::default_, typename TMemberFnPtr, TMemberFnPtr Pmf, typename T>
        static inline constexpr auto
        wrap(T&& t) {
            constexpr auto cv_qualifiers = cv<T> | Flags;
            using object_type = no_ref<decltype(*std::declval<T>())>;
            using ptr_type = no_ref<T>;
            using decayed_fn = member_function_decay<TMemberFnPtr>;
            using wrapper = pmf_ptr_wrapper_slim<member_function_with_pointer_to_object_slim, 
                                cv_qualifiers, object_type, ptr_type, TMemberFnPtr, Pmf, decayed_fn>;
            return wrapper{ std::forward<T>(t) };
        }

        template<qualify_flags Flags = qflags::default_, typename Invocation, typename TMemberFnPtr = decltype(Invocation::pmf), TMemberFnPtr Pmf = Invocation::pmf>
        static inline constexpr auto
            wrap_data(Invocation&& data) {
            return wrap<Flags, TMemberFnPtr, Pmf>(data.object_ptr);
        }
    };
}

#endif