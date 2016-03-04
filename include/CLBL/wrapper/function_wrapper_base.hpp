 /*!
@file

@copyright Barrett Adair 2015
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)

*/

#ifndef CLBL_FUNCTION_WRAPPER_BASE_H
#define CLBL_FUNCTION_WRAPPER_BASE_H

namespace clbl { namespace internal {

template<typename ConstructorType>
class function_wrapper_data {

public:

    using invocation_data_type = ConstructorType;

    inline constexpr
    function_wrapper_data(const invocation_data_type & v)
        : data{v} {}

protected:

    const invocation_data_type data;
};


template<typename T, T Value>
class function_wrapper_data<std::integral_constant<T, Value>> {

public:
    
    using invocation_data_type = T;

    inline constexpr
    function_wrapper_data(const std::integral_constant<T, Value>& )
    {}

protected:

    static constexpr const auto data = Value;
};

template<typename Dispatch>
class function_wrapper_base 
    : public Dispatch, function_wrapper_data<typename Dispatch::constructor_type> {

public:
    using base = function_wrapper_data<typename Dispatch::constructor_type>;
    using base::base;
    using clbl_tag = function_wrapper_tag;

protected:

    template<typename... Args>
    inline constexpr decltype(auto)
    invoke(Args&&... args) const {
        return base::data(static_cast<Args&&>(args)...);
    }

    template<typename... Args>
    inline constexpr decltype(auto)
    invoke(Args&&... args) const volatile {
        return base::data(static_cast<Args&&>(args)...);
    }
};

}}

#endif