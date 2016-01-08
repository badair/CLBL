/*!
@file
Defines `clbl::universal_member_function`.

@copyright Barrett Adair 2015
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)

*/

#ifndef CLBL_UNIVERSAL_MEMBER_FUNCTION_H
#define CLBL_UNIVERSAL_MEMBER_FUNCTION_H

#include <utility>

#include <CLBL/tags.h>
#include <CLBL/no_ref.h>
#include <CLBL/cv.h>
#include <CLBL/qualify_flags.h>
#include <CLBL/wrap/member_function.h>
#include <CLBL/wrappers/unbound_universal_pmf_wrapper.h>

namespace clbl {
    
    namespace universal_member_function_detail {

        template<typename Creator, typename Bad>
        struct wrap_t {
            static_assert(sizeof(Bad) < 0, "Not a member function.");
        };
        
        #define __CLBL_SPECIALIZE_UNIVERSAL_MEMBER_FUNCTION_WRAP(qualifiers) \
        template<typename Creator, typename Return, typename T , typename... Args> \
        struct wrap_t<Creator, Return(T::*)(Args...) qualifiers> { \
            using member_fn_ptr = Return(T::*)(Args...) qualifiers; \
            inline constexpr auto \
            operator()(const member_fn_ptr& ptr) const { \
                using wrapper = unbound_universal_pmf_wrapper< \
                Creator, member_fn_ptr, Return(T::*)(Args...)>; \
                return wrapper{ ptr }; \
            } \
        }

        __CLBL_SPECIALIZE_UNIVERSAL_MEMBER_FUNCTION_WRAP(__CLBL_NO_CV);
        __CLBL_SPECIALIZE_UNIVERSAL_MEMBER_FUNCTION_WRAP(&);
        __CLBL_SPECIALIZE_UNIVERSAL_MEMBER_FUNCTION_WRAP(&&);
        __CLBL_SPECIALIZE_UNIVERSAL_MEMBER_FUNCTION_WRAP(const);
        __CLBL_SPECIALIZE_UNIVERSAL_MEMBER_FUNCTION_WRAP(volatile);
        __CLBL_SPECIALIZE_UNIVERSAL_MEMBER_FUNCTION_WRAP(const volatile);
        __CLBL_SPECIALIZE_UNIVERSAL_MEMBER_FUNCTION_WRAP(const &);
        __CLBL_SPECIALIZE_UNIVERSAL_MEMBER_FUNCTION_WRAP(volatile &);
        __CLBL_SPECIALIZE_UNIVERSAL_MEMBER_FUNCTION_WRAP(const volatile &);
        __CLBL_SPECIALIZE_UNIVERSAL_MEMBER_FUNCTION_WRAP(const &&);
        __CLBL_SPECIALIZE_UNIVERSAL_MEMBER_FUNCTION_WRAP(volatile &&);
        __CLBL_SPECIALIZE_UNIVERSAL_MEMBER_FUNCTION_WRAP(const volatile &&);

        template<typename Creator, typename MemberFnPtr>
        constexpr wrap_t<Creator, no_ref<MemberFnPtr> > wrap {};
    }

    struct universal_member_function {

        template<typename MemberFnPtr>
        static inline constexpr auto
        wrap(MemberFnPtr&& ptr){
            //using member_function as the creator - member_function takes CV flags for hardening
            return universal_member_function_detail::wrap<member_function, MemberFnPtr>(std::forward<MemberFnPtr>(ptr));
        }

        template<typename Invocation>
        static inline constexpr auto
            wrap_data(Invocation&& data) {
            return wrap<no_ref<Invocation> >(data);
        }
    };
}

#endif