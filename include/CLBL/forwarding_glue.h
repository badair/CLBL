#ifndef CLBL_FORWARDING_GLUE_H
#define CLBL_FORWARDING_GLUE_H

#include "CLBL/utility.h"
#include "CLBL/forward.h"

namespace clbl {

    namespace detail {
        template<typename, typename Bad> 
        struct fowarding_glue_t { static_assert("Not a clbl callable wrapper."); };

        template<typename Callable, typename... Args>
        struct fowarding_glue_t<Callable, hana::tuple<Args...> > {
            using ret = typename no_ref<Callable>::return_t;
            using type = ret(forward<Args>...);
        };
    }

    template<typename Callable>
    using fowarding_glue = typename detail::fowarding_glue_t<Callable, args<Callable> >::type;
}
#endif