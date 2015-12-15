#ifndef CLBL_AMBI_FN_OBJ_WRAPPER_H
#define CLBL_AMBI_FN_OBJ_WRAPPER_H

#include "CLBL/tags.h"
#include "CLBL/qualify_flags.h"
#include "CLBL/utility.h"
#include "CLBL/harden_cast.h"
#include "CLBL/invocation_macros.h"

#include <type_traits>

namespace clbl {

    template<typename Creator, qualify_flags CvFlags, typename T>
    struct ambi_fn_obj_wrapper {

        static constexpr qualify_flags cv_flags = CvFlags;
        using creator = Creator;
        using clbl_tag = ambi_fn_obj_tag;
        using semantics = ptr_call_semantics;
        using type = ambiguous_return(ambiguous_args);
        using forwarding_glue = ambiguous_return(ambiguous_args);
        using args_t = ambiguous_args;
        using return_t = ambiguous_return;
        using my_type = ambi_fn_obj_wrapper<Creator, cv_flags, T>;
        using underlying_type = T;

        T _value;
        dummy _object;

        template<qualify_flags Flags>
        using apply_cv = ambi_fn_obj_wrapper<Creator, CvFlags | Flags, T>;

        inline ambi_fn_obj_wrapper(const std::remove_const_t<T>& o, dummy d = dummy{})
            : _value(o)
        {}

        inline ambi_fn_obj_wrapper(my_type& other) = default;
        inline ambi_fn_obj_wrapper(const my_type& other) = default;

        inline ambi_fn_obj_wrapper(volatile my_type& other)
            : _value(other._value)
        {}

        inline ambi_fn_obj_wrapper(const volatile my_type& other)
            : _value(other._value)
        {}

        inline operator underlying_type&(){
            return _value;
        }

        template<typename... Fargs>
        inline auto operator()(Fargs&&... a) {
            return CLBL_UPCAST_AND_CALL_VAL(CLBL_NOTHING, _value, std::forward<Fargs>(a)...);
        }

        template<typename... Fargs>
        inline auto operator()(Fargs&&... a) const {
            return CLBL_UPCAST_AND_CALL_VAL(const, _value, std::forward<Fargs>(a)...);
        }

        template<typename... Fargs>
        inline auto operator()(Fargs&&... a) volatile {
            return CLBL_UPCAST_AND_CALL_VAL(volatile, _value, std::forward<Fargs>(a)...);
        }

        template<typename... Fargs>
        inline auto operator()(Fargs&&... a) const volatile {
            return CLBL_UPCAST_AND_CALL_VAL(const volatile, _value, std::forward<Fargs>(a)...);
        }

        static inline constexpr auto copy_invocation(my_type& c) {
            return [v = c._value](auto&&... args) mutable { 
                return CLBL_UPCAST_AND_CALL_VAL(CLBL_NOTHING, v, args...);
            };
        }

        static inline constexpr auto copy_invocation(const my_type& c) {
            return [v = c._value](auto&&... args){ 
                return CLBL_UPCAST_AND_CALL_VAL(const, v, args...);
            };
        }

        static inline constexpr auto copy_invocation(volatile my_type& c) {
            return [v = c._value](auto&&... args) mutable { 
                return CLBL_UPCAST_AND_CALL_VAL(volatile, v, args...);
            };
        }

        static inline constexpr auto copy_invocation(const volatile my_type& c) {
            return [v = c._value](auto&&... args){ 
                return CLBL_UPCAST_AND_CALL_VAL(const volatile, v, args...);
            };
        }
    };
}

#endif