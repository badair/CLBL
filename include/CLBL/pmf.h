#ifndef CLBL_PMF_H
#define CLBL_PMF_H

#include <CLBL/tags.h>
#include <CLBL/qualified_type.h>
#include <CLBL/harden_cast.h>
#include <CLBL/qflags.h>

namespace clbl {
namespace pmf {
    template<typename T>
    struct define { static_assert(sizeof(T) < 0, "invalid function type"); };

#define CLBL_SPECIALIZE_PMF(qualifiers) \
    template<typename Return, typename... Args> \
    struct define<Return(Args...) qualifiers> { \
        static constexpr auto flags = cv<dummy qualifiers> | ref_qualifiers<dummy qualifiers>; \
        using return_type = Return; \
        using arg_types = std::tuple<Args...>; \
        using abominable_type = Return(Args...) qualifiers; \
        template<typename T> \
        using apply_class = Return(T::*)(Args...) qualifiers; \
        template<typename T> \
        struct wrapper {}; \
        template<typename T> \
        struct wrapper<Return(T::*)(Args...) qualifiers> { \
            static constexpr auto flags = cv<dummy qualifiers> | reference_qualifiers<dummy qualifiers>; \
            using return_type = Return; \
            using arg_types = std::tuple<Args...>; \
            using abominable_type = Return(Args...) qualifiers; \
            using type = Return(T::*)(Args...) qualifiers; \
            using decay_type = Return(T::*)(Args...);\
            const type value; \
            inline wrapper(const type& val) : value(val) {}; \
            inline operator type() const volatile { \
                return value; \
            } \
            template<typename U, typename... Fargs> \
            inline auto invoke(U&& u, Fargs... args) const volatile { \
                return (static_cast<U&&>(u).*value)(static_cast<Fargs&&>(args)...); \
            } \
            using definition = define<Return(Args...) qualifiers>; \
            template<typename U> \
            static inline auto reseat(Return(U::*ptr)(Args...) qualifiers) { \
                return wrapper<Return(U::*)(Args...) qualifiers>{ptr}; \
            } \
        }; \
        template<typename T> \
        using apply_wrapper = wrapper<Return(T::*)(Args...) qualifiers>; \
        template<typename T> \
        static inline constexpr auto \
        disambiguate(Return(T::*ptr)(Args...) qualifiers) { \
            return ptr; \
        } \
        template<typename T> \
        static inline constexpr auto \
        wrap(Return(T::*ptr)(Args...) qualifiers) { \
            return apply_wrapper<T>{ptr}; \
        } \
    }

    CLBL_SPECIALIZE_PMF(CLBL_NO_CV);
    CLBL_SPECIALIZE_PMF(&);
    CLBL_SPECIALIZE_PMF(&&);
    CLBL_SPECIALIZE_PMF(const);
    CLBL_SPECIALIZE_PMF(volatile);
    CLBL_SPECIALIZE_PMF(const volatile);
    CLBL_SPECIALIZE_PMF(const &);
    CLBL_SPECIALIZE_PMF(volatile &);
    CLBL_SPECIALIZE_PMF(const volatile &);
    CLBL_SPECIALIZE_PMF(const &&);
    CLBL_SPECIALIZE_PMF(volatile &&);
    CLBL_SPECIALIZE_PMF(const volatile &&);

    template<typename Pmf>
    using args = typename Pmf::arg_types;

    template<typename Pmf>
    constexpr auto qualifiers = Pmf::flags;

    template<typename Pmf>
    using return_type = typename Pmf::return_type;

    template<typename Pmf>
    using decay = typename Pmf::decay_type;

    template<typename Pmf>
    using abominable = typename Pmf::abominable_type;

    template<typename Pmf>
    using type = typename Pmf::type;

    template<typename Pmf, typename... Args>
    inline constexpr auto
    invoke(Pmf&& p, Args&&... a) {
        return p.invoke(static_cast<Args&&>(a)...);
    }
}
}

#endif