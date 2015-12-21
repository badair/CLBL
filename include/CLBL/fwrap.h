#ifndef CLBL_FWRAP_H
#define CLBL_FWRAP_H

#include <type_traits>
#include <functional>

#include <CLBL/wrappers/free_fn_wrapper.h>
#include <CLBL/wrappers/pmf_wrapper.h>
#include <CLBL/wrappers/pmf_ptr_wrapper.h>
#include <CLBL/wrappers/ambi_fn_obj_wrapper.h>
#include <CLBL/wrappers/ambi_fn_obj_ptr_wrapper.h>
#include <CLBL/wrap/free_function.h>
#include <CLBL/wrap/function_object.h>
#include <CLBL/wrap/pointer_to_function_object.h>
#include <CLBL/wrap/member_function_with_object.h>
#include <CLBL/wrap/member_function_with_object_slim.h>
#include <CLBL/wrap/member_function_with_pointer_to_object.h>
#include <CLBL/wrap/member_function_with_pointer_to_object_slim.h>
#include <CLBL/member_function_decay.h>
#include <CLBL/tags.h>
#include <CLBL/qualify_flags.h>
#include <CLBL/utility.h>
#include <CLBL/is_valid.h>

namespace clbl {

    /*
    clbl::fwrap is a heavily overloaded function that can be used to create CLBL 
    wrappers for anything that is callable.
    */

    namespace detail {

        auto has_normal_call_operator_impl = is_valid(
            [](auto&& arg)->decltype(&no_ref<decltype(arg)>::operator()) {}
        );

        auto ptr_has_normal_call_operator_impl = is_valid(
            [](auto&& arg)->decltype(&no_ref<decltype(*arg)>::operator()) {}
        );

        template<typename T>
        constexpr bool has_normal_call_operator = 
            decltype(has_normal_call_operator_impl(std::declval<T>()))::value;

        template<typename T>
        constexpr bool ptr_has_normal_call_operator =
            decltype(ptr_has_normal_call_operator_impl(std::declval<T>()))::value;

        template<typename T> 
        struct sfinae_switch {
        private:
            
            static constexpr auto can_deref = can_dereference<T>;
            using dereferenceable = std::conditional_t<can_deref, no_ref<T>, dummy*>;

            static constexpr auto is_ref_wrapper = is_reference_wrapper<T>;
            static constexpr auto is_ambiguous = !has_normal_call_operator<T>;
            static constexpr auto ptr_is_ambiguous = !ptr_has_normal_call_operator<T>;
            static constexpr auto is_function_ptr = std::is_function<std::remove_pointer_t<T> >::value;
            static constexpr auto is_member_function_ptr = std::is_member_function_pointer<T>::value;
            
        public:
            static constexpr auto is_clbl = clbl::is_clbl<no_ref<T> > 
                                || clbl::is_clbl<no_ref<decltype(*std::declval<dereferenceable>())> >;

            static constexpr auto is_ptr = !is_clbl && can_dereference<no_ref<T>> 
                                            && !has_normal_call_operator<T>;

            static constexpr auto reference_wrapper_case = is_ref_wrapper;
            static constexpr auto function_ptr_case = !is_clbl && is_function_ptr;
            static constexpr auto member_function_ptr_case = !is_clbl && !is_function_ptr 
                                                                && is_member_function_ptr;

        private:
            static constexpr auto is_complex_case = !is_clbl
                                                    && !reference_wrapper_case 
                                                    && !function_ptr_case 
                                                    && !member_function_ptr_case;
        public:
            static constexpr auto function_object_case = is_complex_case 
                                                        && !is_ptr 
                                                        && !is_ambiguous;

            static constexpr auto ambiguous_function_object_case = is_complex_case 
                                                                && !is_ptr 
                                                                && is_ambiguous;

            static constexpr auto function_object_ptr_case = is_complex_case 
                                                                && is_ptr 
                                                                && !ptr_is_ambiguous;

            static constexpr auto ambiguous_function_object_ptr_case = is_complex_case 
                                                                        && is_ptr 
                                                                        &&  ptr_is_ambiguous;
        };
    }

    /********************
    free function pointer
    *********************/

    template<typename T, std::enable_if_t<
        detail::sfinae_switch<T>::function_ptr_case, dummy>* = nullptr>
    inline constexpr auto 
    fwrap(T&& t) {
        return free_function::template
            wrap<qflags::default_>(std::forward<T>(t));
    }

    /***********************************************
    Pointer to object with a member function pointer
    ************************************************/

    template<typename T, typename TMemberFnPtr, std::enable_if_t<
        detail::sfinae_switch<TMemberFnPtr>::member_function_ptr_case 
        && detail::sfinae_switch<T>::is_ptr, dummy>* = nullptr>
    inline constexpr auto 
    fwrap(T&& t, TMemberFnPtr member_fn_ptr) {
        return member_function_with_pointer_to_object::template
            wrap<qflags::default_>(member_fn_ptr, std::forward<T>(t));
    }

    /**********************************
    Object with member function pointer
    ***********************************/

