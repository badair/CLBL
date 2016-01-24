/*!
@file
Defines `clbl::function_reference_wrapper_factory`.

@copyright Barrett Adair 2015
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)

*/

#ifndef CLBL_FUNCTION_REFERENCE_WRAPPER_FACTORY_H
#define CLBL_FUNCTION_REFERENCE_WRAPPER_FACTORY_H

#include <tuple>
#include <utility>
#include <CLBL/tags.h>
#include <CLBL/type_traits.h>
#include <CLBL/cv.h>
#include <CLBL/qflags.h>
#include <CLBL/forward.h>
#include <CLBL/harden_cast.h>
#include <CLBL/internal/function/function_reference_wrapper.h>
#include <CLBL/internal/function/slim_function_reference_wrapper.h>

namespace clbl {

    struct function_reference_wrapper_factory {

        using this_t = function_reference_wrapper_factory;

        template<typename FunctionReferenceType>
        static inline constexpr auto
        wrap(FunctionReferenceType&& t) {
            using function_type = no_ref<FunctionReferenceType>;
            using wrapper = internal::function_reference_wrapper<this_t, function_type>;
            return wrapper{ static_cast<FunctionReferenceType&&>(t) };
        }

        template<qualify_flags Ignored, typename Invocation>
        static inline constexpr auto
            wrap_data(Invocation&& data) {
            return wrap(static_cast<Invocation&&>(data));
        }

        struct slim {

            using this_t = slim;
            
            template<typename FunctionReferenceType, FunctionReferenceType Value>
            static inline constexpr auto
            wrap() {
                using wrapper = internal::slim_function_reference_wrapper<
                    this_t, FunctionReferenceType, Value>;
                return wrapper{};
            }

            //todo - slim implementation of wrap_data
            template<qualify_flags Ignored, typename Data>
            static inline constexpr auto
            wrap_data(Data&&) {
                using function_reference_type = typename no_ref<Data>::type;
                return wrap<function_reference_type, no_ref<Data>::value>();
            }
        };
    };
}

#endif