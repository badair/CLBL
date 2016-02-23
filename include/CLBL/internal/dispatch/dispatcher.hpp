/*!
@copyright Barrett Adair 2015
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
*/

#ifndef CLBL_DISPATCHER_H
#define CLBL_DISPATCHER_H

#include <functional>
#include <type_traits>
#include <utility>
#include <CLBL/tags.hpp>
#include <CLBL/constraints.hpp>
#include <CLBL/type_traits.hpp>
#include <CLBL/is_clbl.hpp>
#include <CLBL/qflags.hpp>
#include <CLBL/generalized_object.hpp>
#include <CLBL/can_dereference.hpp>
#include <CLBL/is_reference_wrapper.hpp>
#include <CLBL/internal/callable_wrapper.hpp>

namespace clbl { namespace internal {

template<typename Dispatch, typename>
struct dispatcher {
    static_assert(Dispatch::is_valid, "Invalid argument passed to clbl::fwrap.");
};

//todo - pass dispatch result down
template<typename U, typename T>
struct dispatcher<function_ptr<U>, T> {

    using dispatch_info = function_ptr<U>;
    using result = callable_wrapper<function_wrapper_base<dispatch_info>>;

    inline constexpr decltype(auto)
    operator()(typename dispatch_info::type t) const {
        return result{t};
    }

    template<typename... Args>
    inline constexpr decltype(auto)
    operator()(typename dispatch_info::constructor_type t, Args&&... args) const {
        return result{t}.push_bind(static_cast<Args&&>(args)...);
    }
};

template<typename U, typename T>
struct dispatcher<function_reference<U>, T> {

    using dispatch_info = function_reference<U>;
    using result = callable_wrapper<function_wrapper_base<dispatch_info>>;

    inline constexpr decltype(auto)
    operator()(typename dispatch_info::constructor_type t) const {
        return result{t};
    }

    template<typename... Args>
    inline constexpr decltype(auto)
    operator()(typename dispatch_info::constructor_type t, Args&&... args) const {
        return result{t}.push_bind(static_cast<Args&&>(args)...);
    }
};

template<typename U, typename T>
struct dispatcher<pmf<U>, T> {

    using dispatch_info = pmf<U>;
    using pmf_type = typename dispatch_info::constructor_type;
    using class_type = typename dispatch_info::class_type;

    inline constexpr decltype(auto)
    operator()(pmf_type p) const {
        return callable_wrapper<pmf_wrapper_base<dispatch_info>>{p};
    }

    template<typename... Args>
    inline constexpr decltype(auto)
    operator()(pmf_type p, Args&&... args) const {
        return callable_wrapper<pmf_wrapper_base<dispatch_info>>{p}
            .push_bind(static_cast<Args&&>(args)...);
    }

    template<typename Obj>
    inline constexpr decltype(auto)
    operator()(pmf_type p, GenerallyConvertibleObject<class_type, Obj&&> o) const {
        return callable_wrapper<
            bound_pmf_wrapper_base<generalized_object<no_ref<Obj>>, dispatch_info>
        >{{{static_cast<Obj&&>(o)}, p}};
    }
};

template<typename Generalized, typename T>
struct dispatcher<function_object<Generalized>, T> {

    using dispatch_info = function_object<Generalized>;

    CLBL_REQUIRES(!dispatch_info::is_ambiguous)
    inline constexpr decltype(auto)
    operator()(T t) const {
        return callable_wrapper<
            function_object_wrapper_base<cv_of<T>::value, Generalized>
        > {{static_cast<T>(t)}};
    }

    CLBL_REQUIRES(dispatch_info::is_ambiguous)
    inline constexpr decltype(auto)
    operator()(T t) const {
        return callable_wrapper<
            ambiguous_function_object_wrapper_base<cv_of<T>::value, Generalized>
        >{{static_cast<T>(t)}};
    }
};

}}

#endif