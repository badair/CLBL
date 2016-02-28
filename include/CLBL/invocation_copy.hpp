/*!
@file
Defines `clbl::invocation_copy`.

@copyright Barrett Adair 2015
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)

*/

#ifndef CLBL_INVOCATION_COPY_H
#define CLBL_INVOCATION_COPY_H

#include <type_traits>

namespace clbl {

    /*
    clbl::invocation_copy get's the return type of a CLBL wrapper's
    copy_wrapper function
    */
    template<typename Callable>
    using invocation_copy = decltype(Callable::copy_wrapper(std::declval<std::add_lvalue_reference_t<Callable> >()));
}

#endif