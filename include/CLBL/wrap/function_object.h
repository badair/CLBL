#ifndef CLBL_FUNCTION_OBJECT_H
#define CLBL_FUNCTION_OBJECT_H

#include "CLBL/member_function_decay.h"
#include "CLBL/utility.h"
#include "CLBL/wrap/member_function_with_object.h"
#include "CLBL/wrappers/ambi_fn_obj_wrapper.h"

namespace clbl {

    //todo implement "slim" version of pmf_wrapper that doesn't store a PMF
    struct function_object {

        template<qualify_flags Flags, typename T>
        static inline constexpr auto 
        wrap(T&& t) {
            constexpr auto member_fn = &underlying_type<no_ref<T> >::operator();
            return member_function_with_object::wrap<Flags>(member_fn, std::forward<T>(t));
        }

        template<qualify_flags Flags, typename Invocation>
        static inline constexpr auto
            wrap_data(Invocation data) {
            return wrap(data.object);

        }
        static constexpr bool has_member_function_pointer = false;

        struct ambiguous {

            template<qualify_flags Flags, typename T>
            static inline constexpr auto
                wrap(T&& t) {
                constexpr auto cv_qualifiers = cv<T> | Flags;
                using wrapper = ambi_fn_obj_wrapper<function_object::ambiguous, cv_qualifiers, no_ref<T> >;
                return wrapper{ std::forward<T>(t) };
            }

            template<qualify_flags Flags, typename Invocation>
            static inline constexpr auto
                wrap_data(Invocation data) {
                return wrap<Flags>(data.object);
            }

            static constexpr bool has_member_function_pointer = false;
        };
    };
}

#endif