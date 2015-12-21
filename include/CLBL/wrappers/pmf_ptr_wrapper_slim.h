#ifndef CLBL_PMF_PTR_WRAPPER_SLIM_H
#define CLBL_PMF_PTR_WRAPPER_SLIM_H

#include <type_traits>
#include <tuple>

#include "CLBL/utility.h"
#include "CLBL/forward.h"
#include "CLBL/harden_cast.h"
#include "CLBL/invocation_macros.h"
#include "CLBL/invocation_data.h"

namespace clbl {

    template<typename, qualify_flags, typename, typename, typename TMemberFnPtr, TMemberFnPtr, typename DispatchFailureCase>
    struct pmf_ptr_wrapper_slim { static_assert(sizeof(DispatchFailureCase) < 0, "Not a member function."); };

    template<typename Creator, qualify_flags CvFlags, typename UnderlyingType, typename TPtr, typename TMemberFnPtr, TMemberFnPtr Pmf, typename Return, typename... Args>
    struct pmf_ptr_wrapper_slim<Creator, CvFlags, UnderlyingType, TPtr, TMemberFnPtr, Pmf, Return(std::remove_cv_t<UnderlyingType>::*)(Args...)> {
        
        using args_t = std::tuple<Args...>;
        using clbl_tag = pmf_ptr_tag;
        using creator = Creator;
        using forwarding_glue = Return(forward<Args>...);
        using invocation_data_type = indirect_pmf_invocation_data_slim<TPtr, TMemberFnPtr, Pmf>;
        using my_type = pmf_ptr_wrapper_slim<Creator, CvFlags, UnderlyingType, TPtr, TMemberFnPtr, Pmf, Return(std::remove_cv_t<UnderlyingType>::*)(Args...)>;
        using return_t = Return;
        using type = Return(Args...);
        using underlying_type = clbl::underlying_type<UnderlyingType>;

        template<qualify_flags Flags>
        using apply_cv = pmf_ptr_wrapper_slim<Creator, CvFlags | Flags, UnderlyingType, TPtr, TMemberFnPtr, Pmf, Return(std::remove_cv_t<UnderlyingType>::*)(Args...)>;

        static constexpr auto cv_flags = CvFlags;
        static constexpr auto is_ambiguous = std::is_same<Return(Args...), ambiguous_return(ambiguous_args)>::value;

        invocation_data_type data;

        pmf_ptr_wrapper_slim()
        {}

        pmf_ptr_wrapper_slim(TPtr&& o_ptr)
            : data{ std::forward<TPtr>(o_ptr) }
        {}

        inline pmf_ptr_wrapper_slim(TPtr& o_ptr)
            : data{ o_ptr }
        {}

        inline pmf_ptr_wrapper_slim(my_type& other) = default;
        inline pmf_ptr_wrapper_slim(const my_type& other) = default;
        inline pmf_ptr_wrapper_slim(my_type&& other) = default;

        template<typename... Fargs>
        inline Return operator()(Fargs&&... a) {
            return CLBL_UPCAST_AND_CALL_MEMBER_PTR(CLBL_NOTHING, data.object_ptr, invocation_data_type::pmf, std::forward<Fargs>(a)...);
        }

        template<typename... Fargs>
        inline Return operator()(Fargs&&... a) const {
            return CLBL_UPCAST_AND_CALL_MEMBER_PTR(const, data.object_ptr, invocation_data_type::pmf, std::forward<Fargs>(a)...);
        }

        template<typename... Fargs>
        inline Return operator()(Fargs&&... a) volatile {
            return CLBL_UPCAST_AND_CALL_MEMBER_PTR(volatile, data.object_ptr, invocation_data_type::pmf, std::forward<Fargs>(a)...);
        }

        template<typename... Fargs>
        inline Return operator()(Fargs&&... a) const volatile {
            return CLBL_UPCAST_AND_CALL_MEMBER_PTR(const volatile, data.object_ptr, invocation_data_type::pmf, std::forward<Fargs>(a)...);
        }

        template<typename T = UnderlyingType, std::enable_if_t<is_clbl<T>, dummy>* = nullptr>
        static inline constexpr auto copy_invocation(T& c){
            return no_ref<decltype(*c.data.object_ptr)>::copy_invocation(harden_cast<cv_flags>(*c.data.object_ptr));
        }

        template<typename T = UnderlyingType, std::enable_if_t<is_clbl<T>, dummy>* = nullptr>
        static inline constexpr auto copy_invocation(const T& c) {
            return no_ref<decltype(*c.data.object_ptr)>::copy_invocation(harden_cast<qflags::const_ | cv_flags>(*c.data.object_ptr));
        }

        template<typename T = UnderlyingType, std::enable_if_t<is_clbl<T>, dummy>* = nullptr>
        static inline constexpr auto copy_invocation(volatile T& c) {
            return no_ref<decltype(*c.data.object_ptr)>::copy_invocation(harden_cast<qflags::volatile_ | cv_flags>(*c.data.object_ptr));
        }

        template<typename T = UnderlyingType, std::enable_if_t<is_clbl<T>, dummy>* = nullptr>
        static inline constexpr auto copy_invocation(const volatile T& c) {
            return no_ref<decltype(*c.data.object_ptr)>::copy_invocation(harden_cast<qflags::const_ | qflags::volatile_ | cv_flags>(*c.data.object_ptr));
        }

        template<typename T = UnderlyingType, std::enable_if_t<!is_clbl<T>, dummy>* = nullptr>
        static inline constexpr auto copy_invocation(my_type& c) {
            return[d = c.data](auto&&... args) mutable {
                return CLBL_UPCAST_AND_CALL_MEMBER_PTR(CLBL_NOTHING, d.object_ptr, decltype(d)::pmf, args...);
            };
        }

        template<typename T = UnderlyingType, std::enable_if_t<!is_clbl<T>, dummy>* = nullptr>
        static inline constexpr auto copy_invocation(const my_type& c) {
            return[d = c.data](auto&&... args){ 
                return CLBL_UPCAST_AND_CALL_MEMBER_PTR(const, d.object_ptr, decltype(d)::pmf, args...);
            };
        }

        template<typename T = UnderlyingType, std::enable_if_t<!is_clbl<T>, dummy>* = nullptr>
        static inline constexpr auto copy_invocation(volatile my_type& c) {
            return[d = c.data](auto&&... args) mutable { 
                return CLBL_UPCAST_AND_CALL_MEMBER_PTR(volatile, d.object_ptr, decltype(d)::pmf, args...);
            };
        }

        template<typename T = UnderlyingType, std::enable_if_t<!is_clbl<T>, dummy>* = nullptr>
        static inline constexpr auto copy_invocation(const volatile my_type& c) {
            return[d = c.data](auto&&... args){ 
                return CLBL_UPCAST_AND_CALL_MEMBER_PTR(const volatile, d.object_ptr, decltype(d)::pmf, args...);
            };
        }
    };
}

#endif