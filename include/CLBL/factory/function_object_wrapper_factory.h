/*!
@file
Defines `clbl::function_object_wrapper_factory`.

@copyright Barrett Adair 2015
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)

*/

#ifndef CLBL_FUNCTION_OBJECT_WRAPPER_FACTORY_H
#define CLBL_FUNCTION_OBJECT_WRAPPER_FACTORY_H

#include <utility>
#include <type_traits>
#include <CLBL/cv.h>
#include <CLBL/tags.h>
#include <CLBL/type_traits.h>
#include <CLBL/harden_cast.h>
#include <CLBL/qflags.h>
#include <CLBL/member_function_decay.h>
#include <CLBL/internal/member_function/slim_member_function_bound_to_object_wrapper.h>
#include <CLBL/internal/function_object/ambiguous_function_object_wrapper.h>
#include <CLBL/internal/function_object/casted_function_object_wrapper.h>

namespace clbl {

  struct function_object_wrapper_factory {

        using this_t = function_object_wrapper_factory;

        template<qualify_flags Flags, typename T>
        static inline constexpr auto 
        wrap(T&& t) {
            
            constexpr auto cv_qualifiers = cv<no_ref<T> > | Flags;
            using member_fn_type = decltype(&no_ref<T>::operator());
            using decayed_fn = member_function_decay<member_fn_type>;
            using wrapper = internal::slim_member_function_bound_to_object_wrapper<
                                this_t,
                                cv_qualifiers,
                                no_ref<T>,
                                member_fn_type,
                                &no_ref<T>::operator(),
                                decayed_fn>;
            return wrapper{ static_cast<T&&>(t) };
        }

        template<qualify_flags Flags, typename Invocation>
        static inline constexpr auto
        wrap_data(Invocation&& data) {
            return wrap<Flags>(data.object);
        }

        struct ambiguous {

            using this_t = ambiguous;

            template<qualify_flags Flags, typename FunctionObject>
            static inline constexpr auto
            wrap(FunctionObject&& f) {
                constexpr auto cv_qualifiers = cv<FunctionObject> | Flags;
                using wrapper = internal::ambiguous_function_object_wrapper<
                                    this_t,
                                    cv_qualifiers,
                                    no_ref<FunctionObject> >;
                return wrapper{ static_cast<FunctionObject&&>(f) };
            }

            template<qualify_flags Flags, typename Data>
            static inline constexpr auto
            wrap_data(Data&& data) {
                return wrap<Flags>(data);
            }
        };

        struct casted {

            using this_t = casted;

            template<qualify_flags Flags, typename Pmf, typename T>
            static inline constexpr auto
            wrap(T&& t) {
                constexpr auto cv_qualifiers = cv<no_ref<T> > | Flags;
                using decayed_fn = member_function_decay<Pmf>;
                using wrapper = internal::casted_function_object_wrapper<
                                    this_t,
                                    cv_qualifiers,
                                    no_ref<T>,
                                    Pmf,
                                    decayed_fn>;
                return wrapper{ static_cast<T&&>(t) };
            }

            template<qualify_flags Flags, typename Data>
            static inline constexpr auto
            wrap_data(Data&& data) {
                return wrap<
                        Flags,
                        decltype(no_ref<Data>::pmf)>(data.object);
            }
        };
    };
}

#endif