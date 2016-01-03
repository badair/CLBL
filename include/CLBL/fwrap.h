/*!
@file
Defines all overloads of `clbl::fwrap`, as well as the CLBL_PMFWRAP macro.

@copyright Barrett Adair 2015
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)

*/

#ifndef CLBL_FWRAP_H
#define CLBL_FWRAP_H

#ifndef CLBL_EXCLUDE_FUNCTIONAL
#include <functional>
#endif

#include <type_traits>
#include <utility>
#include <CLBL/tags.h>
#include <CLBL/no_ref.h>
#include <CLBL/is_clbl.h>
#include <CLBL/is_valid.h>
#include <CLBL/qualify_flags.h>
#include <CLBL/can_dereference.h>
#include <CLBL/is_reference_wrapper.h>
#include <CLBL/member_function_decay.h>
#include <CLBL/has_normal_call_operator.h>

#include <CLBL/wrap/free_function.h>
#include <CLBL/wrap/function_object.h>
#include <CLBL/wrap/pointer_to_function_object.h>
#include <CLBL/wrap/member_function_with_object.h>
#include <CLBL/wrap/member_function_with_object_slim.h>
#include <CLBL/wrap/member_function_with_pointer_to_object.h>
#include <CLBL/wrap/member_function_with_pointer_to_object_slim.h>

namespace clbl {

    /*
    clbl::fwrap is a heavily overloaded function that can be used to create CLBL 
    wrappers for anything that is callable.
    */

    template<typename T, typename... Args>
    inline constexpr auto fwrap(T&& t, Args&&... args);

    template<typename T>
    inline constexpr auto fwrap(T&& t);
    
    namespace detail {

        template<typename T> 
        struct sfinae_switch {
        private:
            
            static constexpr auto can_deref = can_dereference<T>;
            using dereferenceable = std::conditional_t<can_deref, no_ref<T>, dummy*>;

            #ifndef CLBL_EXCLUDE_FUNCTIONAL
            static constexpr auto is_ref_wrapper = is_reference_wrapper<T>;
            #else
            static constexpr auto is_ref_wrapper = false;
            #endif

            static constexpr auto is_ambiguous = !has_normal_call_operator<T>;

            static constexpr auto ptr_is_ambiguous = !ptr_has_normal_call_operator<T>;

            static constexpr auto is_function_ptr = !std::is_function<no_ref<T> >::value 
                                                    && std::is_function<std::remove_pointer_t<T> >::value;

            static constexpr auto is_function_ref = std::is_function<no_ref<T> >::value
                                                    && !std::is_class<no_ref<T> >::value;
            
            static constexpr auto is_member_function_ptr = std::is_member_function_pointer<T>::value;
            
        public:
            static constexpr auto is_clbl = clbl::is_clbl<no_ref<T> > 
                                || clbl::is_clbl<no_ref<decltype(*std::declval<dereferenceable>())> >;

            static constexpr auto is_ptr = !is_clbl && can_dereference<no_ref<T>> 
                                            && !has_normal_call_operator<T>;

            static constexpr auto reference_wrapper_case = is_ref_wrapper;

            static constexpr auto function_ptr_case = !is_clbl && is_function_ptr;

            static constexpr auto function_ref_case = !is_clbl
                                                      && is_function_ref 
                                                      && !has_normal_call_operator<T>;

            static constexpr auto member_function_ptr_case = !is_clbl && !is_function_ptr 
                                                                && is_member_function_ptr;

        private:
            static constexpr auto is_complex_case = !is_clbl
                                                    && !reference_wrapper_case 
                                                    && !function_ptr_case 
                                                    && !function_ref_case
                                                    && !member_function_ptr_case
                                                    && std::is_class<std::remove_pointer<no_ref<T> > >::value;
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

        template<typename U>
        struct fwrap_t {

            /********************
            free function pointer
            *********************/

            #ifdef CLBL_DOCUMENTATION_BUILD
            //! @addtogroup ffwrapping Wrapping free functions
            //! clbl::fwrap lets you wrap free functions. See also @ref fwrap.h
            //! Example
            //! -------
            //! @include example/fwrap_free_function_pointer.cpp

            //! wrap a free function pointer.
            inline constexpr auto fwrap(free_function_pointer f);
            #else

            template<typename T = U, std::enable_if_t<
                detail::sfinae_switch<T>::function_ptr_case, dummy>* = nullptr>
            inline constexpr auto 
            operator()(T&& t) const {
                return free_function::template
                    wrap<default_>(std::forward<T>(t));
            }

            template<typename T = U, std::enable_if_t<
            detail::sfinae_switch<T>::function_ref_case, dummy>* = nullptr>
            inline constexpr auto 
            operator()(T&& t) const {
                return free_function_reference::template
                    wrap<default_>(std::forward<T>(t));
            }
            
