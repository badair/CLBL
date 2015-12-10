#ifndef CLBL_MEMBER_FUNCTION_OF_PTR_H
#define CLBL_MEMBER_FUNCTION_OF_PTR_H

#include <type_traits>

#include "CLBL/cv_checks.h"
#include "CLBL/callable.h"
#include "CLBL/utility.h"

namespace clbl {

    template<typename, typename, typename, typename DispatchFailureCase>
    struct pmf_ptr_wrapper { static_assert(sizeof(DispatchFailureCase) < 0, "Not a member function."); };

    template<typename UnderlyingType, typename TPtr, typename MemberFunctionPointerType, typename Return, typename... Args>
    struct pmf_ptr_wrapper<
        UnderlyingType,
        TPtr,
        MemberFunctionPointerType,
        Return(std::remove_cv_t<UnderlyingType>::*)(Args...)
    > {
        using clbl_tag = pmf_ptr_tag;
        using type = Return(Args...);
        using args_t = hana::tuple<Args...>;
        using return_t = Return;

        pmf_ptr_wrapper()
        {}

        pmf_ptr_wrapper(TPtr&& o_ptr, MemberFunctionPointerType f_ptr)
            : object_ptr(std::forward<TPtr>(o_ptr)),
            value(f_ptr)
        {}

        inline pmf_ptr_wrapper(TPtr& o_ptr, MemberFunctionPointerType f_ptr)
            : object_ptr(o_ptr), value(f_ptr)
        {}

        using my_type = pmf_ptr_wrapper<UnderlyingType, TPtr, MemberFunctionPointerType, Return(std::remove_cv_t<UnderlyingType>::*)(Args...)>;

        inline pmf_ptr_wrapper(my_type& other) = default;
        inline pmf_ptr_wrapper(const my_type& other) = default;
        inline pmf_ptr_wrapper(my_type&& other) = default;

        inline pmf_ptr_wrapper(volatile my_type& other)
            : object_ptr(other.object_ptr), value(other.value)
        {}

        inline pmf_ptr_wrapper(const volatile my_type& other)
            : object_ptr(other.object_ptr), value(other.value)
        {}

        template<typename... Fargs>
        inline Return operator()(Fargs&&... a) {
            return ((*object_ptr).*value)(std::forward<Fargs>(a)...);
        }

        template<typename... Fargs>
        inline Return operator()(Fargs&&... a) const {
            return CLBL_UPCAST_AND_CALL_MEMBER_PTR(const, object_ptr, value, std::forward<Fargs>(a)...);
        }

        template<typename... Fargs>
        inline auto operator()(Fargs&&... a) volatile {
            return CLBL_UPCAST_AND_CALL_MEMBER_PTR(volatile, object_ptr, value, std::forward<Fargs>(a)...);
        }

        template<typename... Fargs>
        inline Return operator()(Fargs&&... a) const volatile {
            return CLBL_UPCAST_AND_CALL_MEMBER_PTR(const volatile, object_ptr, value, std::forward<Fargs>(a)...);
        }

        static constexpr auto clbl_is_deep_const = is_deep_const<UnderlyingType>()
            || std::is_const<UnderlyingType>::value;

        static constexpr auto clbl_is_deep_volatile = is_deep_volatile<UnderlyingType>()
            || std::is_volatile<UnderlyingType>::value;

        static inline constexpr auto copy_invocation(my_type& c) {
            return[v = c.value, o = c.object_ptr](auto&&... args){ return ((*o).*v)(args...);};
        }

        static inline constexpr auto copy_invocation(const my_type& c) {
            return[v = c.value, o = c.object_ptr](auto&&... args){ 
                return CLBL_UPCAST_AND_CALL_MEMBER_PTR(const, o, v, args...);
            };
        }

        static inline constexpr auto copy_invocation(volatile my_type& c) {
            return[v = c.value, o = c.object_ptr](auto&&... args){ 
                return CLBL_UPCAST_AND_CALL_MEMBER_PTR(volatile, o, v, args...);
            };
        }

        static inline constexpr auto copy_invocation(const volatile my_type& c) {
            return[v = c.value, o = c.object_ptr](auto&&... args){ 
                return CLBL_UPCAST_AND_CALL_MEMBER_PTR(const volatile, o, v, args...);
            };
        }

        TPtr object_ptr;
        MemberFunctionPointerType value;
    };
}

#endif