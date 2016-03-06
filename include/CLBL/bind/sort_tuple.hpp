/*!
@file

@copyright Louis Dionne 2013-2016
@coyright Modified Work Barrett Adair 2016
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
*/

#ifndef CLBL_SORT_TUPLE_HPP
#define CLBL_SORT_TUPLE_HPP

#include <tuple>
#include <type_traits>

//This is a slightly modified version of Boost.Hana's sort
namespace clbl { namespace internal {
    
namespace detail {

    using index = std::size_t;
    using empty_seq = std::index_sequence<>;

    template <typename Pred, index Insert, bool IsInsertionPoint,
        typename Left, index ...Right>
    struct insert;

    // We did not find the insertion point; continue processing elements
    // recursively.
    template <typename Pred, index Insert, index ...Left,
        index Right1, index Right2, index ...Right>
    struct insert<Pred, Insert, false, std::index_sequence<Left...>,
        Right1, Right2, Right...> {
        using type = typename insert<
            Pred, Insert, Pred::template apply<Insert, Right2>::value,
            std::index_sequence<Left..., Right1>,
            Right2, Right...
        >::type;
    };

    // We did not find the insertion point, but there is only one element
    // left. We insert at the end of the list, and we're done.
    template <typename Pred, index Insert, index ...Left, index Last>
    struct insert<Pred, Insert, false, std::index_sequence<Left...>, Last> {
        using type = std::index_sequence<Left..., Last, Insert>;
    };

    // We found the insertion point, we're done.
    template <typename Pred, index Insert, index ...Left, index ...Right>
    struct insert<Pred, Insert, true, std::index_sequence<Left...>, Right...> {
        using type = std::index_sequence<Left..., Insert, Right...>;
    };

    template <typename Pred, typename Result, index ...T>
    struct insertion_sort;

    template <typename Pred, index Result1, index ...Results,
        index T, index ...Ts>
    struct insertion_sort<Pred, std::index_sequence<Result1, Results...>, T, Ts...> {
        static constexpr bool pred_result = Pred::template apply<T, Result1>::value;
        using insert_result = typename insert<
            Pred, T, pred_result, empty_seq, Result1, Results...
        >::type;
        using type = typename insertion_sort<Pred, insert_result, Ts...>::type;
    };

    template <typename Pred, index T, index ...Ts>
    struct insertion_sort<Pred, empty_seq, T, Ts...> {
        using type = typename insertion_sort<
            Pred, std::index_sequence<T>, Ts...
        >::type;
    };

    template <typename Pred, typename Result>
    struct insertion_sort<Pred, Result> {
        using type = Result;
    };

    template <typename Pred, typename Indices>
    struct sort_indices;

    template <typename Pred, index ...i>
    struct sort_indices<Pred, std::index_sequence<i...>> {
        using type = typename insertion_sort<Pred, empty_seq, i...>::type;
    };

    template<typename Tup, typename Pred>
    struct sort_impl {
        static constexpr index len = std::tuple_size<Tup>::value;
        using indices = typename sort_indices<Pred, std::make_index_sequence<len>>::type;
        using type = typename sort_impl<Tup, indices>::type;
    };

    template <typename Tup, index ...i>
    struct sort_impl<Tup, std::index_sequence<i...>> {
        using type = std::tuple<typename std::tuple_element<i, Tup>::type...>;
    };

    template <typename Tup, template<class, class> class Pred>
    struct predicate {
        template <index I, index J>
        using apply = Pred<
            typename std::tuple_element<I, Tup>::type,
            typename std::tuple_element<J, Tup>::type
        >;
    };

} // end namespace detail

template<typename Tup, template<class, class> class Pred>
using sort_tuple = typename detail::sort_impl<Tup, detail::predicate<Tup, Pred>>::type;

}}

#endif