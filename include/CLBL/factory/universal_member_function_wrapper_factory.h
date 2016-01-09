/*!
@file
Defines `clbl::universal_member_function_wrapper_factory`.

@copyright Barrett Adair 2015
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)

*/

#ifndef CLBL_UNIVERSAL_MEMBER_FUNCTION_WRAPPER_FACTORY_H
#define CLBL_UNIVERSAL_MEMBER_FUNCTION_WRAPPER_FACTORY_H

#include <utility>

#include <CLBL/tags.h>
#include <CLBL/type_traits.h>
#include <CLBL/cv.h>
#include <CLBL/qflags.h>
#include <CLBL/factory/member_function_wrapper_factory.h>
#include <CLBL/internal/member_function/universal_member_function_wrapper.h>

namespace clbl {
    
    namespace universal_member_function_detail {

        //todo replace with clbl::pmf::define equivvalent
        template<typename Creator, typename Bad>
        struct wrap_t {
            static_assert(sizeof(Bad) < 0, "Not a member function.");
        };
        
        #define CLBL_SPECIALIZE_UNIVERSAL_MEMBER_FUNCTION_WRAP(qualifiers) \
        template<typename Creator, typename Return, typename T , typename... Args> \
        struct wrap_t<Creator, Return(T::*)(Args...) qualifiers> { \
            using member_fn_ptr = Return(T::*)(Args...) qualifiers; \
            inline constexpr auto \
            operator()(const member_fn_ptr& ptr) const { \
                using wrapper = universal_member_function_wrapper< \
                Creator, member_fn_ptr, Return(T::*)(Args...)>; \
                return wrapper{ ptr }; \
            } \
        }

        CLBL_SPECIALIZE_UNIVERSAL_MEMBER_FUNCTION_WRAP(CLBL_NO_CV);
        CLBL_SPECIALIZE_UNIVERSAL_MEMBER_FUNCTION_WRAP(&);
        CLBL_SPECIALIZE_UNIVERSAL_MEMBER_FUNCTION_WRAP(&&);
        CLBL_SPECIALIZE_UNIVERSAL_MEMBER_FUNCTION_WRAP(const);
        CLBL_SPECIALIZE_UNIVERSAL_MEMBER_FUNCTION_WRAP(volatile);
        CLBL_SPECIALIZE_UNIVERSAL_MEMBER_FUNCTION_WRAP(const volatile);
        CLBL_SPECIALIZE_UNIVERSAL_MEMBER_FUNCTION_WRAP(const &);
        CLBL_SPECIALIZE_UNIVERSAL_MEMBER_FUNCTION_WRAP(volatile &);
        CLBL_SPECIALIZE_UNIVERSAL_MEMBER_FUNCTION_WRAP(const volatile &);
        CLBL_SPECIALIZE_UNIVERSAL_MEMBER_FUNCTION_WRAP(const &&);
        CLBL_SPECIALIZE_UNIVERSAL_MEMBER_FUNCTION_WRAP(volatile &&);
        CLBL_SPECIALIZE_UNIVERSAL_MEMBER_FUNCTION_WRAP(const volatile &&);

        template<typename Creator, typename MemberFnPtr>
        constexpr wrap_t<Creator, no_ref<MemberFnPtr> > wrap {};
    }

    //todo slim
    struct universal_member_function_wrapper_factory {

        template<typename MemberFnPtr>
        static inline constexpr auto
        wrap(MemberFnPtr&& ptr){
            //using member_function as the creator - member_function takes CV flags for hardening
            return universal_member_function_detail::wrap<
                member_function_wrapper_factory, MemberFnPtr>(static_cast<MemberFnPtr&&>(ptr));
        }

        template<typename Invocation>
        static inline constexpr auto
            wrap_data(Invocation&& data) {
            return wrap<no_ref<Invocation> >(data);
        }
    };
}

#endif