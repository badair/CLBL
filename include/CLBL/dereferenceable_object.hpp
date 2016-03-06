/*!
@file

@copyright Barrett Adair 2015
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)

*/

#ifndef CLBL_DEREFERENCEABLE_OBJECT_HPP
#define CLBL_DEREFERENCEABLE_OBJECT_HPP

namespace clbl {
    
    template<typename T>
    using default_dereferenceable = typename std::conditional<
        can_dereference<T>::value,
        T,
        int*
    >::type;

    template<
        typename Ptr, 
        typename std::enable_if<
            can_dereference<Ptr>::value 
            && std::is_class<
                typename std::remove_reference<
                    decltype(*std::declval<default_dereferenceable<Ptr>>())
                >::type
            >::value,
            int*
        >::type = nullptr
    >
    using dereferenceable_object = Ptr;
}

#endif