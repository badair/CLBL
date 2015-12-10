#ifndef CLBL_MEMBER_FUNCTION_H
#define CLBL_MEMBER_FUNCTION_H

#include <type_traits>

#include "CLBL/cv_checks.h"
#include "CLBL/callable.h"

namespace clbl {

    //dispatch failure case for member functions
    template<typename, typename, typename Failure>
    struct member_function { static_assert(sizeof(Failure) < 0, "Not a member function."); };

    template<typename T, typename MemberFunctionPointerType, typename Return, typename... Args>
    struct member_function<
        T,
        MemberFunctionPointerType,
        Return(std::remove_cv_t<T>::*)(Args...)
    >
        : callable<member_function<T, MemberFunctionPointerType, Return(std::remove_cv_t<T>::*)(Args...)>, Return(Args...)> {

        using my_type = member_function<T, MemberFunctionPointerType, Return(std::remove_cv_t<T>::*)(Args...)>;

        inline member_function(T&& o, MemberFunctionPointerType f_ptr)
            : object(o), value(f_ptr)
        {}

        inline member_function(T& o, MemberFunctionPointerType f_ptr)
            : object(o), value(f_ptr)
        {}

        inline member_function(my_type& other) = default;
        inline member_function(const my_type& other) = default;
        inline member_function(my_type&& other) = default;

        inline member_function(volatile my_type& other)
            : object(other.object), value(other.value)
        {}

        inline member_function(const volatile my_type& other)
            : object(other.object), value(other.value)
        {}

        template<typename... Fargs>
        inline Return operator()(Fargs&&... a) {
            return (object.*value)(std::forward<Fargs>(a)...);
        }

        template<typename... Fargs>
        inline Return operator()(Fargs&&... a) const {
            return (object.*value)(std::forward<Fargs>(a)...);
        }

        template<typename... Fargs>
        inline Return operator()(Fargs&&... a) volatile {
            return (object.*value)(std::forward<Fargs>(a)...);
        }

        template<typename... Fargs>
        inline Return operator()(Fargs&&... a) const volatile {
            return (object.*value)(std::forward<Fargs>(a)...);
        }

        static constexpr bool clbl_is_deep_const = is_deep_const<T>() 
                                                    || std::is_const<T>::value;

        static constexpr bool clbl_is_deep_volatile = is_deep_volatile<T>()
                                                    || std::is_volatile<T>::value;

        T object;
        MemberFunctionPointerType value;
    };


}

#endif