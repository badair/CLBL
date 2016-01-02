/*!
@file
Defines `clbl::qualify_flags`, the `` namespace, and the constants therein.

@copyright Barrett Adair 2015
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)

*/

#ifndef CLBL_QUALIFY_FLAGS_H
#define CLBL_QUALIFY_FLAGS_H

namespace clbl {
	
    //! qualify_flags are bitflags used to stack cv-qualifiers
    //! on top of each other.
    using qualify_flags = short;

	//! Flag representing no additional qualifiers on a type.
	constexpr qualify_flags default_ = 0;
	//! Flag representing a const qualifier on a type.
	constexpr qualify_flags const_ = 1;
	//! Flag representing a volatile qualifier on a type.
	constexpr qualify_flags volatile_ = 2;
	
	constexpr qualify_flags lvalue_reference_ = 4;
	constexpr qualify_flags rvalue_reference_ = 8;
}

#endif