/*!
@file
Defines all overloads of `clbl::fwrap`, as well as the CLBL_PMFWRAP macro.

@copyright Barrett Adair 2015
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)

*/

#ifndef CLBL_FWRAP_H
#define CLBL_FWRAP_H

#include <CLBL/core.hpp>

namespace clbl {

template<typename T>
using dispatch = disjunction<
    function_reference<T>,
    function_ptr<no_ref<T>>,
    function_object<generalized_object<no_ref<T>>>,
    //function_ptr<T*>
    pmf<no_ref<T>>
>;

template<typename T>
inline constexpr decltype(auto)
fwrap(T&& t) {
    using dispatch_type = typename dispatch<T&&>::dispatch_type;
    return internal::dispatcher<dispatch_type, T&&>{}(static_cast<T&&>(t));
}

template<typename T, typename... Args>
inline constexpr decltype(auto)
fwrap(T&& t, Args&&... args) {
    using dispatch_type = typename dispatch<T&&>::dispatch_type;
    return internal::dispatcher<dispatch_type, T&&>{}(
        static_cast<T&&>(t),
        static_cast<Args&&>(args)...
    );
}

}

#endif