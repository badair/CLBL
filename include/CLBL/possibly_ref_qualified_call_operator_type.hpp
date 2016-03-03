/*!
@file

@copyright Barrett Adair 2015
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)

*/

#ifndef CLBL_POSSIBLY_REF_QUALIFIED_CALL_OPERATOR_TYPE_H
#define CLBL_POSSIBLY_REF_QUALIFIED_CALL_OPERATOR_TYPE_H

#include <utility>

#include <CLBL/type_traits.hpp>

namespace clbl {

    namespace detail {

        template<typename PmfDispatch, typename T, quali::flags QFlags = quali::default_>
        struct possibly_ref_qualified_call_operator
        {
            static constexpr const quali::flags flags_without_reference = quali::remove_reference<QFlags>::value;

            using reference_pmf_dispatch = typename PmfDispatch::template add_qualifiers<QFlags>;
            using reference_pmf_cast = typename reference_pmf_dispatch::type;

            using normal_pmf_dispatch = typename PmfDispatch::remove_reference;
            using normal_pmf_cast_dispatch = typename normal_pmf_dispatch::template add_qualifiers<flags_without_reference>;
            using normal_pmf_cast = typename normal_pmf_cast_dispatch::type;

            template<typename U, CLBL_REQUIRES_(flags_without_reference != QFlags)>
            static std::int16_t test(decltype(static_cast<reference_pmf_cast>(&U::operator())));

            template<typename U>
            static std::int8_t test(decltype(static_cast<normal_pmf_cast>(&U::operator())));

            static constexpr const bool is_ref_qualified =
                sizeof(decltype(possibly_ref_qualified_call_operator::test<T>(nullptr))) == sizeof(std::int16_t);

            using type = typename std::conditional<is_ref_qualified, reference_pmf_cast, normal_pmf_cast>::type;
        };
    }

    template<typename PmfDispatch, typename T, quali::flags QFlags = quali::default_>
    using possibly_ref_qualified_call_operator_type =
        typename detail::possibly_ref_qualified_call_operator<PmfDispatch, T, QFlags>::type;


}

#endif