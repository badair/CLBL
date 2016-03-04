/*!
@file

@copyright Barrett Adair 2015
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)

*/

#ifndef CLBL_FUNCTION_REFERENCE_H
#define CLBL_FUNCTION_REFERENCE_H

namespace clbl {
    
template<typename T>
struct function_reference {
    static constexpr const bool is_valid = false;
    static constexpr const bool value = is_valid;
    static constexpr const bool is_ambiguous = false;
    using dispatch_type = function_reference;
};

template<typename Return, typename... Args>
struct function_reference<Return(&)(Args...)>{
    static constexpr const bool is_valid = true;
    static constexpr const bool value = is_valid;
    static constexpr const bool has_varargs = false;
    static constexpr const bool is_ambiguous = false;
    using return_type = Return;
    using arg_types = std::tuple<Args...>;
    using type = Return(&)(Args...);
    using function_type = Return(Args...);
    using forwarding_glue = Return(forward<Args>...);
    using constructor_type = type;
    using dispatch_type = function_reference;
};

template<typename Return, typename... Args>
struct function_reference<Return(&)(Args..., ...)>
    : public function_reference<Return(&)(Args...)> {
    static constexpr const bool has_varargs = true;
    using dispatch_type = function_reference;
};

}

#endif
