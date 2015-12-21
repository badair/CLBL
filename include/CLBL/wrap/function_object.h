#ifndef CLBL_FUNCTION_OBJECT_H
#define CLBL_FUNCTION_OBJECT_H

#include <CLBL/member_function_decay.h>
#include <CLBL/utility.h>
#include <CLBL/wrappers/pmf_wrapper_slim.h>
#include <CLBL/wrappers/casted_fn_obj_wrapper.h>
#include <CLBL/wrappers/ambi_fn_obj_wrapper.h>

namespace clbl {

    struct function_object {

        template<qualify_flags Flags, typename T>
        static inline constexpr auto 
        wrap(T&& t) {
            constexpr auto cv_qualifiers = cv<no_ref<T> > | Flags;
            using member_fn_type = decltype(&underlying_type<no_ref<T> >::operator());
            using decayed_fn = member_function_decay<member_fn_type>;
            using wrapper = pmf_wrapper_slim<function_object,
                cv_qualifiers, no_ref<T>, member_fn_type, &underlying_type<no_ref<T> >::operator(), decayed_fn>;
            return wrapper{ std::forward<T>(t)};
        }

        template<qualify_flags Flags, typename Invocation>
        static inline constexpr auto
            wrap_data(Invocation&& data) {
            return wrap<Flags>(data.object);
        }

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
                wrap_data(Invocation&& data) {
                return wrap<Flags>(data.object);
            }
        };

        struct casted {

            template<qualify_flags Flags, typename TMemberFnPtr, typename T>
            static inline constexpr auto
                wrap(T&& t) {
                constexpr auto cv_qualifiers = cv<no_ref<T> > | Flags;
                using decayed_fn = member_function_decay<TMemberFnPtr>;
                using wrapper = casted_fn_obj_wrapper<typename function_object::casted,
                    cv_qualifiers, underlying_type<no_ref<T> >, TMemberFnPtr, decayed_fn>;
                return wrapper{ std::forward<T>(t) };
            }

            template<qualify_flags Flags, typename Invocation>
            static inline constexpr auto
                wrap_data(Invocation&& data) {
                return wrap<Flags, decltype(no_ref<Invocation>::pmf)>(data.object);
            }
        };
    };
}

#endif