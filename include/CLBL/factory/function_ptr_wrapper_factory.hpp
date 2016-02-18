/*!
@file
Defines `clbl::function_ptr`.

@copyright Barrett Adair 2015
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)

*/

#ifndef CLBL_FUNCTION_PTR_WRAPPER_FACTORY_H
#define CLBL_FUNCTION_PTR_WRAPPER_FACTORY_H

#include <type_traits>
#include <tuple>
#include <utility>
#include <CLBL/tags.hpp>
#include <CLBL/type_traits.hpp>
#include <CLBL/cv_of.hpp>
#include <CLBL/qflags.hpp>
#include <CLBL/forward.hpp>
#include <CLBL/harden_cast.hpp>
#include <CLBL/internal/bind/binding_wrapper.hpp>
#include <CLBL/internal/function/function_ptr_wrapper.hpp>
#include <CLBL/internal/function/slim_function_ptr_wrapper.hpp>

namespace clbl {

struct function_ptr_wrapper_factory {

    using this_t = function_ptr_wrapper_factory;

    template<typename FunctionPtr>
    static inline constexpr auto
    wrap(FunctionPtr&& f) {
        using function_type = std::remove_pointer_t<no_ref<FunctionPtr> >;
        using wrapper = internal::function_ptr_wrapper<this_t, function_type>;
        return wrapper{ static_cast<FunctionPtr&&>(f) };
    }

    template<qualify_flags Ignored, typename Data>
    static inline constexpr auto
    wrap_data(Data&& data) {
        return wrap(data);
    }

    struct slim {

        using this_t = slim;

        template<typename FunctionPtr, FunctionPtr Value>
        static inline constexpr auto
        wrap() {
            using function_type = std::remove_pointer_t<FunctionPtr>;
            using wrapper = internal::slim_function_ptr_wrapper<
                                this_t,
                                function_type,
                                Value>;
            return wrapper{};
        }

        template<qualify_flags Ignored, typename Data>
        static inline constexpr auto
        wrap_data(Data&&) {
            using ptr_container = no_ref<Data>;
            using ptr_type = typename ptr_container::type;
            return wrap<ptr_type, ptr_container::value>();
        }
    };
};

}

#endif