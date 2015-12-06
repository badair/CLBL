#include <functional>
#include <boost/hana.hpp>

#include "CLBL/tags.h"
#include "CLBL/forwardable.h"

namespace hana = boost::hana;

namespace clbl {

    namespace detail {
        template<typename Bad>
        struct harden_t {
            static_assert(sizeof(Bad) < 0, "Not a valid function type.");
        };

        template<typename Return, typename... Args>
        struct harden_t<Return(Args...)> {

            template<typename Callable>
            inline constexpr auto operator()(Callable c) const {
                return func(std::move(c), static_cast<Return(Callable::*)(forwardable<Args>...)>(&Callable::operator()));
            }

            template<typename Callable>
            inline constexpr auto operator()(std::reference_wrapper<Callable> c) const {
                return func(c, static_cast<Return(Callable::*)(forwardable<Args>...)>(&Callable::operator()));
            }
        };

        struct default_harden_tag {};

        template<>
        struct harden_t<default_harden_tag> {

            template<typename T>
            struct unpack_args {

            };

            template<typename... Args>
            struct unpack_args<hana::tuple<Args...> > {

                template<typename Callable>
                static inline constexpr auto default_harden(Callable&& c) {
                    static_assert(!std::is_same<Callable::return_t, ambiguous_return>::value,
                        "Unable to disambiguate. Please specify a function type with clbl::harden<Return(Arg1, Arg2, ...)>() ");

                    return func(std::forward<Callable>(c), static_cast<Callable::return_t(Callable::*)(forwardable<Args>...)>(&Callable::operator()));
                }

                template<typename Callable>
                static inline constexpr auto default_harden(std::reference_wrapper<Callable> c) {
                    static_assert(!std::is_same<Callable::return_t, ambiguous_return>::value,
                        "Unable to disambiguate. Please specify a function type with clbl::harden<Return(Arg1, Arg2, ...)>() ");
                    return func(c, static_cast<Callable::return_t(Callable::*)(forwardable<Args>...)>(&Callable::operator()));
                }

            };

            template<typename Callable>
            inline constexpr auto operator()(Callable&& c) const {
                return unpack_args<std::remove_cv_t<std::remove_reference_t<Callable> >::args_t>::default_harden(std::forward<Callable>(c));
            }

            template<typename Callable>
            inline constexpr auto operator()(std::reference_wrapper<Callable> c) const {
                return unpack_args<Callable::args_t>::default_harden(c);
            }
        };

        template<typename T>
        constexpr harden_t<T> harden_v{};
    }

    template<typename Callable>
    inline constexpr auto harden(Callable c) {
        return detail::harden_v<detail::default_harden_tag>(std::forward<Callable>(std::move(c)));
    }

    template<typename FunctionType, typename Callable>
    inline constexpr auto harden(Callable c) {
        return detail::harden_v<FunctionType>(std::forward<Callable>(std::move(c)));
    }
}