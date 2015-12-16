#ifndef CLBL_FWRAP_H
#define CLBL_FWRAP_H

#include <type_traits>
#include <functional>

#include "CLBL/wrappers/free_fn_wrapper.h"
#include "CLBL/wrappers/pmf_wrapper.h"
#include "CLBL/wrappers/pmf_ptr_wrapper.h"
#include "CLBL/wrappers/ambi_fn_obj_wrapper.h"
#include "CLBL/wrappers/ambi_fn_obj_ptr_wrapper.h"
#include "CLBL/wrap/free_function.h"
#include "CLBL/wrap/function_object.h"
#include "CLBL/wrap/pointer_to_function_object.h"
#include "CLBL/wrap/member_function_with_object.h"
#include "CLBL/wrap/member_function_with_pointer_to_object.h"
#include "CLBL/member_function_decay.h"
#include "CLBL/tags.h"
#include "CLBL/qualify_flags.h"
#include "CLBL/utility.h"
#include "CLBL/is_valid.h"

namespace clbl {
    namespace detail {

        auto has_normal_call_operator_impl = is_valid([](auto arg)->decltype(&decltype(arg)::operator()) {});
        auto ptr_has_normal_call_operator_impl = is_valid([](auto arg)->decltype(&no_ref<decltype(*arg)>::operator()) {});

        template<typename T>
        constexpr bool has_normal_call_operator = decltype(has_normal_call_operator_impl(std::declval<T>()))::value;

        template<typename T>
        constexpr bool ptr_has_normal_call_operator = decltype(ptr_has_normal_call_operator_impl(std::declval<T>()))::value;

        template<typename T> 
        struct sfinae_switch {
        private:
            
            static constexpr auto is_ref_wrapper = is_reference_wrapper<T>;
            static constexpr auto is_ambiguous = !has_normal_call_operator<T>;
            static constexpr auto ptr_is_ambiguous = !ptr_has_normal_call_operator<T>;
            static constexpr auto is_function_ptr = std::is_function<std::remove_pointer_t<T> >::value;
            static constexpr auto is_member_function_ptr = std::is_member_function_pointer<T>::value;

        public:
            static constexpr auto is_ptr = can_dereference<no_ref<T>>;
            static constexpr auto reference_wrapper_case = is_ref_wrapper;
            static constexpr auto function_ptr_case = is_function_ptr;
            static constexpr auto member_function_ptr_case = !is_function_ptr && is_member_function_ptr;

        private:
            static constexpr auto is_complex_case = !reference_wrapper_case && !function_ptr_case && !member_function_ptr_case;

        public:
            static constexpr auto function_object_case =                is_complex_case && !is_ptr && !is_ambiguous;
            static constexpr auto ambiguous_function_object_case =     is_complex_case && !is_ptr && is_ambiguous;

            static constexpr auto function_object_ptr_case =            is_complex_case && is_ptr && !ptr_is_ambiguous;
            static constexpr auto ambiguous_function_object_ptr_case = is_complex_case && is_ptr &&  ptr_is_ambiguous;
        };
    }

    /********************
    free function pointer
    *********************/

    template<typename T, std::enable_if_t<detail::sfinae_switch<T>::function_ptr_case, dummy>* = nullptr>
    inline constexpr auto 
    fwrap(T&& t) {
        return free_function::template wrap<qflags::default_>(std::forward<T>(t));
    }

    /***********************************************
    Pointer to object with a member function pointer
    ************************************************/

    template<typename T, typename TMemberFnPtr, std::enable_if_t<detail::sfinae_switch<TMemberFnPtr>::member_function_ptr_case && detail::sfinae_switch<T>::is_ptr, dummy>* = nullptr>
    inline constexpr auto 
    fwrap(T&& t, TMemberFnPtr member_fn_ptr) {
        return member_function_with_pointer_to_object::template wrap<qflags::default_>(member_fn_ptr, std::forward<T>(t));
    }

    /**********************************
    Object with member function pointer
    ***********************************/

    template<typename T, typename TMemberFnPtr, std::enable_if_t<detail::sfinae_switch<TMemberFnPtr>::member_function_ptr_case && !detail::sfinae_switch<T>::is_ptr && !detail::sfinae_switch<T>::reference_wrapper_case, dummy>* = nullptr>
    inline constexpr auto 
    fwrap(T&& t, TMemberFnPtr member_fn_ptr) {
        return member_function_with_object::template wrap<qflags::default_>(member_fn_ptr, std::forward<T>(t));
    }

    /********************************
    Pointer to object with operator()
    *********************************/

    template<typename T, std::enable_if_t<detail::sfinae_switch<T>::function_object_ptr_case, dummy>* = nullptr>
    inline constexpr auto 
    fwrap(T&& t) {
        return pointer_to_function_object::template wrap<qflags::default_>(std::forward<T>(t));
    }

    /*****************************************************************
    Pointer to object with AMBIGUOUS (templated/overloaded) operator() 
    ******************************************************************/

    template<typename T, std::enable_if_t<detail::sfinae_switch<T>::ambiguous_function_object_ptr_case, dummy>* = nullptr>
    inline constexpr auto 
    fwrap(T&& t) {
        return pointer_to_function_object::ambiguous::template wrap<qflags::default_>(std::forward<T>(t));
    }

    /**********************
    Object with operator()
    ***********************/

    template<typename T, std::enable_if_t<detail::sfinae_switch<T>::function_object_case, dummy>* = nullptr>
    inline constexpr auto 
    fwrap(T&& t) {
        return function_object::template wrap<qflags::default_>(std::forward<T>(t));
    }

    /******************************************************
    Object with AMBIGUOUS (templated/overloaded) operator()
    *******************************************************/

    template<typename T, std::enable_if_t<detail::sfinae_switch<T>::ambiguous_function_object_case, dummy>* = nullptr>
    inline constexpr auto 
    fwrap(T&& t) {
        return function_object::ambiguous::template wrap<qflags::default_>(std::forward<T>(t));
    }

    /*********************
    std::reference_wrapper
    **********************/

    template<typename T, std::enable_if_t<detail::sfinae_switch<T>::reference_wrapper_case, dummy>* = nullptr>
    inline constexpr auto 
    fwrap(T&& t) {
        return   fwrap(std::addressof(t.get()));
    }

    /**************************************************
    std::reference_wrapper with member function pointer
    ***************************************************/

    template<typename T, typename TMemberFnPtr, std::enable_if_t<detail::sfinae_switch<TMemberFnPtr>::member_function_ptr_case && detail::sfinae_switch<T>::reference_wrapper_case, dummy>* = nullptr>
    inline constexpr auto 
    fwrap(T&& t, TMemberFnPtr ptr) {
        return   fwrap(std::addressof(t.get()), ptr);
    }

    /* todo - shorter creator aliases for use with fast_wrap
    template<typename Creator, typename... Args>
    inline constexpr auto
        fast_wrap(Args&&... args) {
        return Creator::template wrap<qflags::default_>(std::forward<Args>(args)...);
    }*/
}

#endif