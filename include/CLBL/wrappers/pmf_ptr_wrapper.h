#ifndef CLBL_PMF_PTR_WRAPPER_H
#define CLBL_PMF_PTR_WRAPPER_H

#include <type_traits>

#include "CLBL/utility.h"
#include "CLBL/forward.h"
#include "CLBL/harden_cast.h"
#include "CLBL/invocation_macros.h"

namespace clbl {

    template<typename, qualify_flags, typename, typename, typename, typename DispatchFailureCase>
    struct pmf_ptr_wrapper { static_assert(sizeof(DispatchFailureCase) < 0, "Not a member function."); };

    template<typename Creator, qualify_flags CvFlags, typename UnderlyingType, typename TPtr, typename TMemberFnPtr, typename Return, typename... Args>
    struct pmf_ptr_wrapper<Creator, CvFlags, UnderlyingType, TPtr, TMemberFnPtr, Return(std::remove_cv_t<UnderlyingType>::*)(Args...)> {
        
        static constexpr qualify_flags cv_flags = CvFlags;
        using creator = Creator;
        using clbl_tag = pmf_ptr_tag;
        using type = Return(Args...);
        using forwarding_glue = Return(forward<Args>...);
        using args_t = hana::tuple<Args...>;
        using return_t = Return;
        using my_type = pmf_ptr_wrapper<Creator, cv_flags, UnderlyingType, TPtr, TMemberFnPtr, Return(std::remove_cv_t<UnderlyingType>::*)(Args...)>;

        using underlying_type = clbl::underlying_type<UnderlyingType>;

        TMemberFnPtr _value;
        TPtr _object;

        template<qualify_flags Flags>
        using apply_cv = pmf_ptr_wrapper<Creator, cv_flags | Flags, UnderlyingType, TPtr, TMemberFnPtr, Return(std::remove_cv_t<UnderlyingType>::*)(Args...)>;

        pmf_ptr_wrapper()
        {}

        pmf_ptr_wrapper(TMemberFnPtr f_ptr, TPtr&& o_ptr)
            : _value(f_ptr), _object(std::forward<TPtr>(o_ptr))
        {}

        inline pmf_ptr_wrapper(TMemberFnPtr f_ptr, TPtr& o_ptr)
            :  _value(f_ptr), _object(o_ptr)
        {}

        inline pmf_ptr_wrapper(my_type& other) = default;
        inline pmf_ptr_wrapper(const my_type& other) = default;
        inline pmf_ptr_wrapper(my_type&& other) = default;

        /*
        implicit conversion to other cv representations - will fail if it's a downcast. falls 
        back to ctor defaults for self-safe substitution failure 
        */

        template<qualify_flags Flags, std::enable_if_t<Flags != cv_flags, dummy>* = nullptr>
        inline pmf_ptr_wrapper(pmf_ptr_wrapper<Creator, Flags, UnderlyingType, TPtr, TMemberFnPtr, Return(std::remove_cv_t<UnderlyingType>::*)(Args...)>& other)
            : _value(other._value), _object(other._object)
        {}

        template<qualify_flags Flags, std::enable_if_t<Flags != cv_flags, dummy>* = nullptr>
        inline pmf_ptr_wrapper(const pmf_ptr_wrapper<Creator, Flags, UnderlyingType, TPtr, TMemberFnPtr, Return(std::remove_cv_t<UnderlyingType>::*)(Args...)>& other)
            : _value(other._value), _object(other._object)
        {}

        inline pmf_ptr_wrapper(volatile my_type& other)
            : _value(other._value), _object(other._object)
        {}

        inline pmf_ptr_wrapper(const volatile my_type& other)
            : _value(other._value), _object(other._object)
        {}

        template<typename... Fargs>
        inline Return operator()(Fargs&&... a) {
            return CLBL_UPCAST_AND_CALL_MEMBER_PTR(CLBL_NOTHING, _object, _value, std::forward<Fargs>(a)...);
        }

        template<typename... Fargs>
        inline Return operator()(Fargs&&... a) const {
            return CLBL_UPCAST_AND_CALL_MEMBER_PTR(const, _object, _value, std::forward<Fargs>(a)...);
        }

        template<typename... Fargs>
        inline Return operator()(Fargs&&... a) volatile {
            return CLBL_UPCAST_AND_CALL_MEMBER_PTR(volatile, _object, _value, std::forward<Fargs>(a)...);
        }

        template<typename... Fargs>
        inline Return operator()(Fargs&&... a) const volatile {
            return CLBL_UPCAST_AND_CALL_MEMBER_PTR(const volatile, _object, _value, std::forward<Fargs>(a)...);
        }

        template<typename T = UnderlyingType, std::enable_if_t<is_clbl<T>, dummy>* = nullptr>
        static inline constexpr auto copy_invocation(T& c){
            return no_ref<decltype(*_object)>::copy_invocation(harden_cast<cv_flags>(*c._object));
        }

        template<typename T = UnderlyingType, std::enable_if_t<is_clbl<T>, dummy>* = nullptr>
        static inline constexpr auto copy_invocation(const T& c) {
            return no_ref<decltype(*_object)>::copy_invocation(harden_cast<qflags::const_ | cv_flags>(*c._object));
        }

        template<typename T = UnderlyingType, std::enable_if_t<is_clbl<T>, dummy>* = nullptr>
        static inline constexpr auto copy_invocation(volatile T& c) {
            return no_ref<decltype(*_object)>::copy_invocation(harden_cast<qflags::volatile_ | cv_flags>(*c._object));
        }

        template<typename T = UnderlyingType, std::enable_if_t<is_clbl<T>, dummy>* = nullptr>
        static inline constexpr auto copy_invocation(const volatile T& c) {
            return no_ref<decltype(*_object)>::copy_invocation(harden_cast<qflags::const_ | qflags::volatile_ | cv_flags>(*c._object));
        }

        template<typename T = UnderlyingType, std::enable_if_t<!is_clbl<T>, dummy>* = nullptr>
        static inline constexpr auto copy_invocation(my_type& c) {
            return[v = c._value, o = c._object](auto&&... args){
                return CLBL_UPCAST_AND_CALL_MEMBER_PTR(CLBL_NOTHING, o, v, args...);
            };
        }

        template<typename T = UnderlyingType, std::enable_if_t<!is_clbl<T>, dummy>* = nullptr>
        static inline constexpr auto copy_invocation(const my_type& c) {
            return[v = c._value, o = c._object](auto&&... args){ 
                return CLBL_UPCAST_AND_CALL_MEMBER_PTR(const, o, v, args...);
            };
        }

        template<typename T = UnderlyingType, std::enable_if_t<!is_clbl<T>, dummy>* = nullptr>
        static inline constexpr auto copy_invocation(volatile my_type& c) {
            return[v = c._value, o = c._object](auto&&... args){ 
                return CLBL_UPCAST_AND_CALL_MEMBER_PTR(volatile, o, v, args...);
            };
        }

        template<typename T = UnderlyingType, std::enable_if_t<!is_clbl<T>, dummy>* = nullptr>
        static inline constexpr auto copy_invocation(const volatile my_type& c) {
            return[v = c._value, o = c._object](auto&&... args){ 
                return CLBL_UPCAST_AND_CALL_MEMBER_PTR(const volatile, o, v, args...);
            };
        }
    };
}

#endif