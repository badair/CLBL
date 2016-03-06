/*!
@copyright Barrett Adair 2016

Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
*/

#ifndef CLBL_BIND_PLACEHOLDER_ROUTES_HPP
#define CLBL_BIND_PLACEHOLDER_ROUTES_HPP

namespace clbl{ namespace internal { namespace detail { 

template<std::size_t OutputIndex, std::size_t InputIndex>
struct ph_route {
    static constexpr const auto output_index = OutputIndex;
    static constexpr const auto input_index = InputIndex;
};

template<typename, typename> struct argument_routing {};

template<std::size_t... I, typename Tuple>
struct argument_routing<std::index_sequence<I...>, Tuple> {
    using type =
        std::tuple<
            ph_route<
                I,
                std::is_placeholder<
                    typename std::tuple_element<I, Tuple>::type
                >::value
            >...
        >;
};

template <typename...> struct prepend;

template <> struct prepend<> {
    using type = std::tuple<>;
};

template <typename T, typename... Args>
struct prepend<T, std::tuple<Args...> > {
    using type = std::tuple<T, Args...>;
};

//base case
template <typename...> struct placeholder_routes_detail;
template <> struct placeholder_routes_detail<> {
    using type = std::tuple<>;
};

template <typename Head, typename ...Tail>
struct placeholder_routes_detail<Head, Tail...> {
    //TODO - is there a faster way to do this?
    using type = typename std::conditional<
        Head::input_index == 0,
        typename placeholder_routes_detail<Tail...>::type,
        typename prepend<
            Head,
            typename placeholder_routes_detail<Tail...>::type
        >::type
    >::type;
};

template <typename... Args>
struct placeholder_routes_detail<std::tuple<Args...> > {
    using type = typename placeholder_routes_detail<Args...>::type;
};

template<typename PhLeft, typename PhRight>
struct compare_placeholders {
    static constexpr bool value = 
        std::is_placeholder<PhLeft>::value < std::is_placeholder<PhRight>::value;
};

template <typename...> struct placeholder_routes;
template <> struct placeholder_routes<> { using type = std::tuple<>; };

template <typename... Args>
struct placeholder_routes<clbl::internal::detail::args_tuple<Args...> >
{
    using routed_placeholders = typename placeholder_routes_detail<
        typename argument_routing<
            std::make_index_sequence<sizeof...(Args)>,
            std::tuple<Args...>
        >::type
    >::type;

    using type = sort_tuple<routed_placeholders, compare_placeholders>;
};

}}}

#endif