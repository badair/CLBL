 /*!
@file

@copyright Barrett Adair 2015
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)

*/

#ifndef CLBL_CONSTRAINTS_H
#define CLBL_CONSTRAINTS_H

#include <type_traits>

// CLBL_REQUIRES_ and CLBL_REQUIRES adapted from Range-v3 here:
// https://github.com/ericniebler/range-v3/blob/6600e6054513202e61a067de48c4a05ca2b11099/include/range/v3/utility/concepts.hpp#L861
// Copyright Eric Niebler 2013-2014

#define CLBL_PP_CAT_(X, Y) X ## Y
#define CLBL_PP_CAT(X, Y)  CLBL_PP_CAT_(X, Y)

#define CLBL_REQUIRES_(...)                                                         \
    int CLBL_PP_CAT(clbl_requires_, __LINE__) = 42,                                 \
    typename std::enable_if<                                                        \
        (CLBL_PP_CAT(clbl_requires_, __LINE__) == 43) || (__VA_ARGS__),             \
        int                                                                         \
    >::type = 0                                                                     \
/**/

#define CLBL_REQUIRES(...)                                                          \
    template<                                                                       \
        int CLBL_PP_CAT(clbl_requires_, __LINE__) = 42,                             \
        typename std::enable_if<                                                    \
            (CLBL_PP_CAT(clbl_requires_, __LINE__) == 43) || (__VA_ARGS__),         \
            int                                                                     \
        >::type = 0>                                                                \
/**/

namespace clbl {

template<
    typename Target,
    typename Ptr, 
    CLBL_REQUIRES_(std::is_convertible<decltype(*std::declval<Ptr>()), Target>::value)
>
using ConvertiblePointer = Ptr;

}

#endif