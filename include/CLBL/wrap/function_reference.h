/*!
@file
Defines `clbl::function_reference`.

@copyright Barrett Adair 2015
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)

*/

#ifndef CLBL_FUNCTION_REFERENCE_H
#define CLBL_FUNCTION_REFERENCE_H

#include <type_traits>

#include <CLBL/no_ref.h>
#include <CLBL/wrappers/fn_ref_wrapper.h>

namespace clbl {

    struct function_reference {

        template<qualify_flags Flags, typename T>
        static inline constexpr auto
        wrap(T&& t) {
            using function_type = no_ref<T>;
            using wrapper = fn_ref_wrapper<function_reference, function_type>;
            return wrapper{ std::forward<T>(t) };
        }

        template<qualify_flags Flags, typename Invocation>
        static inline constexpr auto
            wrap_data(Invocation&& data) {
            return wrap<Flags>(data.ref);
        }
    };
}

#endif