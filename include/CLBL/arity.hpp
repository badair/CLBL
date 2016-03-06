 /*!
@file

@copyright Barrett Adair 2015
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)

*/

#ifndef CLBL_ARITY_HPP
#define CLBL_ARITY_HPP

#include <cstdint>

namespace clbl {

namespace arity_detail {

    //any_arg is only used in unevaluated contexts
    template<std::size_t I>
    struct any_arg {

        template<typename T>
        operator T&& () {
            return std::declval<T&&>();
        }

        any_arg () = default;

        template<typename... T>
        any_arg (T&&...) {}

        //todo - this is currently unused
        static constexpr std::size_t index = I;
    };


    template<typename, typename>
    struct max_args_t {
        static constexpr bool value = true;
        static constexpr int arg_count = -1;
    };

    template<typename U>
    struct max_args_t<U, std::index_sequence<0>> {
        static constexpr bool value = true;
        static constexpr int arg_count =
            can_call_with<U, any_arg<1>>::value ? 1 : (
                can_call_with<U, void>::value ? 0 : -1
            );
    };

    template<typename U, std::size_t... I>
    struct max_args_t<U, std::index_sequence<I...>> {

        using result_type = disjunction<
            can_call_with<U, any_arg<I>...>,
            max_args_t<U, std::make_index_sequence<sizeof...(I)-1> >
        >;

        static constexpr bool value = result_type::value;
        static constexpr int arg_count = result_type::arg_count;
    };

    template<typename U, std::size_t Max, typename = int>
    struct min_args_t;

    struct sentinel {};

    template<typename U, std::size_t Max>
    struct min_args_t<U, Max, sentinel> {
        static constexpr bool value = true;
        static constexpr int arg_count = -1;
    };

    template<typename U, std::size_t Max, std::size_t... I>
    struct min_args_t<U, Max, std::index_sequence<I...>> {

        using next = std::conditional_t<
            sizeof...(I) + 1 <= Max,
            std::make_index_sequence<sizeof...(I) + 1>,
            sentinel
        >;

        using result_type = disjunction<
            can_call_with<U, any_arg<I>...>,
            min_args_t<U, Max, next>
        >;

        static constexpr bool value = result_type::value;
        static constexpr int arg_count = result_type::arg_count;
    };

    template<typename U, std::size_t Max>
    struct min_args_t<U, Max, void> {

        using result_type = disjunction<
            can_call_with<U, void>,
            min_args_t<U, Max, std::make_index_sequence<1>>
        >;

        static constexpr int arg_count = result_type::arg_count;
        static constexpr bool value = result_type::value;
    };

    struct dummy_wrapper {
        using arg_types = ambiguous_args;
    };

    template<typename T>
    struct lenient_tuple_size {
        static constexpr int value = -1;
    };

    template<typename... T>
    struct lenient_tuple_size <std::tuple<T...>> {
        static constexpr int value = sizeof...(T);
    };

} //namespace arity_detail

template<typename T, std::size_t SearchLimit = 7>
struct arity {

private: 

    using wrapper = std::conditional_t<
        is_clbl<T>::value,
        T,
        arity_detail::dummy_wrapper
    >;

    static constexpr int precient_arity = arity_detail::lenient_tuple_size<
        typename wrapper::arg_types
    >::value;

    static constexpr int tentative_min_arity = arity_detail::min_args_t<
        T, SearchLimit, void
    >::arg_count;

    static constexpr int tentative_max_arity = arity_detail::max_args_t<
        T, std::make_index_sequence<SearchLimit>
    >::arg_count;

public: 

    static constexpr std::size_t search_limit = SearchLimit;

    template<std::size_t N>
    using search = arity<T, N>;

    static constexpr int min = 
        precient_arity == -1 || tentative_min_arity < precient_arity
            ? tentative_min_arity : precient_arity;
            
    static constexpr int max = 
        precient_arity == -1 || tentative_max_arity > precient_arity
            ? tentative_max_arity : precient_arity;
};

}

#endif