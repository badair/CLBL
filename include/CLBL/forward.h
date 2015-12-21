#ifndef CLBL_FORWARD_H
#define CLBL_FORWARD_H

#include <type_traits>

#include <CLBL/forwardable.h>
#include <CLBL/utility.h>

namespace clbl {

    /*
    clbl::forward is an implicitly convertible vessel for pefectly forwarded parameters. This allows
    us to create a "perfect forwarding" std::function object, that still forwards to the original
    function/object, even in the case of nested CLBL callable types
    */

    template<typename FwdType>
    struct forward {
        using T = std::conditional_t <
            std::is_lvalue_reference<FwdType>::value || std::is_rvalue_reference<FwdType>::value,
            forwardable<FwdType>,
            std::add_lvalue_reference_t<forwardable<FwdType> >
        >;

        T value;

        template<typename U = FwdType, std::enable_if_t<std::is_rvalue_reference<U>::value, dummy>* = nullptr>
        inline forward(forwardable<FwdType> t) : value(static_cast<FwdType&&>(t)) {}

        template<typename U = FwdType, std::enable_if_t<!std::is_rvalue_reference<U>::value, dummy>* = nullptr>
        inline forward(forwardable<FwdType> t) : value(t) {}

        inline forward() = default;
        inline forward(forward<FwdType>&) = default;
        inline forward(const forward<FwdType>&) = default;
        inline forward(forward<FwdType>&&) = default;
        inline forward(volatile forward<FwdType>& other) : value(other.value) {}
        inline forward(const volatile forward<FwdType>& other) : value(other.value) {}

        inline operator std::conditional_t<std::is_rvalue_reference<T>::value, no_ref<T>, T>() const { return value; }
        inline operator std::conditional_t<std::is_rvalue_reference<T>::value, no_ref<T>, T>() const volatile { return value; }
    };
}

#endif