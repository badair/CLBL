#ifndef CLBL_MEMBER_FUNCTION_OF_PTR_H
#define CLBL_MEMBER_FUNCTION_OF_PTR_H

#include <type_traits>

#include "CLBL/cv_checks.h"
#include "CLBL/callable.h"

namespace clbl {

    template<typename, typename, typename, typename DispatchFailureCase>
    struct member_function_of_ptr { static_assert(sizeof(DispatchFailureCase) < 0, "Not a member function."); };

    template<typename UnderlyingType, typename TPtr, typename MemberFunctionPointerType, typename Return, typename... Args>
    struct member_function_of_ptr<
        UnderlyingType,
        TPtr,
        MemberFunctionPointerType,
        Return(std::remove_cv_t<UnderlyingType>::*)(Args...)
    >
        : callable<member_function_of_ptr<UnderlyingType, TPtr, MemberFunctionPointerType, Return(std::remove_cv_t<UnderlyingType>::*)(Args...)>, Return(Args...)> {

        member_function_of_ptr()
        {}

        member_function_of_ptr(TPtr&& o_ptr, MemberFunctionPointerType f_ptr)
            : object_ptr(std::forward<TPtr>(o_ptr)),
            value(f_ptr)
        {}

        inline member_function_of_ptr(TPtr& o_ptr, MemberFunctionPointerType f_ptr)
            : object_ptr(o_ptr), value(f_ptr)
        {}

        using my_type = member_function_of_ptr<UnderlyingType, TPtr, MemberFunctionPointerType, Return(std::remove_cv_t<UnderlyingType>::*)(Args...)>;

        inline member_function_of_ptr(my_type& other) = default;
        inline member_function_of_ptr(const my_type& other) = default;
        inline member_function_of_ptr(my_type&& other) = default;

        inline member_function_of_ptr(volatile my_type& other)
            : object_ptr(other.object_ptr), value(other.value)
        {}

        inline member_function_of_ptr(const volatile my_type& other)
            : object_ptr(other.object_ptr), value(other.value)
        {}

        template<typename... Fargs>
        inline Return operator()(Fargs&&... a) {
            return ((*object_ptr).*value)(std::forward<Fargs>(a)...);
        }

        template<typename... Fargs>
        inline Return operator()(Fargs&&... a) const {
            return ((*object_ptr).*value)(std::forward<Fargs>(a)...);
        }

        template<typename... Fargs>
        inline auto operator()(Fargs&&... a) volatile {
            return ((*object_ptr).*value)(std::forward<Fargs>(a)...);
        }

        template<typename... Fargs>
        inline Return operator()(Fargs&&... a) const volatile {
            return ((*object_ptr).*value)(std::forward<Fargs>(a)...);
        }

        static constexpr auto clbl_is_deep_const = is_deep_const<UnderlyingType>()
            || std::is_const<UnderlyingType>::value;

        static constexpr auto clbl_is_deep_volatile = is_deep_volatile<UnderlyingType>()
            || std::is_volatile<UnderlyingType>::value;

        TPtr object_ptr;
        MemberFunctionPointerType value;
    };
}

#endif