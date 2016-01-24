 /*
@file
Defines `clbl::function_object_ptr_wrapper_factory`.

Copyright Barrett Adair 2015
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)

*/

#ifndef CLBL_FUNCTION_OBJECT_PTR_WRAPPER_FACTORY_H
#define CLBL_FUNCTION_OBJECT_PTR_WRAPPER_FACTORY_H

#include <utility>

#include <CLBL/cv_of.h>
#include <CLBL/tags.h>
#include <CLBL/type_traits.h>
#include <CLBL/harden_cast.h>
#include <CLBL/qflags.h>
#include <CLBL/internal/member_function/slim_member_function_bound_to_object_ptr_wrapper.h>
#include <CLBL/internal/function_object/casted_function_object_ptr_wrapper.h>
#include <CLBL/internal/function_object/ambiguous_function_object_ptr_wrapper.h>

namespace clbl {

    struct function_object_ptr_wrapper_factory {

    	using this_t = function_object_ptr_wrapper_factory;

        template<qualify_flags Flags, typename FunctionObjectPtr>
        static inline constexpr auto 
        wrap(FunctionObjectPtr&& object_ptr) {

        	CLBL_ASSERT_VALID_REFERENCE_FLAGS(Flags);

            constexpr auto cv_qualifiers = cv_of<FunctionObjectPtr> | Flags;
            using object_type = no_ref<decltype(*std::declval<FunctionObjectPtr>())>;
            using member_fn_type = decltype(&object_type::operator());
            using ptr_type = no_ref<FunctionObjectPtr>;
            using wrapper = slim_member_function_bound_to_object_ptr_wrapper<
                                this_t, //todo may need to pass a different factory - test clbl::harden
                                cv_qualifiers,          
                                object_type,
                                ptr_type,
                                member_fn_type,
                                &object_type::operator()>;
            return wrapper{ static_cast<FunctionObjectPtr&&>(object_ptr) };
        }

        template<qualify_flags Flags, typename Invocation>
        static inline constexpr auto
        wrap_data(Invocation&& data) {
            return wrap<Flags>(data.object_ptr);
        }

        struct ambiguous {

    		using this_t = ambiguous;

            template<qualify_flags Flags, typename T>
            static inline constexpr auto
            wrap(T&& t) {

        		CLBL_ASSERT_VALID_REFERENCE_FLAGS(Flags);

                constexpr auto cv_qualifiers = cv_of<T> | Flags;
                using wrapper = internal::ambiguous_function_object_ptr_wrapper<
                                    this_t,
                                    cv_qualifiers,
                                    no_ref<T> >;
                return wrapper{static_cast<T&&>(t)};
            }

            template<qualify_flags Flags, typename Invocation>
            static inline constexpr auto
            wrap_data(Invocation&& data) {
                return wrap<Flags>(data.ptr);
            }
        };

        struct casted {

			using this_t = casted;

            template<qualify_flags Flags = qflags::default_, typename Pmf, typename T>
            static inline constexpr auto
            wrap(T&& t) {
                constexpr auto cv_qualifiers = cv_of<T> | Flags;
                using object_type = no_ref<decltype(*std::declval<no_ref<T> >())>;
                using ptr_type = no_ref<T>;
                using wrapper = internal::casted_function_object_ptr_wrapper<
                                    this_t,
                                    cv_qualifiers,
                                    object_type,
                                    ptr_type,
                                    Pmf>;
                return wrapper{ static_cast<T&&>(t) };
            }

            template<qualify_flags Flags, typename Invocation>
            static inline constexpr auto
            wrap_data(Invocation&& data) {
                /*
                no_const here removes constness from the 
                type of the constexpr pointer in Invocation, which is
                only const because of constexr. This has no affect
                whatsoever on const-correctness
                */
                using pmf_type = no_const<decltype(no_ref<Invocation>::pmf)>;
                return wrap<Flags, pmf_type>(data.object_ptr);
            }
        };
    };

}

#endif