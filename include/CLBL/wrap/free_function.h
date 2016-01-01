/*!
@file
Defines `clbl::free_function`.

@copyright Barrett Adair 2015
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)

*/

#ifndef CLBL_FREE_FUNCTION_H
#define CLBL_FREE_FUNCTION_H

#include <type_traits>

#include <CLBL/no_ref.h>
#include <CLBL/wrappers/free_fn_wrapper.h>

namespace clbl {

    struct free_function {

        template<qualify_flags Flags, typename T>
        static inline constexpr auto
        wrap(T&& t) {
            using function_type = std::remove_pointer_t<no_ref<T> >;
            using wrapper = free_fn_wrapper<free_function, function_type>;
            return wrapper{ std::forward<T>(t) };
        }

        template<qualify_flags Flags, typename Invocation>
        static inline constexpr auto
            wrap_data(Invocation&& data) {
            return wrap<Flags>(data.ptr);
        }
    };

}

#endif