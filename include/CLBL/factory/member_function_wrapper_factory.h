/*!
@file
Defines `clbl::member_function_wrapper_factory`.

@copyright Barrett Adair 2015
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)

*/

#ifndef CLBL_MEMBER_FUNCTION_WRAPPER_FACTORY_H
#define CLBL_MEMBER_FUNCTION_WRAPPER_FACTORY_H

#include <utility>

#include <CLBL/tags.h>
#include <CLBL/type_traits.h>
#include <CLBL/cv.h>
#include <CLBL/qflags.h>
#include <CLBL/internal/member_function/member_function_wrapper.h>

namespace clbl {
    
    namespace member_function_detail {

        template<typename Creator, qualify_flags, typename Bad>
        struct wrap_t {
            static_assert(sizeof(Bad) < 0, "Not a member function.");
        };
        
        #define CLBL_SPECIALIZE_MEMBER_FUNCTION_WRAP(qualifiers) \
        template<typename Creator, qualify_flags Flags, typename Return, typename T , typename... Args> \
        struct wrap_t<Creator, Flags, Return(T::*)(Args...) qualifiers> { \
            using member_fn_ptr = Return(T::*)(Args...) qualifiers; \
            inline constexpr auto \
            operator()(const member_fn_ptr& ptr) const { \
                using wrapper = member_function_wrapper< \
                Creator, Flags, member_fn_ptr, Return(T::*)(Args...)>; \
                return wrapper{ ptr }; \
            } \
        }

        CLBL_SPECIALIZE_MEMBER_FUNCTION_WRAP(CLBL_NO_CV);
        CLBL_SPECIALIZE_MEMBER_FUNCTION_WRAP(&);
        CLBL_SPECIALIZE_MEMBER_FUNCTION_WRAP(&&);
        CLBL_SPECIALIZE_MEMBER_FUNCTION_WRAP(const);
        CLBL_SPECIALIZE_MEMBER_FUNCTION_WRAP(volatile);
        CLBL_SPECIALIZE_MEMBER_FUNCTION_WRAP(const volatile);
        CLBL_SPECIALIZE_MEMBER_FUNCTION_WRAP(const &);
        CLBL_SPECIALIZE_MEMBER_FUNCTION_WRAP(volatile &);
        CLBL_SPECIALIZE_MEMBER_FUNCTION_WRAP(const volatile &);
        CLBL_SPECIALIZE_MEMBER_FUNCTION_WRAP(const &&);
        CLBL_SPECIALIZE_MEMBER_FUNCTION_WRAP(volatile &&);
        CLBL_SPECIALIZE_MEMBER_FUNCTION_WRAP(const volatile &&);

        template<typename Creator, qualify_flags Flags, typename MemberFnPtr>
        constexpr wrap_t<Creator, Flags, no_ref<MemberFnPtr> > wrap {};
    }

    struct member_function_wrapper_factory {

        using this_t = member_function_wrapper_factory;

        template<qualify_flags Flags, typename MemberFnPtr>
        static inline constexpr auto
        wrap(MemberFnPtr&& ptr){
            return member_function_detail::wrap<
                this_t, Flags, MemberFnPtr>(static_cast<MemberFnPtr&&>(ptr));
        }

        template<qualify_flags Flags, typename Invocation>
        static inline constexpr auto
            wrap_data(Invocation&& data) {
            return wrap<Flags, no_ref<Invocation> >(data);
        }
    };
}

#endif