/*!
@file
Defines `clbl::qualify_flags`, the `` namespace, and the constants therein.

@copyright Barrett Adair 2015
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)

*/

#ifndef CLBL_QUALIFY_FLAGS_H
#define CLBL_QUALIFY_FLAGS_H

#include <cstdint>

#define CLBL_ASSERT_VALID_REFERENCE_FLAGS(my_flags) \
    static_assert(my_flags <= clbl::qflags::max_qualify_flags_value, \
        "Error: qualify_flags cannot contain both lvalue_reference_ and " \
        "rvalue_reference_. To resolve this issue, you may use clbl::qfl" \
        "ags::collapse_reference, which mimics the reference collapsing " \
        "rules that were added in C++11. (example: auto my_flags = clbl:" \
        ":qflags::collapse_reference<existing_flags, clbl::qflags::[l/r]" \
        "value_reference_>;). You may also use clbl::qflags::add_[l/r]va" \
        "lue_reference in a similar manner.")

namespace clbl {
	
    //! qualify_flags are bit flags used to signify cv-qualifiers
    //! and ref-qualifiers/reference types
    using qualify_flags = std::uint8_t;

    namespace qflags {

        /*
        This grid is useful for debugging type errors where
        the error messages often include a `qualify_flags` value:

         value | RR LR V  C | result
        --------------------------------------------
           0   | 0  0  0  0 | default
           1   | 0  0  0  1 | const
           2   | 0  0  1  0 | volatile
           3   | 0  0  1  1 | const volatile
        --------------------------------------------
           4   | 0  1  0  0 | lref
           5   | 0  1  0  1 | const lref
           6   | 0  1  1  0 | volatile lref
           7   | 0  1  1  1 | const volatile lref
        --------------------------------------------
           8   | 1  0  0  0 | rref 
           9   | 1  0  0  1 | const rref
          10   | 1  0  1  0 | volatile rref
          11   | 1  0  1  1 | const volatile rref
        -------------------------------------------- 
          12+  | 1  1  *  * | invalid - use 
                               `clbl::qflags::collapse_reference`
        */

        constexpr qualify_flags max_qualify_flags_value = 11;
        
        //! Flag representing the default qualifiers on a type 
        //! or member function overload.
        constexpr qualify_flags default_           = 0;

        //! Flag representing a const qualifier on a type or
        //! member function overload.
        constexpr qualify_flags const_             = 1;

        //! Flag representing a volatile qualifier on a type
        //! or member function overload.
        constexpr qualify_flags volatile_          = 2;
        
        //! Flag representing an lvalue reference type, or
        //! an lvalue-reference-qualified member function
        //! overload.
        constexpr qualify_flags lvalue_reference_  = 4;

        //! shorthand for `clbl::qflags::lvalue_reference_`.
        constexpr qualify_flags ref_               = 4;

        //! Flag representing an lvalue reference type, or
        //! an rvalue-reference-qualified member function
        //! overload.
        constexpr qualify_flags rvalue_reference_  = 8;

        //! shorthand for `clbl::qflags::rvalue_reference_`.
        constexpr qualify_flags move_              = 8;

        template<qualify_flags Flags>
        constexpr auto add_const = Flags | const_;

        template<qualify_flags Flags>
        constexpr auto remove_const = Flags & ~const_;

        template<qualify_flags Flags>
        constexpr auto add_volatile = Flags | volatile_;

        template<qualify_flags Flags>
        constexpr auto remove_volatile = Flags & ~volatile_;

        template<qualify_flags Flags>
        constexpr auto add_cv = Flags | volatile_ | const_;

        template<qualify_flags Flags>
        constexpr auto remove_cv = Flags & ~volatile_ & ~const_;

        template<qualify_flags Flags>
        constexpr auto remove_reference = 
            (Flags & ~rvalue_reference_) & ~lvalue_reference_;

        template<qualify_flags Flags>
        constexpr auto force_lvalue_reference =
            (Flags & ~rvalue_reference_) | lvalue_reference_;

        template<qualify_flags Flags>
        constexpr auto force_rvalue_reference =
            (Flags & ~lvalue_reference_) | rvalue_reference_;

        namespace detail {
            template<qualify_flags Flags, qualify_flags Other>
            struct collapse_reference_t {
                static constexpr auto value = Flags | Other;
            };

            template<qualify_flags Flags>
            struct collapse_reference_t<Flags, lvalue_reference_> {
                static constexpr auto value = 
                    remove_reference<Flags> | lvalue_reference_;
            };

            template<qualify_flags Flags>
            struct collapse_reference_t<Flags, rvalue_reference_> {
                static constexpr auto value = 
                    (Flags & lvalue_reference_) == 0?
                         (Flags | rvalue_reference_)
                        :(remove_reference<Flags> | lvalue_reference_);
            };
        }

        template<qualify_flags existing, qualify_flags reference>
        constexpr auto collapse_reference =
            detail::collapse_reference_t<existing, reference>::value;

        template<qualify_flags Flags>
        constexpr auto add_lvalue_reference 
            = collapse_reference<Flags, lvalue_reference_>;

        template<qualify_flags Flags>
        constexpr auto add_rvalue_reference 
            = collapse_reference<Flags, rvalue_reference_>;

        template<qualify_flags Flags>
        constexpr auto guarantee_reference = 
            ((Flags & rvalue_reference_) == 0 
            && (Flags & lvalue_reference_) == 0) ? (Flags | lvalue_reference_) : Flags;
    }
}

#endif