/*

Copyright Barrett Adair 2015
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)

*/

#ifndef CLBL_NOREF_H
#define CLBL_NOREF_H

#include <type_traits>

namespace clbl {
	
    /*
    clbl::no_ref lets us use std::remove_reference_t
    with less screen clutter, because we use it EVERYWHERE
    */
    
    template<typename T>
    using no_ref = std::remove_reference_t<T>;
}

#endif