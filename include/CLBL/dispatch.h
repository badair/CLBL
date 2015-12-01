#ifndef CLBL_DISPATCH_H
#define CLBL_DISPATCH_H

#include <utility>

namespace clbl {
    namespace detail {

        //this is slower but more flexible - use if more than 4 cases needed
        /*template<bool... Bools>
        struct dispatch {
            template<typename... Args>
            static constexpr auto result(Args... a) {
                constexpr auto bools = hana::make_tuple(hana::bool_<Bools>{}...);
                auto args = hana::make_tuple(a...);
                auto zipped = hana::zip(bools, args);
                auto result = hana::filter(zipped, [](auto element) {return element[0_c];});
                return result[0_c][1_c];
            }
        };*/

        /*
        template<>
        struct dispatch<true> {
        template<typename Arg1, typename Arg2>
        static constexpr auto result(Arg1&& a1, Arg2&& a2) {
        return std::forward<Arg1>(a1);
        }
        };

        template<>
        struct dispatch<false> {
        template<typename Arg1, typename Arg2>
        static constexpr auto result(Arg1&& a1, Arg2&& a2) {
        return std::forward<Arg2>(a2);
        }
        };*/

        template<bool, bool, bool, bool>
        struct dispatch {};

        template<>
        struct dispatch<true, false, false, false> {
            template<typename Result, typename T1, typename T2, typename T3>
            static constexpr auto result(Result&& r, T1&&, T2&&, T3&&) {
                return std::forward<Result>(r);
            }
        };

        template<>
        struct dispatch<false, true, false, false> {
            template<typename T0, typename Result, typename T2, typename T3>
            static constexpr auto result(T0&&, Result&& r, T2&&, T3&&) {
                return std::forward<Result>(r);
            }
        };

        template<>
        struct dispatch<false, false, true, false> {
            template<typename T0, typename T1, typename Result, typename T3>
            static constexpr auto result(T0&&, T1&&, Result&& r, T3&&) {
                return std::forward<Result>(r);
            }
        };

        template<>
        struct dispatch<false, false, false, true> {
            template<typename T0, typename T1, typename T2, typename Result>
            static constexpr auto result(T0&&, T1&&, T2&&, Result&& r) {
                return std::forward<Result>(r);
            }
        };
    }
}

#endif