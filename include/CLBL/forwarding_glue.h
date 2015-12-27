/*!
@file
Defines `clbl::forwarding_glue`.

@copyright Barrett Adair 2015
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)

*/

#ifndef CLBL_FORWARDING_GLUE_H
#define CLBL_FORWARDING_GLUE_H

#include <CLBL/no_ref.h>

namespace clbl {
    template<typename Callable>
    using forwarding_glue = typename no_ref<Callable>::forwarding_glue;
}

#endif