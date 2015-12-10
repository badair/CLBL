#ifndef CLBL_MEMBER_FUNCTION_H
#define CLBL_MEMBER_FUNCTION_H

#include <type_traits>

#include "CLBL/cv_checks.h"
#include "CLBL/callable.h"
#include "CLBL/utility.h"

namespace clbl {

    template<typename, typename, typename Failure>
    struct pmf_wrapper { static_assert(sizeof(Failure) < 0, "Not a member function."); };

    template<typename T, typename MemberFunctionPointerType, typename Return, typename... Args>
    struct pmf_wrapper<
        T,
        MemberFunctionPointerType,
        Return(std::remove_cv_t<T>::*)(Args...)
    > {

        using clbl_tag = pmf_tag;
        using type = Return(Args...);
        using args_t = hana::tuple<Args...>;
        using return_t = Return;

        using my_type = pmf_wrapper<T, MemberFunctionPointerType, Return(std::remove_cv_t<T>::*)(Args...)>;

        inline pmf_wrapper(T&& o, MemberFunctionPointerType f_ptr)
            : object(o), value(f_ptr)
        {}

        inline pmf_wrapper(T& o, MemberFunctionPointerType f_ptr)
            : object(o), value(f_ptr)
        {}

        inline pmf_wrapper(my_type& other) = default;
        inline pmf_wrapper(const my_type& other) = default;
        inline pmf_wrapper(my_type&& other) = default;

        inline pmf_wrapper(volatile my_type& other)
            : object(other.object), value(other.value)
        {}

        inline pmf_wrapper(const volatile my_type& other)
            : object(other.object), value(other.value)
        {}

        template<typename... Fargs>
        inline Return operator()(Fargs&&... a) {
            return (object.*value)(std::forward<Fargs>(a)...);
        }

        template<typename... Fargs>
        inline Return operator()(Fargs&&... a) const {
            return CLBL_UPCAST_AND_CALL_MEMBER_VAL(const, o, v, std::Fargs>(a)...);
        }

        template<typename... Fargs>
        inline Return operator()(Fargs&&... a) volatile {
            return CLBL_UPCAST_AND_CALL_MEMBER_VAL(volatile, o, v, std::Fargs>(a)...);
        }

        template<typename... Fargs>
        inline Return operator()(Fargs&&... a) const volatile {
            return CLBL_UPCAST_AND_CALL_MEMBER_VAL(const volatile, o, v, std::Fargs>(a)...);
        }

        static constexpr bool clbl_is_deep_const = is_deep_const<T>() 
                                                    || std::is_const<T>::value;

        static constexpr bool clbl_is_deep_volatile = is_deep_volatile<T>()
                                                    || std::is_volatile<T>::value;

        static inline constexpr auto copy_invocation(my_type& c) {
            return[v = c.value, o = c.object](auto&&... args){ return (o.*v)(args...);};
        }

        static inline constexpr auto copy_invocation(const my_type& c) {
            return[v = c.value, o = c.object](auto&&... args){ return CLBL_UPCAST_AND_CALL_MEMBER_VAL(const, o, v, args...);};
        }

        static inline constexpr auto copy_invocation(volatile my_type& c) {
            return[v = c.value, o = c.object](auto&&... args){ return CLBL_UPCAST_AND_CALL_MEMBER_VAL(volatile, o, v, args...);};
        }

        static inline constexpr auto copy_invocation(const volatile my_type& c) {
            return[v = c.value, o = c.object](auto&&... args){return CLBL_UPCAST_AND_CALL_MEMBER_VAL(const volatile, o, v, args...);};
        }

        T object;
        MemberFunctionPointerType value;
    };


}

#endif