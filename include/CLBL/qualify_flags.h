#ifndef CLBL_QUALIFY_FLAGS_H
#define CLBL_QUALIFY_FLAGS_H

namespace clbl {

    using qualify_flags = short;

    namespace qflags {
        constexpr qualify_flags default_ = 0;
        constexpr qualify_flags const_ = 1;
        constexpr qualify_flags volatile_ = 2;
        constexpr qualify_flags lvalue_reference_ = 4;
        constexpr qualify_flags rvalue_reference_ = 8;
    }
}

#endif