#ifndef CLBL_OVERLOADED_FUNCTION_OBJECT_PTR_H
#define CLBL_OVERLOADED_FUNCTION_OBJECT_PTR_H

#include "CLBL/tags.h"
#include "CLBL/cv_checks.h"
#include "CLBL/utility.h"
#include <type_traits>

namespace clbl {

    template<typename TPtr>
    struct ambi_fn_obj_ptr_wrapper {

        using clbl_tag = ambi_fn_obj_ptr_tag;
        using semantics = ptr_call_semantics;
        using type = ambiguous_return(ambiguous_args);
        using args_t = ambiguous_args;
        using return_t = ambiguous_return;

        using underlying_type = no_ref<decltype(*std::declval<TPtr>())>;

        ambi_fn_obj_ptr_wrapper()
        {}

        ambi_fn_obj_ptr_wrapper(TPtr&& o_ptr)
            : value(std::forward<TPtr>(o_ptr))
        {}

        using my_type = ambi_fn_obj_ptr_wrapper<TPtr>;

        ambi_fn_obj_ptr_wrapper(my_type& other) = default;
        ambi_fn_obj_ptr_wrapper(const my_type& other) = default;
        ambi_fn_obj_ptr_wrapper(my_type&& other) = default;
      
        inline ambi_fn_obj_ptr_wrapper(volatile my_type& other)
            : value(other.value)
        {}

        inline ambi_fn_obj_ptr_wrapper(const volatile my_type& other)
            : value(other.value)
        {}

        template<typename... Fargs>
        inline auto operator()(Fargs&&... a) {
            return (*value)(std::forward<Fargs>(a)...);
        }

        template<typename... Fargs>
        inline auto operator()(Fargs&&... a) const {
            return CLBL_UPCAST_AND_CALL_PTR(const, value, std::forward<Fargs>(a)...);
        }

        template<typename... Fargs>
        inline auto operator()(Fargs&&... a) volatile {
            return CLBL_UPCAST_AND_CALL_PTR(volatile, value, std::forward<Fargs>(a)...);
        }

        template<typename... Fargs>
        inline auto operator()(Fargs&&... a) const volatile {
            return CLBL_UPCAST_AND_CALL_PTR(const volatile, value, std::forward<Fargs>(a)...);
        }

        static constexpr bool clbl_is_deep_const = is_deep_const<underlying_type>()
                                                    || std::is_const<underlying_type>::value;


        static constexpr bool clbl_is_deep_volatile = is_deep_volatile<underlying_type>()
                                                    || std::is_volatile<underlying_type>::value;

        static inline constexpr auto copy_invocation(my_type& c) {
            return[v = c.value](auto&&... args){ return (*v)(args...);};
        }

        static inline constexpr auto copy_invocation(const my_type& c) {
            return[v = c.value](auto&&... args){ return CLBL_UPCAST_AND_CALL_PTR(const, v, args...);};
        }

        static inline constexpr auto copy_invocation(volatile my_type& c) {
            return[v = c.value](auto&&... args){ return CLBL_UPCAST_AND_CALL_PTR(volatile, v, args...);};
        }

        static inline constexpr auto copy_invocation(const volatile my_type& c) {
            return[v = c.value](auto&&... args){ return CLBL_UPCAST_AND_CALL_PTR(const volatile, v, args...);};
        }

        TPtr value;
    };
}

#endif