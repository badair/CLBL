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

#include <CLBL/tags.hpp>
#include <CLBL/type_traits.hpp>
#include <CLBL/cv_of.hpp>
#include <CLBL/qflags.hpp>
#include <CLBL/internal/bind/binding_wrapper.hpp>
#include <CLBL/internal/member_function/member_function_wrapper.hpp>
#include <CLBL/internal/member_function/slim_member_function_wrapper.hpp>

namespace clbl {

    struct member_function_wrapper_factory {

        using this_t = member_function_wrapper_factory;

        template<qualify_flags Ignored, typename Pmf>
        static inline constexpr auto
        wrap(Pmf&& ptr){
            return internal::member_function_wrapper<
                this_t, Pmf>{{ static_cast<Pmf&&>(ptr) }};
        }

        template<qualify_flags Flags, typename Invocation>
        static inline constexpr auto
            wrap_data(Invocation&& data) {
            return wrap<Flags, no_ref<Invocation> >(data);
        }

        struct slim {
            
            using this_t = slim;

            template<typename Pmf, Pmf Value>
            static inline constexpr auto
            wrap() {
                using wrapper = internal::slim_member_function_wrapper<
                                    this_t,
                                    Pmf,
                                    Value>;
                return wrapper{};
            }

            template<qualify_flags Ignored, typename Data>
            static inline constexpr auto
            wrap_data(Data&&) {
                return wrap<
                            typename no_ref<Data>::type,
                            no_ref<Data>::ptr
                        >();
            }
        };
    };
}

#endif