/*!
@file

@copyright Barrett Adair 2015
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)

*/

#include <quali/quali_macros.hpp>
#include <cassert>
#include <string>
#include <functional>

using namespace quali;

struct foo {};

//GCC doesn't like this to be constexpr
template<typename T>
inline /*constexpr*/ std::true_type
is_dereferenceable_object(dereferenceable_object<T&&>) { return{}; }

inline /*constexpr*/ std::false_type
is_dereferenceable_object(...) { return{}; }

int main() {

    auto f = foo{};

    static_assert(decltype(is_dereferenceable_object(&f))::value, "");
    static_assert(!decltype(is_dereferenceable_object(f))::value, "");
    static_assert(!decltype(is_dereferenceable_object(""))::value, "");
    static_assert(std::is_same<dereferenceable_object<foo*>, foo*>::value, "");

    return 0;
}