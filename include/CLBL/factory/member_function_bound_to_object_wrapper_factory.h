/*!
@file
Defines `clbl::internal::slim_object_bound_member_function_wrapper`.

@copyright Barrett Adair 2015
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)

*/

#include <tuple>
#include <utility>
#include <type_traits>
#include <CLBL/cv_of.h>
#include <CLBL/tags.h>
#include <CLBL/type_traits.h>
#include <CLBL/forward.h>
#include <CLBL/harden_cast.h>
#include <CLBL/qflags.h>
#include <CLBL/internal/bind/binding_wrapper.h>
#include <CLBL/internal/member_function/member_function_bound_to_object_wrapper.h>
#include <CLBL/internal/member_function/slim_member_function_bound_to_object_wrapper.h>

#ifndef CLBL_MEMBER_FUNCTION_BOUND_TO_OBJECT_WRAPPER_FACTORY_H
#define CLBL_MEMBER_FUNCTION_BOUND_TO_OBJECT_WRAPPER_FACTORY_H

namespace clbl {

struct member_function_bound_to_object_wrapper_factory {

    using this_t = member_function_bound_to_object_wrapper_factory;

    template<qualify_flags Flags, typename T, typename Pmf>
    static inline constexpr auto 
    wrap(Pmf member_fn_ptr, T&& t) {
        using wrapper = internal::member_function_bound_to_object_wrapper<
                            this_t, 
                            cv_of<T>::value | Flags,
                            no_ref<T>,
                            no_ref<Pmf> >;
        return wrapper{{ member_fn_ptr, static_cast<T&&>(t) }};
    }

    template<qualify_flags Flags, typename Data>
    static inline constexpr auto
    wrap_data(Data&& data) {
        return wrap<Flags>(data.pmf, data.object);
    }

    //"slim" means it takes the PMF as a template argument
    struct slim {

        using this_t = slim;

        template<
            qualify_flags Flags = qflags::default_,
            typename Pmf,
            Pmf Value,
            typename T>
        static inline constexpr auto
        wrap(T&& t) {
            using wrapper = internal::slim_member_function_bound_to_object_wrapper<
                                this_t, 
                                cv_of<no_ref<T> >::value | Flags,
                                no_ref<T>,
                                Pmf,
                                Value>;
            return wrapper{{ static_cast<T&&>(t) }};
        }

        template<qualify_flags Flags, typename Data>
        static inline constexpr auto
        wrap_data(Data&& data) {
            return wrap<
                     Flags, 
                     decltype(no_ref<Data>::pmf),
                     no_ref<Data>::pmf
                   >(data.object);
        }
    };
};

}

#endif