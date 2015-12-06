#ifndef CLBL_TAGS_H
#define CLBL_TAGS_H

#include <type_traits>

#define CLBL_NOTHING_ARGS(x)
#define CLBL_NOTHING CLBL_NOTHING_ARGS(x)

namespace clbl {

    struct ambiguous_return {};
    struct ambiguous_args {};
}

#endif