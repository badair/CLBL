 /*!
@file
Defines `clbl::internal::function_ptr_wrapper`.

@copyright Barrett Adair 2015
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)

*/

#ifndef CLBL_PMF_WRAPPER_DATA_H
#define CLBL_PMF_WRAPPER_DATA_H

#include <quali/quali.hpp>

namespace clbl { namespace internal {

template<typename ConstructorType>
class pmf_wrapper_data {

public:
    
    using invocation_data_type = ConstructorType;

    inline constexpr
    pmf_wrapper_data(const invocation_data_type& v)
        : data{v} {}

    inline constexpr
    pmf_wrapper_data(invocation_data_type&& v)
        : data{static_cast<invocation_data_type&&>(v)} {}

protected:

    const invocation_data_type data;
};


template<typename T, T Value>
class pmf_wrapper_data<std::integral_constant<T, Value>> {

public:
    
    using invocation_data_type = T;

    inline constexpr
    pmf_wrapper_data(const std::integral_constant<T, Value>& )
    {}

protected:

    static constexpr const auto data = Value;
};

template<typename Dispatch>
class pmf_wrapper_base
    : public Dispatch, public pmf_wrapper_data<typename Dispatch::constructor_type> {

public:
    
    using base = pmf_wrapper_data<typename Dispatch::constructor_type>;
    using base::base;
    using underlying_type = typename Dispatch::invoke_type;

    using arg_types = typename Dispatch::template prepend_and_unpack_args_to_template<
        std::tuple, underlying_type
    >;

    using clbl_tag = pmf_tag;
    using forwarding_glue =
         typename Dispatch::template prepend_arg_to_forward_function<underlying_type>;
    using type = typename Dispatch::template prepend_arg_to_function<underlying_type>;

protected:

    template<typename Obj, typename... Args>
    inline CLBL_CXX14_CONSTEXPR decltype(auto)
    invoke(GenerallyConvertibleObject<underlying_type, Obj&&> o, Args&&... args) const {
        return (quali::generalize(static_cast<Obj&&>(o))
            .*base::data)(static_cast<Args&&>(args)...);
    }

    template<typename Obj, typename... Args>
    inline CLBL_CXX14_CONSTEXPR decltype(auto)
    move_invoke(GenerallyConvertibleObject<underlying_type, Obj&&> o, Args&&... args) const {
        return (quali::generalize(static_cast<Obj&&>(o))
            .*base::data)(static_cast<Args&&>(args)...);
    }
    
    template<typename Obj, typename... Args>
    inline CLBL_CXX14_CONSTEXPR decltype(auto)
    invoke(GenerallyConvertibleObject<underlying_type, Obj&&> o, Args&&... args) const volatile {
        return (quali::generalize(static_cast<Obj&&>(o))
            .*base::data)(static_cast<Args&&>(args)...);
    }

    template<typename Obj, typename... Args>
    inline CLBL_CXX14_CONSTEXPR decltype(auto)
    move_invoke(GenerallyConvertibleObject<underlying_type, Obj&&> o, Args&&... args) const volatile {
        return (quali::generalize(static_cast<Obj&&>(o))
            .*base::data)(static_cast<Args&&>(args)...);
    }
};

}}

#endif