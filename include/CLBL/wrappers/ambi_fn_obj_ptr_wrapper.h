#ifndef CLBL_AMBI_FN_OBJ_PTR_WRAPPER_H
#define CLBL_AMBI_FN_OBJ_PTR_WRAPPER_H

#include <type_traits>

#include "CLBL/tags.h"
#include "CLBL/qualify_flags.h"
#include "CLBL/utility.h"
#include "CLBL/harden_cast.h"
#include "CLBL/invocation_macros.h"
#include "CLBL/invocation_data.h"

namespace clbl {

    /*
    ambi_fn_obj_ptr_wrapper wraps a pointer to a callable object whose 
    operator() is overloaded/templated. You may still call this
    wrapper as you would call the original object, but you must 
    disambiguate by calling clbl::harden before using any of the 
    additional features in CLBL.
    */
    template<typename Creator, qualify_flags CvFlags, typename TPtr>
    struct ambi_fn_obj_ptr_wrapper {

        using args_t = ambiguous_args;
        using clbl_tag = ambi_fn_obj_ptr_tag;
        using creator = Creator;
        using forwarding_glue = ambiguous_return(ambiguous_args);
        using invocation_data_type = ptr_invocation_data<TPtr>;
        using my_type = ambi_fn_obj_ptr_wrapper<Creator, CvFlags, TPtr>;
        using return_t = ambiguous_return;
        using type = ambiguous_return(ambiguous_args);
        using underlying_type = clbl::underlying_type<no_ref<decltype(*std::declval<TPtr>())> >;

        template<qualify_flags Flags>
        using apply_cv = ambi_fn_obj_ptr_wrapper<Creator, CvFlags | Flags, TPtr>;

        static constexpr auto cv_flags = CvFlags;
        static constexpr auto is_ambiguous = true;

        invocation_data_type data;

        inline ambi_fn_obj_ptr_wrapper(std::remove_const_t<TPtr>& o_ptr)
            : data{ o_ptr }
        {}

        inline ambi_fn_obj_ptr_wrapper(const TPtr& o_ptr)
            : data{ o_ptr }
        {}

        inline ambi_fn_obj_ptr_wrapper(TPtr&& o_ptr)
            : data{ std::forward<TPtr>(o_ptr) }
        {}

        inline ambi_fn_obj_ptr_wrapper(my_type& other) = default;
        inline ambi_fn_obj_ptr_wrapper(const my_type& other) = default;
        inline ambi_fn_obj_ptr_wrapper(my_type&& other) = default;
      
        inline ambi_fn_obj_ptr_wrapper(volatile my_type& other)
            : data(other.data)
        {}

        inline ambi_fn_obj_ptr_wrapper(const volatile my_type& other)
            : data(other.data)
        {}

        /*
        implicit conversion to other cv representations - will fail if it's a downcast. falls
        back to ctor defaults for self-safe substitution failure
        */
        template<qualify_flags Flags, std::enable_if_t<Flags != cv_flags, dummy>* = nullptr>
        inline ambi_fn_obj_ptr_wrapper(ambi_fn_obj_ptr_wrapper<Creator, Flags, TPtr>& other)
            : data(other.data)
        {}

        template<qualify_flags Flags, std::enable_if_t<Flags != cv_flags, dummy>* = nullptr>
        inline ambi_fn_obj_ptr_wrapper(const ambi_fn_obj_ptr_wrapper<Creator, Flags, TPtr>& other)
            : data(other.data)
        {}

        template<typename... Fargs>
        inline auto operator()(Fargs&&... a) {
            return CLBL_UPCAST_AND_CALL_PTR(__CLBL_NO_CV, data.ptr, std::forward<Fargs>(a)...);
        }

        template<typename... Fargs>
        inline auto operator()(Fargs&&... a) const {
            return CLBL_UPCAST_AND_CALL_PTR(const, data.ptr, std::forward<Fargs>(a)...);
        }

        template<typename... Fargs>
        inline auto operator()(Fargs&&... a) volatile {
            return CLBL_UPCAST_AND_CALL_PTR(volatile, data.ptr, std::forward<Fargs>(a)...);
        }

        template<typename... Fargs>
        inline auto operator()(Fargs&&... a) const volatile {
            return CLBL_UPCAST_AND_CALL_PTR(const volatile, data.ptr, std::forward<Fargs>(a)...);
        }

        static inline constexpr auto copy_invocation(my_type& c) {
            return[v = c.data.ptr](auto&&... args){ 
                return CLBL_UPCAST_AND_CALL_PTR(__CLBL_NO_CV, v, args...);
            };
        }

        static inline constexpr auto copy_invocation(const my_type& c) {
            return[v = c.data.ptr](auto&&... args){ 
                return CLBL_UPCAST_AND_CALL_PTR(const, v, args...);
            };
        }

        static inline constexpr auto copy_invocation(volatile my_type& c) {
            return[v = c.data.ptr](auto&&... args){ 
                return CLBL_UPCAST_AND_CALL_PTR(volatile, v, args...);
            };
        }

        static inline constexpr auto copy_invocation(const volatile my_type& c) {
            return[v = c.data.ptr](auto&&... args){ 
                return CLBL_UPCAST_AND_CALL_PTR(const volatile, v, args...);
            };
        }
    };
}

#endif