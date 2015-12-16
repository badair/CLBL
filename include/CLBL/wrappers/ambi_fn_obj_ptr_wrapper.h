#ifndef CLBL_AMBI_FN_OBJ_PTR_WRAPPER_H
#define CLBL_AMBI_FN_OBJ_PTR_WRAPPER_H

#include <type_traits>

#include "CLBL/tags.h"
#include "CLBL/qualify_flags.h"
#include "CLBL/utility.h"
#include "CLBL/harden_cast.h"
#include "CLBL/invocation_macros.h"

namespace clbl {

    /*
    "ambiguous function object pointer wrapper" - wraps a pointer to a 
    callable object whose operator() is overloaded and/or templated.
    You may still call this wrapper as you would call the original
    object, but you must disambiguate by calling clbl::harden before
    using any of the additional features in CLBL
    */
    template<typename Creator, qualify_flags CvFlags, typename TPtr>
    struct ambi_fn_obj_ptr_wrapper {

        static constexpr qualify_flags cv_flags = CvFlags;
        using creator = Creator;
        using clbl_tag = ambi_fn_obj_ptr_tag;
        using semantics = ptr_call_semantics;
        using type = ambiguous_return(ambiguous_args);
        using forwarding_glue = ambiguous_return(ambiguous_args);
        using args_t = ambiguous_args;
        using return_t = ambiguous_return;
        using my_type = ambi_fn_obj_ptr_wrapper<Creator, CvFlags, TPtr>;
        using underlying_type = no_ref<decltype(*std::declval<TPtr>())>;

        TPtr _value;
        dummy _object;

        template<qualify_flags Flags>
        using apply_cv = ambi_fn_obj_ptr_wrapper<Creator, CvFlags | Flags, TPtr>;

        ambi_fn_obj_ptr_wrapper()
        {}

        ambi_fn_obj_ptr_wrapper(const TPtr& o_ptr, dummy d = dummy{})
            : _value(std::forward<TPtr>(o_ptr))
        {}

        ambi_fn_obj_ptr_wrapper(TPtr&& o_ptr, dummy d = dummy{})
            : _value(std::forward<TPtr>(o_ptr))
        {}

        ambi_fn_obj_ptr_wrapper(my_type& other) = default;
        ambi_fn_obj_ptr_wrapper(const my_type& other) = default;
        ambi_fn_obj_ptr_wrapper(my_type&& other) = default;
      
        inline ambi_fn_obj_ptr_wrapper(volatile my_type& other)
            : _value(other._value)
        {}

        inline ambi_fn_obj_ptr_wrapper(const volatile my_type& other)
            : _value(other._value)
        {}

        /*
        implicit conversion to other cv representations - will fail if it's a downcast. falls
        back to ctor defaults for self-safe substitution failure
        */
        template<qualify_flags Flags, std::enable_if_t<Flags != cv_flags, dummy>* = nullptr>
        inline ambi_fn_obj_ptr_wrapper(ambi_fn_obj_ptr_wrapper<Creator, Flags, TPtr>& other)
            : _value(other._value), _object(other._object)
        {}

        template<qualify_flags Flags, std::enable_if_t<Flags != cv_flags, dummy>* = nullptr>
        inline ambi_fn_obj_ptr_wrapper(const ambi_fn_obj_ptr_wrapper<Creator, Flags, TPtr>& other)
            : _value(other._value), _object(other._object)
        {}

        inline operator underlying_type&() {
            return _value;
        }

        template<typename... Fargs>
        inline auto operator()(Fargs&&... a) {
            return CLBL_UPCAST_AND_CALL_PTR(CLBL_NOTHING, _value, std::forward<Fargs>(a)...);
        }

        template<typename... Fargs>
        inline auto operator()(Fargs&&... a) const {
            return CLBL_UPCAST_AND_CALL_PTR(const, _value, std::forward<Fargs>(a)...);
        }

        template<typename... Fargs>
        inline auto operator()(Fargs&&... a) volatile {
            return CLBL_UPCAST_AND_CALL_PTR(volatile, _value, std::forward<Fargs>(a)...);
        }

        template<typename... Fargs>
        inline auto operator()(Fargs&&... a) const volatile {
            return CLBL_UPCAST_AND_CALL_PTR(const volatile, _value, std::forward<Fargs>(a)...);
        }

        static inline constexpr auto copy_invocation(my_type& c) {
            return[v = c._value](auto&&... args){ return CLBL_UPCAST_AND_CALL_PTR(CLBL_NOTHING, v, args...);};
        }

        static inline constexpr auto copy_invocation(const my_type& c) {
            return[v = c._value](auto&&... args){ return CLBL_UPCAST_AND_CALL_PTR(const, v, args...);};
        }

        static inline constexpr auto copy_invocation(volatile my_type& c) {
            return[v = c._value](auto&&... args){ return CLBL_UPCAST_AND_CALL_PTR(volatile, v, args...);};
        }

        static inline constexpr auto copy_invocation(const volatile my_type& c) {
            return[v = c._value](auto&&... args){ return CLBL_UPCAST_AND_CALL_PTR(const volatile, v, args...);};
        }
    };
}

#endif