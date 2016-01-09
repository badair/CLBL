#ifndef TEST_UTILITIES_H
#define TEST_UTILITIES_H

#include <cassert>
#include <type_traits>

namespace test {

    template<typename T, typename U>
    constexpr auto is_same = std::is_same<T, U>::value;

    template<typename From, typename To>
    constexpr auto can_convert = std::is_convertible<From, To>::value;

    template<typename T, typename Return>
    void assert_return(T&&) {
        using callable = std::remove_reference_t<T>;
        static_assert(is_same<callable::return_type, Return>, "is_same<callable::return_type, Return>");
        static_assert(is_same<result_of<callable> , Return>, "is_same<result_of<callable> , Return>");
    }

    template<typename T, typename... Args>
    void assert_args(T&&){
        using callable = std::remove_reference_t<T>;
        static_assert(is_same<callable::arg_types, std::tuple<Args...> >, "");
        static_assert(is_same<args<callable>, std::tuple<Args...> >, "");
    }

    template<typename> struct call {};
    template<typename Result typename...Args>
    struct call<Result(Args...)> {
        template<typename T>
        void assert_valid(T&&){
            static_assert(is_same<decltype(std::declval<T>()(std::declval<Args>()...)), Result>, "");
        }
    };

    template<typename T, typename Result, typename...Args>
    void assert_result(T&& c, Result&& r, Args&&... args) {
        assert(c(std::forward<Args>(args)...) == r);
    }
}

#endif