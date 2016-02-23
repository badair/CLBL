/*!
@file
Defines `quali::ref_of`.

@copyright Barrett Adair 2015
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)

*/

#ifndef QUALI_QUALI_REF_OF_HPP
#define QUALI_QUALI_REF_OF_HPP

namespace quali {

    struct force_ref{};
    
    //todo move
    template<typename T, typename E = void>
    struct ref_of { 

        static constexpr const quali::flags value = 
            std::is_rvalue_reference<T>::value? quali::rvalue_reference_ 
                : (std::is_lvalue_reference<T>::value ? quali::lvalue_reference_ 
                    : (std::is_same<E, force_ref>::value? quali::lvalue_reference_
                            : quali::default_));
    };

}

#endif