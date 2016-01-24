/*!
@file
Defines `clbl::universal_member_function_wrapper_factory`.

@copyright Barrett Adair 2015
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)

*/

#ifndef CLBL_UNIVERSAL_MEMBER_FUNCTION_WRAPPER_FACTORY_H
#define CLBL_UNIVERSAL_MEMBER_FUNCTION_WRAPPER_FACTORY_H

#include <utility>

#include <CLBL/tags.h>
#include <CLBL/type_traits.h>
#include <CLBL/cv.h>
#include <CLBL/qflags.h>
#include <CLBL/factory/member_function_wrapper_factory.h>
#include <CLBL/internal/member_function/universal_member_function_wrapper.h>

namespace clbl {

    //todo slim
    struct universal_member_function_wrapper_factory {

        template<typename Pmf>
        static inline constexpr auto
        wrap(Pmf&& ptr){
            //using member_function_wrapper_factory as the creator, because 
            //member_function_wrapper_factory takes CV flags for hardening
            return universal_member_function_wrapper<
                member_function_wrapper_factory, Pmf>(static_cast<Pmf&&>(ptr));
        }

        template<typename Invocation>
        static inline constexpr auto
            wrap_data(Invocation&& data) {
            return wrap<no_ref<Invocation> >(data);
        }
    };
}

#endif