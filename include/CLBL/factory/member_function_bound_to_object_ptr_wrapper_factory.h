/*!
@file
Defines `clbl::member_function_bound_to_object_ptr_wrapper_factory`.

@copyright Barrett Adair 2015
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)

*/

#ifndef CLBL_MEMBER_FUNCTION_BOUND_TO_OBJECT_PTR_FACTORY_H
#define CLBL_MEMBER_FUNCTION_BOUND_TO_OBJECT_PTR_FACTORY_H

#include <utility>
#include <CLBL/tags.h>
#include <CLBL/type_traits.h>
#include <CLBL/cv_of.h>
#include <CLBL/qflags.h>
#include <CLBL/internal/member_function/member_function_bound_to_object_ptr_wrapper.h>
#include <CLBL/internal/member_function/slim_member_function_bound_to_object_ptr_wrapper.h>

namespace clbl {

    struct member_function_bound_to_object_ptr_wrapper_factory {

        using this_t = member_function_bound_to_object_ptr_wrapper_factory;

        template<qualify_flags Flags = qflags::default_, typename T, typename TMemberFnPtr>
        static inline constexpr auto
        wrap(TMemberFnPtr member_fn_ptr, T&& t) {
            using object_type = no_ref<decltype(*std::declval<T>())>;
            using ptr_type = no_ref<T>;
            using wrapper = internal::member_function_bound_to_object_ptr_wrapper<
                                    this_t,
                                    cv_of<T>::value | Flags,
                                    object_type,
                                    ptr_type,
                                    TMemberFnPtr>;
            return wrapper{{ member_fn_ptr, static_cast<T&&>(t) }};
        }

        template<qualify_flags Flags = qflags::default_, typename Invocation>
        static inline constexpr auto
        wrap_data(Invocation&& data) {
            return wrap<Flags>(data.pmf, data.object_ptr);
        }

        struct slim {

            using this_t = slim;

            template<qualify_flags Flags = qflags::default_,
                    typename TMemberFnPtr,
                    TMemberFnPtr Pmf,
                    typename T>
            static inline constexpr auto
            wrap(T&& t) {
                using object_type = no_ref<decltype(*std::declval<T>())>;
                using ptr_type = no_ref<T>;
                using wrapper = internal::slim_member_function_bound_to_object_ptr_wrapper<
                                                this_t, 
                                                cv_of<T>::value | Flags,
                                                object_type,
                                                ptr_type,
                                                TMemberFnPtr,
                                                Pmf>;
                return wrapper{ static_cast<T&&>(t) };
            }

            template<qualify_flags Flags = qflags::default_,
                    typename Invocation,
                    typename TMemberFnPtr = decltype(Invocation::pmf),
                    TMemberFnPtr Pmf = Invocation::pmf>
            static inline constexpr auto
                wrap_data(Invocation&& data) {
                return wrap<Flags, TMemberFnPtr, Pmf>(data.object_ptr);
            }
        };
    };
}

#endif