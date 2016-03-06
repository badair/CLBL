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
        template<std::size_t N, typename T = int>
        struct lexical_flag_map {
            static_assert(sizeof(T) < 0,
                "Invalid argument passed to QUALI macro. "\
                "Valid arguments: 'const', 'volatile', '&', '&&' ");
        };

        //mapping flags by length of string - a most convenient hack
        template<> struct lexical_flag_map<1, int> { static constexpr auto value = default_; };
        template<> struct lexical_flag_map<2, int> { static constexpr auto value = lref_;    };
        template<> struct lexical_flag_map<3, int> { static constexpr auto value = rref_;    };
        template<> struct lexical_flag_map<6, int> { static constexpr auto value = const_;   };
        template<> struct lexical_flag_map<9, int> { static constexpr auto value = volatile_;};
    }
}

#endif