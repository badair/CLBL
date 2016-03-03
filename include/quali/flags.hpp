/*!
@file
Defines `quali::flags`

@copyright Barrett Adair 2015
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)

*/

#ifndef QUALI_FLAGS_HPP
#define QUALI_FLAGS_HPP

#define QUALI_ASSERT_VALID_REFERENCE_FLAGS(my_flags) \
    static_assert(my_flags <= quali::max_flags_value, \
        "quali::flags should not contain both lvalue_reference_ and " \
        "rvalue_reference_. To resolve this issue, you may use quali::" \
        "collapse_reference, which mimics the reference collapsing " \
        "rules that were added in C++11. (example: auto my_flags = " \
        "quali::collapse_reference<existing_flags, quali::[l/r]" \
        "value_reference_>;). You may also use quali::add_[l/r]va" \
        "lue_reference in a similar manner.")

namespace quali {
	
    //! flags are bit flags used to signify cv-qualifiers
    //! and ref-qualifiers/reference types
    using flags = std::uint32_t;

    /*
    This grid is useful for debugging type errors where
    the error messages often include a `flags` value:

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
                           `quali::collapse_reference`
    */

    constexpr flags max_flags_value    = 11;
    
    //! Flag representing the default qualifiers on a type 
    //! or member function overload.
    constexpr flags default_           = 0;

    //! Flag representing a const qualifier on a type or
    //! member function overload.
    constexpr flags const_             = 1;

    //! Flag representing a volatile qualifier on a type
    //! or member function overload.
    constexpr flags volatile_          = 2;
    
    //! Flag representing an lvalue reference type, or
    //! an lvalue-reference-qualified member function
    //! overload.
    constexpr flags lvalue_reference_  = 4;

    //! shorthand for `quali::lvalue_reference_`.
    constexpr flags lref_              = 4;

    //! shorthand for `quali::const_ | quali::lvalue_reference_`.
    constexpr flags clref_             = 5;

    //! Flag representing an lvalue reference type, or
    //! an rvalue-reference-qualified member function
    //! overload.
    constexpr flags rvalue_reference_  = 8;

    //! shorthand for `quali::rvalue_reference_`.
    constexpr flags rref_              = 8;

    template<flags Flags>
    struct add_const {
        static constexpr const auto value = Flags | const_;
    };

    template<flags Flags>
    struct remove_const {
        static constexpr const auto value = Flags & ~const_;
    };

    template<flags Flags>
    struct add_volatile {
        static constexpr const auto value = Flags | volatile_;
    };

    template<flags Flags>
    struct remove_volatile {
        static constexpr const auto value = Flags & ~volatile_;
    };

    template<flags Flags>
    struct add_cv {
        static constexpr const auto value =
            Flags | volatile_ | const_;
    };

    template<flags Flags>
    struct remove_cv {
        static constexpr const auto value =
            Flags & ~volatile_ & ~const_;
    };

    template<flags Flags>
    struct remove_reference {
        static constexpr const auto value =
            (Flags & ~rvalue_reference_) & ~lvalue_reference_;
    };

    template<flags Flags>
    struct force_lvalue_reference {
        static constexpr const auto value =
            (Flags & ~rvalue_reference_) | lvalue_reference_;
    };

    template<flags Flags>
    struct force_rvalue_reference {
        static constexpr const auto value =
            (Flags & ~lvalue_reference_) | rvalue_reference_;
    };

    namespace detail {
        template<flags Flags, flags Other>
        struct collapse_reference_t {
            static constexpr const auto value = Flags | Other;
        };

        template<flags Flags>
        struct collapse_reference_t<Flags, lvalue_reference_> {
            static constexpr const auto value = 
                remove_reference<Flags>::value | lvalue_reference_;
        };

        template<flags Flags>
        struct collapse_reference_t<Flags, rvalue_reference_> {
            static constexpr const auto value = 
                (Flags & lvalue_reference_) == 0?
                     (Flags | rvalue_reference_)
                    :(remove_reference<Flags>::value | lvalue_reference_);
        };
    }

    template<flags existing, flags reference>
    struct collapse_reference {
        static constexpr const auto value =
            detail::collapse_reference_t<existing, reference>::value;
    };

    template<flags Flags>
    struct add_lvalue_reference {
        static constexpr const auto value =
            collapse_reference<Flags, lvalue_reference_>::value;
    };

    template<flags Flags>
    struct add_rvalue_reference {
        static constexpr const auto value =
            collapse_reference<Flags, rvalue_reference_>::value;
    };

    template<flags Flags>
    struct guarantee_reference {
        static constexpr const auto value = 
        (
            (Flags & rvalue_reference_) == 0 
            && (Flags & lvalue_reference_) == 0
        ) ? 
        (Flags | lvalue_reference_) : Flags;
    };
}

#endif