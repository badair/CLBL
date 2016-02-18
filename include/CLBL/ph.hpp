/*!
Copyright (c) 2002 Peter Dimov and Multi Media Ltd.
Copyright (c) 2016 Barrett Adair

Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
*/

#ifndef CLBL_PH_H
#define CLBL_PH_H

namespace clbl {

    template<int I> struct ph;

}

namespace std {

    template<int I> 
    struct is_placeholder< clbl::ph<I> >
    {
        static constexpr const int value = I;
    };
}

namespace clbl { 

template<int I>
    struct ph {

        ph() = default;

        template<typename T>
        ph(T const &) {
            static_assert(I == std::is_placeholder<T>::value, "Invalid placeholder");
        }
    };
}

#endif