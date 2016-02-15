/*!
@file
Defines `clbl::forward`.

@copyright Barrett Adair 2015
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)

*/

#ifndef CLBL_FORWARD_H
#define CLBL_FORWARD_H

#include <type_traits>
#include <utility>

#include <CLBL/tags.h>
#include <CLBL/constraints.h>
#include <CLBL/type_traits.h>
#include <CLBL/forwardable.h>

namespace clbl {

    /*!
    clbl::forward is an implicitly convertible vessel for pefectly forwarded parameters.
    This allows us to, ultimately, create a "perfect forwarding" std::function object,
    that still forwards to the original function/object.
    */

    template<typename FwdType>
    struct forward {

        std::conditional_t<std::is_rvalue_reference<FwdType>::value, 
            const no_ref<FwdType>&, 
            forwardable<FwdType> 
        > value;

        inline forward() = default;

        inline forward(forward<FwdType>&) = default;

        inline forward(const forward<FwdType>&) = default;

        inline forward(forward<FwdType>&&) = default;

        inline forward(volatile forward<FwdType>& other)
            : value(other.value) {}
            
        inline forward(const volatile forward<FwdType>& other)
            : value(other.value) {}

        //construction from rvalue
        template<
            typename U = FwdType, 
            CLBL_REQUIRES_(std::is_rvalue_reference<U>::value)
        >
        inline forward(U t) : value(std::move(t)) {}

        //construction from lvalue
        template<
            typename U = FwdType,
            CLBL_REQUIRES_(std::is_rvalue_reference<U>::value)
        >
        inline forward(const no_ref<U>& t) : value(t) {}

        //construction from lvalue
        template<
            typename U = FwdType,
            CLBL_REQUIRES_(!std::is_rvalue_reference<U>::value)
        >
        inline forward(forwardable<FwdType> t) : value(t) {}

        //implicit conversion to xvalue
        template<
            typename U = FwdType,
            CLBL_REQUIRES_(std::is_rvalue_reference<U>::value)
        >
        inline operator U() const { 
            return std::move(value);
        }

        //implicit conversion to xvalue
        template<
            typename U = FwdType,
            CLBL_REQUIRES_(std::is_rvalue_reference<U>::value)
        >
        inline operator U() const volatile {
            return std::move(value);
        }

        //implicit conversion to prvalue
        template<
            typename U = FwdType,
            CLBL_REQUIRES_(std::is_rvalue_reference<U>::value)
        >
        inline operator no_ref<U>() const {
            return value;
        }

        //implicit conversion to prvalue
        template<
            typename U = FwdType,
            CLBL_REQUIRES_(std::is_rvalue_reference<U>::value)
        >
        inline operator no_ref<U>() const volatile {
            return value;
        }

        //implicit conversion to lvalue reference
        template<
            typename U = FwdType,
            CLBL_REQUIRES_(!std::is_rvalue_reference<U>::value)
        >
        inline operator forwardable<FwdType>() const {
            return value; 
        }

        //implicit conversion to lvalue reference
        template<
            typename U = FwdType,
            CLBL_REQUIRES_(!std::is_rvalue_reference<U>::value)
        >
        inline operator forwardable<FwdType>() const volatile {
            return value;
        }
    };

    template<>
    struct forward<void> {};
}

#endif