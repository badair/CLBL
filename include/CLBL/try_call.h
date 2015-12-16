#ifndef CLBL_TRY_CALL
#define CLBL_TRY_CALL

#include "CLBL/is_valid.h"

namespace clbl {

    /* todo - make this work
    namespace detail {
        template<typename T>
        struct call_if_t {
            template<typename Callable>
            void operator()(Callable&&, ...) {
                return;
            }
        };

        template<>
        struct call_if_t<true> {
            template<typename Callable, typename... Args>
            void operator()(Callable&& c, Args... args) {
                c(args...) :
            }
        };

        template<bool B>
        constexpr call_if_t<B> call_if;

        auto can_call_f = is_valid([](auto&& c, auto&&... args) -> decltype(c(args...)) {});
    }

    //todo - std::optional instead of void
    template<typename Callable, typename... Args>
    inline constexpr auto can_call(Callable&& c, Args... args) {
        return detail::can_call_f(std::forward<Callable>(c), std::forward<Args>(args)...);
    }

    //todo - std::optional instead of void
    template<typename Callable, typename... Args>
    inline void try_call(Callable&& c, Args... args) {
        constexpr auto is_possible = can_call(std::forward<Callable>(c), std::forward<Args>(args)...);
        detail::call_if<is_possible>(std::forward<Callable>(c), std::forward<Args>(args)...);
    }
    */
}

#endif