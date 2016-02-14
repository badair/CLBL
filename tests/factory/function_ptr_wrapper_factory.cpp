/*


Copyright Barrett Adair 2015
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)

*/

#include <cassert>
#include <type_traits>
#include <CLBL/factory/function_ptr_wrapper_factory.h>

#undef NDEBUG

#define DEFINE_TEST_ARG_STRUCT(type_name) \
    static int move_count; \
    static int copy_count; \
    struct type_name { \
        int value = 0; \
        type_name() = default; \
        void operator++(int) volatile { value++; } \
        type_name(const type_name&) { copy_count++; } \
        type_name(type_name&&) { move_count++; } \
    }; \
    auto reset_counts = [](){move_count = 0; copy_count = 0;}; \
    reset_counts()

#define ASSERT_MOVE_AND_COPY_COUNTS(m, c) \
    assert(move_count == m); \
    assert(copy_count == c)

#define ASSERT_VALUE(test_arg_object, v) \
    assert(test_arg_object.value == v)

//assert at end of macro is used to suppress unused variable warnings
#define CREATE_WRAPPERS_DETAIL(var, slim_var, test_case, test_case_id) \
auto var = factory::wrap(&test_case); \
auto slim_var = slim_factory::wrap<decltype(&test_case), &test_case>(); \
auto current_case = id::test_case_id; \
assert(current_case == id::test_case_id)

//assert at end of macro is used to suppress unused variable warnings
#define CREATE_WRAPPER(var, test_case, test_case_id) \
auto var = factory::wrap(&test_case); \
auto current_case = id::test_case_id; \
assert(current_case == id::test_case_id)

#define CREATE_WRAPPERS(var, slim_var, test_case) \
    CREATE_WRAPPERS_DETAIL(var, slim_var, test_case, test_case)

#define STATIC_CHECKS_SINGLE(f, expected_ret, expected_type) \
    static_assert(std::is_same<decltype(f)::return_type, expected_ret>::value, "unexpected return_type"); \
    static_assert(std::is_same<decltype(f)::type, expected_type>::value, "unexpected emulated function type")

#define STATIC_CHECKS(f, s, expected_ret, expected_type) \
    STATIC_CHECKS_SINGLE(f, expected_ret, expected_type); \
    STATIC_CHECKS_SINGLE(s, expected_ret, expected_type)

using factory = clbl::function_ptr_wrapper_factory;
using slim_factory = typename factory::slim;

enum class id {
    case1,
    case2,
    case3,
    case4,
    case5,
    case6,
    case7,
    case8
};

void case1() {}
id case2() { return id::case2; }
id case3(int) { return id::case3; }

template<typename T>
id case4(T i) { i++; return id::case4; }

template<typename T>
id case5(T) { return id::case5; }

int main() {
    {
        CREATE_WRAPPERS(f, s, case1);
        STATIC_CHECKS(f, s, void, void(void));
        f();
        s();
    }{
        CREATE_WRAPPERS(f, s, case2);
        STATIC_CHECKS(f, s, id, id(void));
        assert(f() == current_case);
        assert(s() == current_case);
    }{
        CREATE_WRAPPERS(f, s, case3);
        STATIC_CHECKS(f, s, id, id(int));
        assert(f(1) == current_case);
        assert(s(1) == current_case);
    }{
        DEFINE_TEST_ARG_STRUCT(foo);
        CREATE_WRAPPER(f, case4<foo>, case4);
        STATIC_CHECKS_SINGLE(f, id, id(foo));
        auto test_arg = foo{};
        assert(f(test_arg) == current_case);
        ASSERT_MOVE_AND_COPY_COUNTS(0, 1);
        ASSERT_VALUE(test_arg, 0);
        reset_counts();
        assert(f(foo{}) == current_case);
        ASSERT_MOVE_AND_COPY_COUNTS(1, 0);
    }{
        DEFINE_TEST_ARG_STRUCT(foo);
        CREATE_WRAPPER(f, case4<foo&>, case4);
        STATIC_CHECKS_SINGLE(f, id, id(foo&));
        //should cause compiler error: 
        //assert(f(1) == current_case);
        //should cause compiler error: 
        //assert(s(1) == current_case);
        auto test_arg = foo{};
        assert(f(test_arg) == current_case);
        ASSERT_MOVE_AND_COPY_COUNTS(0, 0);
        ASSERT_VALUE(test_arg, 1);
    }{
        DEFINE_TEST_ARG_STRUCT(foo);
        CREATE_WRAPPER(f, case4<volatile foo&>, case4);
        STATIC_CHECKS_SINGLE(f, id, id(volatile foo&));
        //should cause compiler error: 
        //assert(f(1) == current_case);
        //should cause compiler error: 
        //assert(s(1) == current_case);
        volatile auto test_arg = foo{};
        assert(f(test_arg) == current_case);
        ASSERT_MOVE_AND_COPY_COUNTS(0, 0);
        ASSERT_VALUE(test_arg, 1);
    }{
        DEFINE_TEST_ARG_STRUCT(foo);
        CREATE_WRAPPER(f, case4<foo&&>, case4);
        STATIC_CHECKS_SINGLE(f, id, id(foo&&));
        assert(f(foo{}) == current_case);
        ASSERT_MOVE_AND_COPY_COUNTS(0, 0);
    }{
        DEFINE_TEST_ARG_STRUCT(foo);
        CREATE_WRAPPER(f, case5<const foo&>, case5);
        STATIC_CHECKS_SINGLE(f, id, id(const foo&));
        assert(f(foo{}) == current_case);
        ASSERT_MOVE_AND_COPY_COUNTS(0, 0);
        auto test_arg = foo{};
        assert(f(test_arg) == current_case);
        ASSERT_MOVE_AND_COPY_COUNTS(0, 0);
        const auto& test_arg_ref = test_arg;
        assert(f(test_arg_ref) == current_case);
        ASSERT_MOVE_AND_COPY_COUNTS(0, 0);
    }{
        DEFINE_TEST_ARG_STRUCT(foo);
        CREATE_WRAPPER(f, case5<const volatile foo&>, case5);
        STATIC_CHECKS_SINGLE(f, id, id(const volatile foo&));
        const volatile auto test_arg = foo{};
        auto& test_arg_ref = test_arg;
        assert(f(test_arg_ref) == current_case);
        ASSERT_MOVE_AND_COPY_COUNTS(0, 0);
    }

    return 0;
}