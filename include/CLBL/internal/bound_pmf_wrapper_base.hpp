 /*!
@file

@copyright Barrett Adair 2015
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)

*/

#ifndef CLBL_BOUND_PMF_WRAPPER_BASE_H
#define CLBL_BOUND_PMF_WRAPPER_BASE_H

namespace clbl { namespace internal {

template<typename BoundObject, typename PmfConstructorType>
class bound_pmf_wrapper_data {

public:
    
    struct invocation_data_type {
        BoundObject object;
        PmfConstructorType pmf;
    };

    inline constexpr
    bound_pmf_wrapper_data(const invocation_data_type& v)
        : data{v} {}

    inline constexpr
    bound_pmf_wrapper_data(invocation_data_type&& v)
        : data{static_cast<invocation_data_type&&>(v)} {}

protected:

    invocation_data_type data;

    inline constexpr decltype(auto)
        get_pmf() {
        return data.pmf;
    }

    inline constexpr decltype(auto)
    get_pmf() const {
        return data.pmf;
    }
};


template<typename BoundObject, typename T, T Value>
class bound_pmf_wrapper_data<BoundObject, std::integral_constant<T, Value>> {

public:
    
    struct invocation_data_type {

       inline constexpr
        invocation_data_type(const BoundObject& o, std::integral_constant<T, Value> )
            : object{o} {}

        inline constexpr
        invocation_data_type(BoundObject&& o, std::integral_constant<T, Value> )
            : object{static_cast<BoundObject&&>(o)} {}

        BoundObject object;
    };

    inline constexpr
    bound_pmf_wrapper_data(const invocation_data_type& v)
        : data{v}{}

    inline constexpr
    bound_pmf_wrapper_data(invocation_data_type&& v)
        : data{static_cast<invocation_data_type&&>(v)} {}

protected:

    invocation_data_type data;

    static inline constexpr decltype(auto)
    get_pmf() {
        return Value;
    }
};

template<typename BoundObject, typename Dispatch>
struct bound_pmf_wrapper_base
    : public Dispatch,
    public bound_pmf_wrapper_data<BoundObject, typename Dispatch::constructor_type> {

    using base = bound_pmf_wrapper_data<BoundObject, typename Dispatch::constructor_type>;
    using base::base;
    using arg_types = typename Dispatch::arg_types;
    using clbl_tag = bound_pmf_tag;
    using forwarding_glue = typename Dispatch::forwarding_glue;
    using type = typename Dispatch::decay_to_function;

    static constexpr auto q_flags = Dispatch::q_flags;
    static constexpr auto is_ambiguous = false;

    template<typename... Args>
    inline CLBL_CXX14_CONSTEXPR decltype(auto)
    invoke(Args&&... args) {
        return (base::data.object.template get<q_flags>().*base::get_pmf())(
            static_cast<Args&&>(args)...
        );
    }

    template<typename... Args>
    inline constexpr decltype(auto)
    invoke(Args&&... args) const {
        return (base::data.object.template get<quali::const_ | q_flags>().*base::get_pmf())(
            static_cast<Args&&>(args)...
        );
    }

    template<typename... Args>
    inline CLBL_CXX14_CONSTEXPR decltype(auto)
    invoke(Args&&... args) volatile {
        return (base::data.object.template get<quali::volatile_ | q_flags>().*base::get_pmf())(
            static_cast<Args&&>(args)...
        );
    }

    template<typename... Args>
    inline constexpr decltype(auto)
    invoke(Args&&... args) const volatile {
        return (base::data.object.template get<
                quali::const_ | quali::volatile_ | q_flags
            >().*base::data.pmf)(static_cast<Args&&>(args)...);
    }
};

}}

#endif