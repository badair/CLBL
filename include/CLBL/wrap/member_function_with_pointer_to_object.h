#ifndef CLBL_MEMBER_FUNCTION_WITH_POINTER_TO_OBJECT_H
#define CLBL_MEMBER_FUNCTION_WITH_POINTER_TO_OBJECT_H

#include <CLBL/utility.h>
#include <CLBL/tags.h>
#include <CLBL/qualify_flags.h>
#include <CLBL/member_function_decay.h>
#include <CLBL/wrappers/pmf_ptr_wrapper.h>

namespace clbl {

    struct member_function_with_pointer_to_object {

        template<qualify_flags Flags = qflags::default_, typename T, typename TMemberFnPtr>
        static inline constexpr auto
        wrap(TMemberFnPtr member_fn_ptr, T&& t) {
            constexpr auto cv_qualifiers = cv<T> | Flags;
            using object_type = no_ref<decltype(*std::declval<underlying_type<T> >())>;
            using ptr_type = no_ref<underlying_type<T> >;
            using decayed_fn = member_function_decay<TMemberFnPtr>;
            using wrapper = pmf_ptr_wrapper<member_function_with_pointer_to_object, 
                                cv_qualifiers, object_type, ptr_type, TMemberFnPtr, decayed_fn>;
            return wrapper{ member_fn_ptr, std::forward<T>(t) };
        }

        template<qualify_flags Flags = qflags::default_, typename Invocation>
        static inline constexpr auto
            wrap_data(Invocation&& data) {
            return wrap<Flags>(data.pmf, data.object_ptr);
        }
    };

}

#endif