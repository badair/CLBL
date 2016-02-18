/*!
@file
Defines all overloads of `clbl::fwrap`, as well as the CLBL_PMFWRAP macro.

@copyright Barrett Adair 2015
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)

*/

#ifndef CLBL_FWRAP_H
#define CLBL_FWRAP_H

#include <functional>
#include <type_traits>
#include <utility>
#include <CLBL/tags.hpp>
#include <CLBL/constraints.hpp>
#include <CLBL/type_traits.hpp>
#include <CLBL/is_clbl.hpp>
#include <CLBL/qflags.hpp>
#include <CLBL/can_dereference.hpp>
#include <CLBL/is_reference_wrapper.hpp>
#include <CLBL/has_normal_call_operator.hpp>
#include <CLBL/factory/function_ptr_wrapper_factory.hpp>
#include <CLBL/factory/function_reference_wrapper_factory.hpp>
#include <CLBL/factory/function_object_wrapper_factory.hpp>
#include <CLBL/factory/function_object_ptr_wrapper_factory.hpp>
#include <CLBL/factory/member_function_wrapper_factory.hpp>
#include <CLBL/factory/member_function_bound_to_object_wrapper_factory.hpp>
#include <CLBL/factory/member_function_bound_to_object_ptr_wrapper_factory.hpp>

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
    
        template<qualify_flags Flags>
        using flags_t = std::integral_constant<qualify_flags, Flags>;
    
        template<typename T> 
        struct switch_ {
            
        private:
            
            static constexpr auto can_deref = can_dereference<T>::value;

            using dereferenceable = std::conditional_t<can_deref, no_ref<T>, dummy*>;

            static constexpr auto is_ref_wrapper = is_reference_wrapper<T>::value;

            static constexpr auto is_ambiguous = !has_normal_call_operator<T>::value;

            static constexpr auto ptr_is_ambiguous = !ptr_has_normal_call_operator<T>::value;

            static constexpr auto is_function_ptr =
                !std::is_function<no_ref<T> >::value 
                && std::is_function<std::remove_pointer_t<T> >::value;

            /*
            function pointer conversions are handled as a "complex case".
            CLBL doesn't currently handle types that define a function
            pointer conversion operator without also defining an operator(),
            because that would be silly
            */
            static constexpr auto is_function_ref =
                std::is_function<no_ref<T> >::value

                && !std::is_class<no_ref<T> >::value;
            
            static constexpr auto is_member_function_ptr =
                std::is_member_function_pointer<T>::value;
            
        public:

            static constexpr auto is_clbl =
                clbl::is_clbl<no_ref<T> >::value 
                || clbl::is_clbl<no_ref<decltype(*std::declval<dereferenceable>())> >::value;

            static constexpr auto is_ptr =
                !is_clbl && can_dereference<no_ref<T> >::value
                && !has_normal_call_operator<T>::value;

            static constexpr auto reference_wrapper_case = is_ref_wrapper;

            static constexpr auto function_ptr_case =
                !is_clbl
                && is_function_ptr;

            static constexpr auto function_ref_case =
                !is_clbl
                && is_function_ref 
                && !has_normal_call_operator<T>::value;

            static constexpr auto pmf_case = 
                !is_clbl 
                && !is_function_ptr 
                && is_member_function_ptr;

        private:

            static constexpr auto is_complex_case =
                !is_clbl
                && !reference_wrapper_case 
                && !function_ptr_case 
                && !function_ref_case
                && !pmf_case
                && std::is_class<std::remove_pointer<no_ref<T> > >::value;

        public:

            static constexpr auto function_object_case =
                is_complex_case
                && !is_ptr
                && !is_ambiguous;

            static constexpr auto ambiguous_function_object_case = 
                is_complex_case
                && !is_ptr
                && is_ambiguous;

            static constexpr auto function_object_ptr_case =
                is_complex_case 
                && is_ptr 
                && !ptr_is_ambiguous;

            static constexpr auto ambiguous_function_object_ptr_case =
                is_complex_case 
                && is_ptr 
                && ptr_is_ambiguous;
        };

        template<typename U>
        struct fwrap_t {

            /********************
            free function pointer
            *********************/

            #ifdef CLBL_DOCUMENTATION_BUILD
            //! @addtogroup ffwrapping Wrapping free functions
            //! clbl::fwrap lets you wrap free functions. See also @ref fwrap.hpp
            //! Example
            //! -------
            //! @include example/fwrap_free_function_pointer.cpp

            //! wrap a free function pointer.
            inline constexpr auto fwrap(free_function_pointer f);
            #else

            template<
                typename T,
                CLBL_REQUIRES_(detail::switch_<T>::function_ptr_case)
            >
            inline constexpr auto 
            operator()(T&& t) const {
                return function_ptr_wrapper_factory::template
                    wrap(static_cast<T&&>(t));
            }

            template<
                typename T, 
                std::enable_if_t<detail::switch_<T>::function_ptr_case, T> Value>
            inline constexpr auto 
            slim() const {
                return function_ptr_wrapper_factory::slim::template
                    wrap<T, Value>();
            }

            template<
                typename T,
                std::enable_if_t<detail::switch_<T>::function_ref_case, T> Value>
            inline constexpr auto 
            slim() const {
                return function_reference_wrapper_factory::slim::template
                    wrap<T, Value>();
            }

            template<
                typename T,
                std::enable_if_t<detail::switch_<T>::pmf_case, T> Value>
            inline constexpr auto 
            slim() const {
                return member_function_wrapper_factory::slim::template
                    wrap<T, Value>();
            }

            template<
                typename Pmf,
                Pmf Value,
                typename T,
                CLBL_REQUIRES_(
                    !detail::switch_<T>::is_ptr
                    && !detail::switch_<T>::reference_wrapper_case
                )
            >
            static inline constexpr auto 
            slim_bound(T&& t) {
                return member_function_bound_to_object_wrapper_factory::slim::template
                    wrap<qflags::default_, Pmf, Value>(static_cast<T&&>(t));
            }

            template<
                typename Pmf,
                Pmf Value,
                typename TPtr,
                CLBL_REQUIRES_(
                    detail::switch_<TPtr>::is_ptr
                    && !detail::switch_<TPtr>::reference_wrapper_case
                )
            >
            static inline constexpr auto
            slim_bound(TPtr&& object_ptr) {
                return member_function_bound_to_object_ptr_wrapper_factory::slim::template
                    wrap<qflags::default_, Pmf, Value>(static_cast<TPtr&&>(object_ptr));
            }

            template<
                typename Pmf,
                Pmf Value,
                typename T,
                CLBL_REQUIRES_(detail::switch_<T>::reference_wrapper_case)
            >
            static inline constexpr auto
            slim_bound(T&& t) {
                return member_function_bound_to_object_ptr_wrapper_factory::slim::template
                    wrap<qflags::default_, Pmf, Value>(&t.get());
            }

            template<
                typename T,
                CLBL_REQUIRES_(detail::switch_<T>::function_ref_case)
            >
            inline constexpr auto 
            operator()(T&& t) const {
                return function_reference_wrapper_factory::template
                    wrap(static_cast<T&&>(t));
            }

            #endif
            
            /***********************************************
            Pointer to object with a member function pointer
            ************************************************/

            #ifdef CLBL_DOCUMENTATION_BUILD
            //! @addtogroup mfwrapping Wrapping member functions
            //! clbl::fwrap lets you wrap a pointer-to-member-function bound 
            //! to an object. The following example shows all the ways you can
            //! do this in CLBL. See also @ref fwrap.hpp
            //! Example
            //! -------
            //! @include example/fwrap_member_function_pointer.cpp

            //! Wrap a pointer-to-member-function, binding an object via raw 
            //! pointer, smart pointer, copy, rvalue reference, or ref-wrapper.
            inline constexpr auto fwrap(object o, pointer_to_member_function p);
            #else

            template<
                typename T,
                typename Pmf,
                CLBL_REQUIRES_(
                    detail::switch_<Pmf>::pmf_case 
                    && detail::switch_<T>::is_ptr
                )
            >
            inline constexpr auto 
            operator()(T&& t, Pmf p) const {
                return member_function_bound_to_object_ptr_wrapper_factory::template
                    wrap<qflags::default_>(p, static_cast<T&&>(t));
            }

            /**********************************
            Object with member function pointer
            ***********************************/

            template<
                typename T,
                typename Pmf,
                CLBL_REQUIRES_(
                    detail::switch_<Pmf>::pmf_case 
                    && !detail::switch_<T>::is_ptr 
                    && !detail::switch_<T>::reference_wrapper_case
                )
            >
            inline constexpr auto 
            operator()(T&& t, Pmf p) const {
                return member_function_bound_to_object_wrapper_factory::template
                    wrap<qflags::default_>(p, static_cast<T&&>(t));
            }

            #endif

            /***********************
            Member function pointer
            ************************/
            
            template<
                typename Pmf,
                CLBL_REQUIRES_(
                    detail::switch_<no_ref<Pmf>>::pmf_case
                )
            >
            inline constexpr auto 
            operator()(Pmf&& p) const {
                return member_function_wrapper_factory::template
                    wrap<qflags::default_, std::remove_cv_t<no_ref<Pmf> > >(
                        static_cast<Pmf&&>(p)
                    );
            }
            
            /********************************
            Pointer to object with operator()
            *********************************/

            #ifdef CLBL_DOCUMENTATION_BUILD
            //! @addtogroup fowrapping Wrapping callable objects
            //! clbl::fwrap lets you wrap a callable object via raw pointer, 
            //! smart pointer, copy, rvalue reference, or ref-wrapper.
            //! See also @ref fwrap.hpp
            //! Example
            //! -------
            //! @include example/fwrap_function_object.cpp

            //! Wrap a callable object via raw pointer, smart pointer, copy, 
            //! rvalue reference, or ref-wrapper.
            inline constexpr auto fwrap(object o);
            #else

            template<
                typename T,
                CLBL_REQUIRES_(detail::switch_<T>::function_object_ptr_case)
            >
            inline constexpr auto 
            operator()(T&& t) const {
                return function_object_ptr_wrapper_factory::template
                    wrap<qflags::default_>(static_cast<T&&>(t));
            }

            /*****************************************************************
            Pointer to object with AMBIGUOUS (templated/overloaded) operator() 
            ******************************************************************/

            template<
                typename T,
                CLBL_REQUIRES_(detail::switch_<T>::ambiguous_function_object_ptr_case)
            >
            inline constexpr auto 
            operator()(T&& t) const {
                return function_object_ptr_wrapper_factory::ambiguous::template
                    wrap<qflags::default_>(static_cast<T&&>(t));
            }

            /**********************
            Object with operator()
            ***********************/

            template<
                typename T,
                CLBL_REQUIRES_(detail::switch_<T>::function_object_case)
            >
            inline constexpr decltype(auto)
            operator()(T&& t) const {
                return function_object_wrapper_factory::template
                    wrap<qflags::default_>(static_cast<T&&>(t));
            }

            /******************************************************
            Object with AMBIGUOUS (templated/overloaded) operator()
            *******************************************************/

            template<
                typename T,
                CLBL_REQUIRES_(detail::switch_<T>::ambiguous_function_object_case)
            >
            inline constexpr auto 
            operator()(T&& t) const {
                return function_object_wrapper_factory::ambiguous::template 
                    wrap<qflags::default_>(static_cast<T&&>(t));
            }

            #endif

            /*********************
            std::reference_wrapper
            **********************/

            template<
                typename T,
                CLBL_REQUIRES_(detail::switch_<T>::reference_wrapper_case)
            >
            inline constexpr auto 
            operator()(T&& t) const {
                return fwrap(std::addressof(t.get()));
            }

            /**************************************************
            std::reference_wrapper with member function pointer
            ***************************************************/

            template<
                typename T,
                typename Pmf,
                CLBL_REQUIRES_(
                    detail::switch_<Pmf>::pmf_case
                    && detail::switch_<T>::reference_wrapper_case
                )
            >
            inline constexpr auto 
            operator()(T&& t, Pmf ptr) const {
                return fwrap(std::addressof(t.get()), ptr);
            }

            /*********************************************
            preempting recursive attempts at CLBL wrappers
            **********************************************/

            template<
                typename T,
                CLBL_REQUIRES_(
                    detail::switch_<T>::is_clbl
                    && !can_dereference<T>::value
                )
            >
            inline constexpr auto
            operator()(T&& t) const {
                using callable = no_ref<T>;
                return callable::creator::template
                    wrap_data<callable::q_flags | cv_of<callable>::value >(t.data);
            }

            template<
                typename T,
                CLBL_REQUIRES_(
                    detail::switch_<T>::is_clbl
                    && can_dereference<T>::value
                )
            >
            inline constexpr auto
            operator()(T&& t) const {
                using callable = no_ref<decltype(*t)>;
                return callable::creator::template
                    wrap_data<callable::q_flags | cv_of<callable>::value >((*t).data);
            }
        };
        
        template<qualify_flags Flags>
        struct fwrap_t<detail::flags_t<Flags> > {

            template<
                typename Pmf,
                CLBL_REQUIRES_(detail::switch_<no_ref<Pmf> >::pmf_case)
            >
            inline constexpr auto 
            operator()(Pmf&& p) const {
                return member_function_wrapper_factory::template
                    wrap<
                        qflags::guarantee_reference<Flags>::value,
                        std::remove_cv_t<no_ref<Pmf> > 
                    >(
                        static_cast<Pmf&&>(p)
                    );
            }
        };
        
        template<>
        struct fwrap_t<detail::flags_t<qflags::default_> > {

            template<
                typename Pmf,
                CLBL_REQUIRES_(detail::switch_<no_ref<Pmf> >::pmf_case)
            >
            inline constexpr auto 
            operator()(Pmf&& p) const {
                return member_function_wrapper_factory::template
                    wrap<std::remove_cv_t<no_ref<Pmf> > >(
                        static_cast<Pmf&&>(p)
                    );
            }
        };
        
        template<>
        struct fwrap_t<copy_> {

            template<
                typename Pmf,
                CLBL_REQUIRES_(detail::switch_<no_ref<Pmf> >::pmf_case)
            >
            inline constexpr auto 
            operator()(Pmf&& p) const {
                return member_function_wrapper_factory::template
                    wrap<qflags::default_, std::remove_cv_t<no_ref<Pmf> > >(
                        static_cast<Pmf&&>(p)
                    );
            }
        };
    }

    template<typename T, typename... Args>
    inline constexpr auto fwrap(T&& t, Args&&... args) {
        return detail::fwrap_t<T>{}(
                    static_cast<T&&>(t),
                    static_cast<Args&&>(args)...
                );
    }

    template<typename T>
    inline constexpr auto fwrap(T&& t) {
        return detail::fwrap_t<T>{}(static_cast<T&&>(t));
    }
    
    template<qualify_flags flags, typename T>
    inline constexpr auto fwrap(T&& t) {
        return detail::fwrap_t<detail::flags_t<flags> >{}(static_cast<T&&>(t));
    }
    
    template<typename Specialization, typename T>
    inline constexpr auto fwrap(T&& t) {
        return detail::fwrap_t<Specialization>{}(static_cast<T&&>(t));
    }

    template<typename T, T Value>
    inline constexpr auto fwrap() {
        return detail::fwrap_t<T>{}.template slim<T, Value>();
    }

    template<typename T, T Value, typename Obj>
    inline constexpr auto fwrap(Obj&& obj) {
        return detail::fwrap_t<T>{}.template
            slim_bound<T, Value>(static_cast<Obj&&>(obj));
    }
}

#endif