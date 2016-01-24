/*!
@file
Defines `clbl::member_function_wrapper_factory`.

@copyright Barrett Adair 2015
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)

*/

#ifndef CLBL_MEMBER_FUNCTION_WRAPPER_FACTORY_H
#define CLBL_MEMBER_FUNCTION_WRAPPER_FACTORY_H

#include <utility>

#include <CLBL/tags.h>
#include <CLBL/type_traits.h>
#include <CLBL/cv.h>
#include <CLBL/qflags.h>
#include <CLBL/internal/member_function/member_function_wrapper.h>

namespace clbl {

    struct member_function_wrapper_factory {

        using this_t = member_function_wrapper_factory;

        template<qualify_flags Flags, typename Pmf>
        static inline constexpr auto
        wrap(Pmf&& ptr){
            return member_function_wrapper<
                this_t, Flags, Pmf>(static_cast<Pmf&&>(ptr));
        }

        template<qualify_flags Flags, typename Invocation>
        static inline constexpr auto
            wrap_data(Invocation&& data) {
            return wrap<Flags, no_ref<Invocation> >(data);
        }
    };
}

#endif