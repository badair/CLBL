/*!
@file
Defines `clbl::free_fn_wrapper`.

@copyright Barrett Adair 2015
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)

*/

#ifndef CLBL_FREE_FN_WRAPPER_H
#define CLBL_FREE_FN_WRAPPER_H

#include <tuple>
#include <utility>

#include <CLBL/tags.h>
#include <CLBL/no_ref.h>
#include <CLBL/cv.h>
#include <CLBL/qualify_flags.h>
#include <CLBL/forward.h>
#include <CLBL/harden_cast.h>
#include <CLBL/invocation_macros.h>
#include <CLBL/invocation_data.h>

namespace clbl {

    //! Wraps a free function pointer
    template<typename, typename Failure>
    struct free_fn_wrapper { static_assert(sizeof(Failure) < 0, "Not a function."); };

    template<typename Creator, typename Return, typename... Args>
    struct free_fn_wrapper<Creator, Return(Args...)> {

        using arg_types = std::tuple<Args...>;
        using clbl_tag = free_fn_tag;
        using creator = Creator;
        using forwarding_glue = Return(forward<Args>...);
        using invocation_data_type = ptr_invocation_data<Return(*)(Args...)>;
        using my_type = free_fn_wrapper<Creator, Return(Args...)>;
        using return_type = Return;
        using type = Return(Args...);
        using underlying_type = my_type;
        
        template<qualify_flags>
        using apply_cv = my_type;

        static constexpr auto cv_flags = default_;
        static constexpr auto is_ambiguous = false;

        invocation_data_type data;

        free_fn_wrapper(Return(*f_ptr)(Args...))
            : data{ f_ptr }
        {}

        template<typename... Fargs>
        inline Return operator()(Fargs&&... a) { 
            return (*data.ptr)(std::forward<Fargs>(a)...);
        }

        template<typename... Fargs>
        inline Return operator()(Fargs&&... a) const {
            return CLBL_CALL_PTR(const, data.ptr, std::forward<Fargs>(a)...);
        }

        template<typename... Fargs>
        inline Return operator()(Fargs&&... a) volatile {
            return CLBL_CALL_PTR(volatile, data.ptr, std::forward<Fargs>(a)...);
        }

        template<typename... Fargs>
        inline Return operator()(Fargs&&... a) const volatile {
            return CLBL_CALL_PTR(const volatile, data.ptr, std::forward<Fargs>(a)...);
        }

        static inline constexpr auto copy_invocation(my_type& c) {
            return[v = c.data.ptr](auto&&... args){
                return (*v)(args...);
            };
        }

        static inline constexpr auto copy_invocation(const my_type& c) {
            return[v = c.data.ptr](auto&&... args){
                return CLBL_CALL_PTR(const, v, args...);
            };
        }

        static inline constexpr auto copy_invocation(volatile my_type& c) {
            return[v = c.data.ptr](auto&&... args){
                return CLBL_CALL_PTR(volatile, v, args...);
            };
        }

        static inline constexpr auto copy_invocation(const volatile my_type& c) {
            return[v = c.data.ptr](auto&&... args){
                return CLBL_CALL_PTR(const volatile, v, args...);
            };
        }
    };
    
        //! Wraps a free function reference
    template<typename, typename Failure>
    struct free_fn_ref_wrapper { static_assert(sizeof(Failure) < 0, "Not a function."); };

    template<typename Creator, typename Return, typename... Args>
    struct free_fn_ref_wrapper<Creator, Return(Args...)> {

        using arg_types = std::tuple<Args...>;
        using clbl_tag = free_fn_ref_tag;
        using creator = Creator;
        using forwarding_glue = Return(forward<Args>...);
        using invocation_data_type = reference_invocation_data<Return(&)(Args...)>;
        using my_type = free_fn_ref_wrapper<Creator, Return(Args...)>;
        using return_type = Return;
        using type = Return(Args...);
        using underlying_type = my_type;
        
        template<qualify_flags>
        using apply_cv = my_type;

        static constexpr auto cv_flags = default_;
        static constexpr auto is_ambiguous = false;

        invocation_data_type data;

        free_fn_ref_wrapper(Return(&f)(Args...))
            : data{ f }
        {}

        template<typename... Fargs>
        inline Return operator()(Fargs&&... a) { 
            return (data.ref)(std::forward<Fargs>(a)...);
        }

        template<typename... Fargs>
        inline Return operator()(Fargs&&... a) const {
            return CLBL_CALL_VAL(const, data.ref, std::forward<Fargs>(a)...);
        }

        template<typename... Fargs>
        inline Return operator()(Fargs&&... a) volatile {
            return CLBL_CALL_VAL(volatile, data.ref, std::forward<Fargs>(a)...);
        }

        template<typename... Fargs>
        inline Return operator()(Fargs&&... a) const volatile {
            return CLBL_CALL_VAL(const volatile, data.ref, std::forward<Fargs>(a)...);
        }

        static inline constexpr auto copy_invocation(my_type& c) {
            return[&v = c.data.ref](auto&&... args){
                return v(args...);
            };
        }

        static inline constexpr auto copy_invocation(const my_type& c) {
            return[&v = c.data.ref](auto&&... args){
                return CLBL_CALL_VAL(const, v, args...);
            };
        }

        static inline constexpr auto copy_invocation(volatile my_type& c) {
            return[&v = c.data.ref](auto&&... args){
                return CLBL_CALL_VAL(volatile, v, args...);
            };
        }

        static inline constexpr auto copy_invocation(const volatile my_type& c) {
            return[&v = c.data.ref](auto&&... args){
                return CLBL_CALL_VAL(const volatile, v, args...);
            };
        }
    };
}

#endif