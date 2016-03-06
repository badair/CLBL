/*!
@file

@copyright Barrett Adair 2015
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)

*/

#ifndef QUALI_LEXICAL_FLAG_MAP_HPP
#define QUALI_LEXICAL_FLAG_MAP_HPP

namespace quali {

    namespace pp_detail {

        template<typename T>
        struct lexical_flag_map {
            static_assert(sizeof(T) < 0,
                "Invalid argument passed to QUALI_FLAGS macro. use one or more of "\
                "the following, separated by commas: const, volatile, &, && ");

            static constexpr quali::flags value = default_; 
        };

        template<> struct lexical_flag_map<int> { static constexpr quali::flags value = default_; };
        template<> struct lexical_flag_map<int &> { static constexpr quali::flags value = lref_; };
        template<> struct lexical_flag_map<int &&> { static constexpr quali::flags value = rref_; };
        template<> struct lexical_flag_map<int const> { static constexpr quali::flags value = const_; };
        template<> struct lexical_flag_map<int volatile> { static constexpr quali::flags value = volatile_; };

        //lexical_intc_map is logically identical to lexical_flag_map. Only the error message differs.
        template<typename T>
        struct lexical_intc_map {
            static_assert(sizeof(T) < 0,
                "Invalid argument passed to QUALI_INTC macro. use one or more of "\
                "the following, separated by commas: const, volatile, &, && ");

            static constexpr quali::flags value = default_;
        };

        template<> struct lexical_intc_map<int> { static constexpr quali::flags value = default_; };
        template<> struct lexical_intc_map<int &> { static constexpr quali::flags value = lref_; };
        template<> struct lexical_intc_map<int &&> { static constexpr quali::flags value = rref_; };
        template<> struct lexical_intc_map<int const> { static constexpr quali::flags value = const_; };
        template<> struct lexical_intc_map<int volatile> { static constexpr quali::flags value = volatile_; };
    }
}

#endif