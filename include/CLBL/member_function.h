#ifndef CLBL_MEMBER_FUNCTION_H
#define CLBL_MEMBER_FUNCTION_H

#include <type_traits>

namespace clbl {

    //dispatch failure case for member functions
    template<typename, typename, typename, typename Failure>
    struct member_function {

        member_function() {}

        template<typename... T>
        member_function(T...) {}

        template<typename... T>
        void operator()(T...) {
            static_assert(sizeof...(T) < 0, "Not a member function.");
        }

        template<typename... T>
        void operator()(T..., ...) {
            static_assert(sizeof...(T) < 0, "Not a member function.");
        }
    };

    template<typename UnderlyingType, typename T, typename MemberFunctionPointerType, typename Return, typename... Args>
    struct member_function<
        UnderlyingType,
        T,
        MemberFunctionPointerType,
        Return(UnderlyingType::*)(Args...)
    >
        : callable<member_function<UnderlyingType, T, MemberFunctionPointerType, Return(UnderlyingType::*)(Args...)>, Return(Args...)> {

        member_function(T o, MemberFunctionPointerType f_ptr)
            : object(o),
            member_function_ptr(f_ptr)
        {}

        template<typename... Fargs>
        inline Return operator()(Fargs&&... a) {
            return (object.*member_function_ptr)(std::forward<Fargs>(a)...);
        }

        template<typename... Fargs>
        inline Return operator()(Fargs&&... a) const {
            return (object.*member_function_ptr)(std::forward<Fargs>(a)...);
        }

        template<typename... Fargs>
        inline Return operator()(Fargs&&... a) volatile {
            return (object.*member_function_ptr)(std::forward<Fargs>(a)...);
        }

        template<typename... Fargs>
        inline Return operator()(Fargs&&... a) const volatile {
            return (object.*member_function_ptr)(std::forward<Fargs>(a)...);
        }

    private:
        T object;
        MemberFunctionPointerType member_function_ptr;
    };

    //dispatch failure case for member functions
    template<typename, typename, typename, typename Failure>
    struct member_function_of_ptr {

        member_function_of_ptr() {}

        template<typename... T>
        member_function_of_ptr(T...) {}

        template<typename... T>
        void operator()(T...) {
            static_assert(sizeof...(T) < 0, "Not a member function.");
        }

        template<typename... T>
        void operator()(T..., ...) {
            static_assert(sizeof...(T) < 0, "Not a member function.");
        }
    };

    template<typename UnderlyingType, typename TPtr, typename MemberFunctionPointerType, typename Return, typename... Args>
    struct member_function_of_ptr<
        UnderlyingType,
        TPtr,
        MemberFunctionPointerType,
        Return(UnderlyingType::*)(Args...)
    >
    : callable<member_function_of_ptr<UnderlyingType, TPtr, MemberFunctionPointerType, Return(UnderlyingType::*)(Args...)>, Return(Args...)> {

        member_function_of_ptr(TPtr&& o_ptr, MemberFunctionPointerType f_ptr)
            : object_ptr(std::forward<TPtr>(o_ptr)),
            member_function_ptr(f_ptr)
        {}

        template<typename... Fargs>
        inline Return operator()(Fargs&&... a) {
            return ((*object_ptr).*member_function_ptr)(std::forward<Fargs>(a)...);
        }

        template<typename... Fargs>
        inline Return operator()(Fargs&&... a) const {
            return ((*object_ptr).*member_function_ptr)(std::forward<Fargs>(a)...);
        }

        template<typename... Fargs>
        inline Return operator()(Fargs&&... a) volatile {
            return ((*object_ptr).*member_function_ptr)(std::forward<Fargs>(a)...);
        }

        template<typename... Fargs>
        inline Return operator()(Fargs&&... a) const volatile {
            return ((*object_ptr).*member_function_ptr)(std::forward<Fargs>(a)...);
        }

    private:
        TPtr object_ptr;
        MemberFunctionPointerType member_function_ptr;
    };
}

#endif