/*!
@file
Defines `quali::flags`

@copyright Barrett Adair 2015
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)

*/

#ifndef QUALI_FLAGS_HPP
#define QUALI_FLAGS_HPP

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
      12+  | 1  1  *  * | invalid - use `quali::collapse`
                           
    */

    inline namespace qualifier_flags {

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
    }

    template<flags Flags>
    struct validate_flags {

        static_assert(Flags <= quali::max_flags_value, \
        "A quali::flags value may not contain both lvalue_reference_ "\
        "and rvalue_reference_. To resolve this issue, you can use "\
        "quali::collapse, which enforces the reference collapsing "\
        "rules that were added in C++11. (example: quali::collapse"\
        "<existing_flags, new_flags>)");

        using type = std::integral_constant<flags, Flags>;
    };

    #ifdef QUALI_CHECK_FLAGS
    #define QUALI_FLAG_CONSTANT(...) typename validate_flags<__VA_ARGS__>::type
    #else
    #define QUALI_FLAG_CONSTANT(...) std::integral_constant<flags, __VA_ARGS__>
    #endif //QUALI_CHECK_FLAGS


    template<flags Flags>
    using add_const = QUALI_FLAG_CONSTANT(Flags | 1 /*const*/);

    template<flags Flags>
    using remove_const = QUALI_FLAG_CONSTANT(Flags & ~1 /*const*/);

    template<flags Flags>
    using is_const = std::integral_constant<bool,
        (Flags & 1 /*const*/) != 0
    >;

    template<flags Flags>
    using add_volatile = QUALI_FLAG_CONSTANT(Flags | 2 /*volatile*/);

    template<flags Flags>
    using remove_volatile = QUALI_FLAG_CONSTANT(Flags & ~2 /*volatile*/);

    template<flags Flags>
    using is_volatile = std::integral_constant<bool,
        (Flags & 2 /*volatile*/) != 0
    >;

    template<flags Flags>
    using add_cv = QUALI_FLAG_CONSTANT(Flags | 2 /*volatile*/ | 1 /*const*/);

    template<flags Flags>
    using remove_cv = QUALI_FLAG_CONSTANT(Flags & ~2 /*volatile*/ & ~1 /*const*/);

    template<flags Flags>
    using remove_reference = QUALI_FLAG_CONSTANT(
        (Flags & ~8 /*&&*/) & ~4 /*&*/
    );

    template<flags Flags>
    using is_lvalue_reference = std::integral_constant<bool,
        (Flags & 4 /*&*/) != 0
    >;

    //is_rvalue_reference uses reference collapsing rules
    template<flags Flags>
    using is_rvalue_reference = std::integral_constant<bool,
        (Flags & (4 /*&*/ | 8 /*&&*/)) == 8 /*&&*/
    >;

    template<flags Flags>
    using force_lvalue_reference = QUALI_FLAG_CONSTANT(
        (Flags & ~8 /*&&*/) | 4 /*&*/
    );

    template<flags Flags>
    using force_rvalue_reference = QUALI_FLAG_CONSTANT(
        (Flags & ~4 /*&*/) | 8 /*&&*/
    );

    template<flags Existing, flags Other>
    using collapse = QUALI_FLAG_CONSTANT(
        (Existing & 4 /*&*/) == 0 ?
            (Existing | Other)
            : (Existing | (Other & ~8 /*&&*/))
    );

    template<flags Flags>
    using add_lvalue_reference = QUALI_FLAG_CONSTANT(
        4 /*&*/ | (Flags & ~8 /*&&*/)
    );

    template<flags Flags>
    using add_rvalue_reference = collapse<Flags, 8 /*&&*/>;

    template<flags Flags>
    using guarantee_reference = QUALI_FLAG_CONSTANT(
        (
            (Flags & 8 /*&&*/) == 0 
            && (Flags & 4 /*&*/) == 0
        ) ? 
        (Flags | 4 /*&*/) : Flags
    );

    template<typename U, typename T = typename std::remove_reference<U>::type>
    using cv_of = std::integral_constant<quali::flags,
        (std::is_const<T>::value ? 1 /*const*/ : 0 /*default*/)
         | (std::is_volatile<T>::value ? 2 /*volatile*/ : 0 /*default*/)
     >;

     using force_ref = std::true_type;
    
    //todo move
    template<typename T, typename ForceRef = std::false_type>
    using ref_of = std::integral_constant<flags,
            std::is_rvalue_reference<T>::value? 8 /*&&*/ 
                : (std::is_lvalue_reference<T>::value ? 4 /*&*/ 
                    : (ForceRef::value? 4 /*&*/ : 0 /*default*/))
    >;
}

#endif