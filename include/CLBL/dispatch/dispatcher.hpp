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
#include <quali/quali.hpp>
#include <CLBL/is_reference_wrapper.hpp>
#include <CLBL/wrapper/callable_wrapper.hpp>

namespace clbl { namespace internal {

template<typename Dispatch, typename>
struct dispatcher {
    static_assert(Dispatch::is_valid, "Invalid argument passed to clbl::fwrap.");
};

//todo - pass dispatch result down
template<typename U, typename ForwardingReference>
struct dispatcher<function_ptr<U>, ForwardingReference> {

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

template<typename U, typename ForwardingReference>
struct dispatcher<function_reference<U>, ForwardingReference> {

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

template<typename U, typename ForwardingReference>
struct dispatcher<pmf<U>, ForwardingReference> {

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
            bound_pmf_wrapper_base<quali::generalized_object<no_ref<Obj>>, dispatch_info>
        >{{{static_cast<Obj&&>(o)}, p}};
    }
};

template<typename Generalized, typename ForwardingReference>
struct dispatcher<function_object<Generalized>, ForwardingReference> {

    using dispatch_info = function_object<Generalized>;

    template<quali::flags Flags>
    using callable_base = typename std::conditional<
        dispatch_info::is_ambiguous,
        ambiguous_function_object_wrapper_base<Flags, Generalized>,
        function_object_wrapper_base<Flags, Generalized>
    >::type;

    template<quali::flags Flags>
    using result = callable_wrapper<callable_base<Flags>>;

    inline constexpr decltype(auto)
    operator()(ForwardingReference t) const {
        return result<
            quali::cv_of<std::remove_reference_t<ForwardingReference>>::value 
            | (is_reference_wrapper<typename Generalized::original_type>::value
                || quali::ref_of<ForwardingReference>::value == quali::rvalue_reference_?
                    quali::default_ : quali::lvalue_reference_
                )
        >{static_cast<ForwardingReference>(t)};
    }
};

template<typename GeneralizedWrapper, typename ForwardingReference>
struct dispatcher<wrapper_object<GeneralizedWrapper>, ForwardingReference> {

    using wrapper = typename unqualified<GeneralizedWrapper>::type;

    inline constexpr decltype(auto)
    operator()(ForwardingReference t) const {
        return wrapper::transform(quali::generalize(static_cast<ForwardingReference>(t)));
    }
};

}}

#endif