            #endif


            /***********************************************
            Pointer to object with a member function pointer
            ************************************************/

            #ifdef CLBL_DOCUMENTATION_BUILD
            //! @addtogroup mfwrapping Wrapping member functions
            //! clbl::fwrap lets you wrap a pointer-to-member-function bound 
            //! to an object. The following example shows all the ways you can
            //! do this in CLBL. See also @ref fwrap.h
            //! Example
            //! -------
            //! @include example/fwrap_member_function_pointer.cpp

            //! Wrap a pointer-to-member-function, binding an object via raw 
            //! pointer, smart pointer, copy, rvalue reference, or ref-wrapper.
            inline constexpr auto fwrap(object o, pointer_to_member_function p);
            #else

            template<typename T = U, typename TMemberFnPtr, std::enable_if_t<
                detail::sfinae_switch<TMemberFnPtr>::member_function_ptr_case 
                && detail::sfinae_switch<T>::is_ptr, dummy>* = nullptr>
            inline constexpr auto 
            operator()(T&& t, TMemberFnPtr member_fn_ptr) const {
                return member_function_with_pointer_to_object::template
                    wrap<default_>(member_fn_ptr, std::forward<T>(t));
            }


            /**********************************
            Object with member function pointer
            ***********************************/

            template<typename T = U, typename TMemberFnPtr, std::enable_if_t<
                detail::sfinae_switch<TMemberFnPtr>::member_function_ptr_case 
                && !detail::sfinae_switch<T>::is_ptr 
                && !detail::sfinae_switch<T>::reference_wrapper_case, dummy>* = nullptr>
            inline constexpr auto 
            operator()(T&& t, TMemberFnPtr member_fn_ptr) const {
                return member_function_with_object::template
                    wrap<default_>(member_fn_ptr, std::forward<T>(t));
            }

            #endif


            /********************************
            Pointer to object with operator()
            *********************************/

            #ifdef CLBL_DOCUMENTATION_BUILD
            //! @addtogroup fowrapping Wrapping callable objects
            //! clbl::fwrap lets you wrap a callable object via raw pointer, 
            //! smart pointer, copy, rvalue reference, or ref-wrapper.
            //! See also @ref fwrap.h
            //! Example
            //! -------
            //! @include example/fwrap_function_object.cpp

            //! Wrap a callable object via raw pointer, smart pointer, copy, 
            //! rvalue reference, or ref-wrapper.
            inline constexpr auto fwrap(object o);
            #else

            template<typename T = U, std::enable_if_t<
                detail::sfinae_switch<T>::function_object_ptr_case, dummy>* = nullptr>
            inline constexpr auto 
            operator()(T&& t) const {
                return pointer_to_function_object::template
                    wrap<default_>(std::forward<T>(t));
            }

            /*****************************************************************
            Pointer to object with AMBIGUOUS (templated/overloaded) operator() 
            ******************************************************************/

            template<typename T = U, std::enable_if_t<
                detail::sfinae_switch<T>::ambiguous_function_object_ptr_case, dummy>* = nullptr>
            inline constexpr auto 
            operator()(T&& t) const {
                return pointer_to_function_object::ambiguous::template
                    wrap<default_>(std::forward<T>(t));
            }

            /**********************
            Object with operator()
            ***********************/

            template<typename T = U, std::enable_if_t<
                detail::sfinae_switch<T>::function_object_case, dummy>* = nullptr>
            inline constexpr auto 
            operator()(T&& t) const {
                return function_object::template
                    wrap<default_>(std::forward<T>(t));
            }

            /******************************************************
            Object with AMBIGUOUS (templated/overloaded) operator()
            *******************************************************/

            template<typename T = U, std::enable_if_t<
                detail::sfinae_switch<T>::ambiguous_function_object_case, dummy>* = nullptr>
            inline constexpr auto 
            operator()(T&& t) const {
                return function_object::ambiguous::template 
                    wrap<default_>(std::forward<T>(t));
            }

            #endif

            /*********************
            std::reference_wrapper
            **********************/

            #ifndef CLBL_EXCLUDE_FUNCTIONAL
            template<typename T = U, std::enable_if_t<
                detail::sfinae_switch<T>::reference_wrapper_case, dummy>* = nullptr>
            inline constexpr auto 
            operator()(T&& t) const {
                return fwrap(std::addressof(t.get()));
            }
            #endif

            /**************************************************
            std::reference_wrapper with member function pointer
            ***************************************************/

            template<typename T = U, typename TMemberFnPtr, std::enable_if_t<
                detail::sfinae_switch<TMemberFnPtr>::member_function_ptr_case
                && detail::sfinae_switch<T>::reference_wrapper_case, dummy>* = nullptr>
            inline constexpr auto 
            operator()(T&& t, TMemberFnPtr ptr) const {
                return   fwrap(std::addressof(t.get()), ptr);
            }