    template<typename T, typename TMemberFnPtr, std::enable_if_t<
        detail::sfinae_switch<TMemberFnPtr>::member_function_ptr_case 
        && !detail::sfinae_switch<T>::is_ptr 
        && !detail::sfinae_switch<T>::reference_wrapper_case, dummy>* = nullptr>
    inline constexpr auto 
    fwrap(T&& t, TMemberFnPtr member_fn_ptr) {
        return member_function_with_object::template
            wrap<qflags::default_>(member_fn_ptr, std::forward<T>(t));
    }

    /********************************
    Pointer to object with operator()
    *********************************/

    template<typename T, std::enable_if_t<
        detail::sfinae_switch<T>::function_object_ptr_case, dummy>* = nullptr>
    inline constexpr auto 
    fwrap(T&& t) {
        return pointer_to_function_object::template
            wrap<qflags::default_>(std::forward<T>(t));
    }

    /*****************************************************************
    Pointer to object with AMBIGUOUS (templated/overloaded) operator() 
    ******************************************************************/

    template<typename T, std::enable_if_t<
        detail::sfinae_switch<T>::ambiguous_function_object_ptr_case, dummy>* = nullptr>
    inline constexpr auto 
    fwrap(T&& t) {
        return pointer_to_function_object::ambiguous::template
            wrap<qflags::default_>(std::forward<T>(t));
    }

    /**********************
    Object with operator()
    ***********************/

    template<typename T, std::enable_if_t<
        detail::sfinae_switch<T>::function_object_case, dummy>* = nullptr>
    inline constexpr auto 
    fwrap(T&& t) {
        return function_object::template
            wrap<qflags::default_>(std::forward<T>(t));
    }

    /******************************************************
    Object with AMBIGUOUS (templated/overloaded) operator()
    *******************************************************/

    template<typename T, std::enable_if_t<
        detail::sfinae_switch<T>::ambiguous_function_object_case, dummy>* = nullptr>
    inline constexpr auto 
    fwrap(T&& t) {
        return function_object::ambiguous::template 
            wrap<qflags::default_>(std::forward<T>(t));
    }

    /*********************
    std::reference_wrapper
    **********************/

    template<typename T, std::enable_if_t<
        detail::sfinae_switch<T>::reference_wrapper_case, dummy>* = nullptr>
    inline constexpr auto 
    fwrap(T&& t) {
        return fwrap(std::addressof(t.get()));
    }

    /**************************************************
    std::reference_wrapper with member function pointer
    ***************************************************/

    template<typename T, typename TMemberFnPtr, std::enable_if_t<
        detail::sfinae_switch<TMemberFnPtr>::member_function_ptr_case
        && detail::sfinae_switch<T>::reference_wrapper_case, dummy>* = nullptr>
    inline constexpr auto 
    fwrap(T&& t, TMemberFnPtr ptr) {
        return   fwrap(std::addressof(t.get()), ptr);
    }

    /*************************************************************************************
    pmf accepts a PMF as a compile-time template argument (can't be a cast result, though)
    **************************************************************************************/

    template <typename TMemberFnPtr, TMemberFnPtr Pmf>
    struct pmf
    {
        template<typename T, std::enable_if_t<
            !detail::sfinae_switch<T>::is_ptr
            && !detail::sfinae_switch<T>::is_ref_wrapper, dummy>* = nullptr>
        static constexpr auto 
        fwrap(T&& t) {
            return member_function_with_object_slim::template
                wrap<qflags::default_, TMemberFnPtr, Pmf>(std::forward<T>(t));
        }

        template<typename TPtr, std::enable_if_t<
            detail::sfinae_switch<TPtr>::is_ptr
            && !detail::sfinae_switch<TPtr>::is_ref_wrapper, dummy>* = nullptr>
        static constexpr auto
        fwrap(TPtr&& object_ptr) {
            return member_function_with_pointer_to_object_slim::template
                wrap<qflags::default_, TMemberFnPtr, Pmf>(std::forward<TPtr>(object_ptr));
        }

        template<typename T, std::enable_if_t<
            detail::sfinae_switch<T>::reference_wrapper_case, dummy>* = nullptr>
        inline constexpr auto
        fwrap(T&& t) {
            return fwrap(std::addressof(t.get()));
        }
    };

#define CLBL_PMFWRAP(pmf_expr, o) \
(clbl::pmf<clbl::no_ref<decltype(pmf_expr)>, pmf_expr>::fwrap(o))

    //todo size tests, reference_wrapper tests, CLBL_PMFWRAP tests

    /*********************************************
    preempting recursive attempts at CLBL wrappers
    **********************************************/

    template<typename T, std::enable_if_t<
        detail::sfinae_switch<T>::is_clbl
        && !can_dereference<T>, dummy>* = nullptr>
    inline constexpr auto
    fwrap(T&& t) {
        using callable = no_ref<T>;
        return callable::creator::template
            wrap_data<callable::cv_flags | cv<callable> >(t.data);
    }

    template<typename T, std::enable_if_t<
        detail::sfinae_switch<T>::is_clbl
        && can_dereference<T>, dummy>* = nullptr>
    inline constexpr auto
        fwrap(T&& t) {
        using callable = no_ref<decltype(*t)>;
        return callable::creator::template
            wrap_data<callable::cv_flags | cv<callable> >(t -> data);
    }
}

#endif