#ifndef CLBL_POINTER_TO_FUNCTION_OBJECT_H
#define CLBL_POINTER_TO_FUNCTION_OBJECT_H

#include "CLBL/utility.h"
#include "CLBL/tags.h"
#include "CLBL/qualify_flags.h"
#include "CLBL/member_function_decay.h"
#include "CLBL/wrap/member_function_with_pointer_to_object.h"
#include "CLBL/wrappers/ambi_fn_obj_ptr_wrapper.h"

namespace clbl {

    //todo make "slim" version of pmf_ptr_wrapper that takes the member_fn as a template argument
    struct pointer_to_function_object {

        template<qualify_flags Flags, typename T>
        static inline constexpr auto 
        wrap(T&& t, dummy d = dummy{}) {
            using object_type = no_ref<decltype(*std::declval<T>())>;
            constexpr auto member_fn = &object_type::operator();
            return member_function_with_pointer_to_object::wrap<Flags>(member_fn, std::forward<T>(t));
        }

        template<qualify_flags Flags, typename Invocation>
        static inline constexpr auto
            wrap_data(Invocation data) {
            return wrap<Flags>(data.ptr);
        }

        static constexpr bool has_member_function_pointer = false;

        struct ambiguous {

            template<qualify_flags Flags, typename T>
            static inline constexpr auto
            wrap(T&& t) {
                constexpr auto cv_qualifiers = cv<T> | Flags;
                using wrapper = ambi_fn_obj_ptr_wrapper<pointer_to_function_object::ambiguous, cv_qualifiers, no_ref<T> >;
                return wrapper{std::forward<T>(t)};
            }

            template<qualify_flags Flags, typename Invocation>
            static inline constexpr auto
                wrap_data(Invocation data) {
                return wrap<Flags>(data.ptr);
            }

            static constexpr bool has_member_function_pointer = false;
        };
    };

}

#endif