                /*********************************************
            preempting recursive attempts at CLBL wrappers
            **********************************************/

            template<typename T = U, std::enable_if_t<
                detail::sfinae_switch<T>::is_clbl
                && !can_dereference<T>, dummy>* = nullptr>
            inline constexpr auto
            operator()(T&& t) const {
                using callable = no_ref<T>;
                return callable::creator::template
                    wrap_data<callable::cv_flags | cv<callable> >(t.data);
            }

            template<typename T = U, std::enable_if_t<
                detail::sfinae_switch<T>::is_clbl
                && can_dereference<T>, dummy>* = nullptr>
            inline constexpr auto
            operator()(T&& t) const {
                using callable = no_ref<decltype(*t)>;
                return callable::creator::template
                    wrap_data<callable::cv_flags | cv<callable> >(t -> data);
            }
        };
        
        /*
        template<typename T, typename... Args>
        struct fwrap_t<T(Args...)> {
            
            template<typename ActualReturnType>
            inline constexpr auto
            operator()(ActualReturnType(&&function_reference)(Args...)) const {
                return fwrap<ActualReturnType(&&)(Args...)>(function_reference);
            }
        };

        template<typename T, typename... Args>
        struct fwrap_t<T(Args..., ...)> {
            template<typename ActualReturnType>
            inline constexpr auto
            operator()(ActualReturnType(&&function_reference)(Args..., ...)) const {
                return fwrap<ActualReturnType(&&)(Args..., ...)>(function_reference);
            }
        };*/
        
        template<typename T>
        constexpr fwrap_t<T> fwrap_v{};
    }

    template<typename T, typename... Args>
    inline constexpr auto fwrap(T&& t, Args&&... args) {
        return detail::fwrap_v<T>(std::forward<T>(t), std::forward<Args>(args)...);
    }

    template<typename T>
    inline constexpr auto fwrap(T&& t) {
        return detail::fwrap_v<T>(std::forward<T>(t));
    }
    
    template<typename T, typename Specialization>
    inline constexpr auto fwrap(T&& t) {
        return detail::fwrap_v<Specialization>(std::forward<T>(t));
    }

    /*************************************************************************************
    pmf accepts a PMF as a compile-time template argument (can't be a cast result, though)
    **************************************************************************************/

    template <typename TMemberFnPtr, TMemberFnPtr Pmf>
    struct pmf
    {
        template<typename T, std::enable_if_t<
            !detail::sfinae_switch<T>::is_ptr
            && !detail::sfinae_switch<T>::reference_wrapper_case, dummy>* = nullptr>
        static inline constexpr auto 
        fwrap(T&& t) {
            return member_function_with_object_slim::template
                wrap<default_, TMemberFnPtr, Pmf>(std::forward<T>(t));
        }

        template<typename TPtr, std::enable_if_t<
            detail::sfinae_switch<TPtr>::is_ptr
            && !detail::sfinae_switch<TPtr>::reference_wrapper_case, dummy>* = nullptr>
        static inline constexpr auto
        fwrap(TPtr&& object_ptr) {
            return member_function_with_pointer_to_object_slim::template
                wrap<default_, TMemberFnPtr, Pmf>(std::forward<TPtr>(object_ptr));
        }

        template<typename T, std::enable_if_t<
            detail::sfinae_switch<T>::reference_wrapper_case, dummy>* = nullptr>
        static inline constexpr auto
        fwrap(T&& t) {
            return member_function_with_pointer_to_object_slim::template
                wrap<default_, TMemberFnPtr, Pmf>(&t.get());
        }
    };

    #ifdef CLBL_DOCUMENTATION_BUILD
    //! @def CLBL_PMFWRAP
    //! CLBL_PMFWRAP Wraps a non-overloaded member function pointer at compile-time, 
    //! binding an object at the runtime call site via copy/pointer/ref-wrapper/rvalue.
    //! This removes the object bloat from calling clbl::fwrap with a runtime PMF. Note:
    //! the pointer-to-member-function cannot be the result of a cast, per the C++ standard.
    //! If you need to wrap a pointer-to-member-function cast, use clbl::fwrap instead 
    //! of this macro. Expands to `pmf<decltype(my_pmf), my_pmf>::fwrap(my_object)`
    //! @include example/clbl_pmfwrap.cpp
    #define CLBL_PMFWRAP(object, pointer_to_member_function)
    #else

    #define CLBL_PMFWRAP(o, pmf_expr) \
    clbl::pmf<decltype(pmf_expr), pmf_expr>::fwrap(o)

    #endif

    //todo size tests, reference_wrapper tests, CLBL_PMFWRAP tests
}

#endif