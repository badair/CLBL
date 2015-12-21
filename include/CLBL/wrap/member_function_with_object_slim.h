#ifndef CLBL_MEMBER_FUNCTION_WITH_OBJECT_SLIM_H
#define CLBL_MEMBER_FUNCTION_WITH_OBJECT_SLIM_H

#include "CLBL/utility.h"
#include "CLBL/tags.h"
#include "CLBL/qualify_flags.h"
#include "CLBL/member_function_decay.h"
#include "CLBL/wrappers/pmf_wrapper_slim.h"

namespace clbl {

    //"slim" version takes the member_fn as a template argument
    struct member_function_with_object_slim {

        template<qualify_flags Flags = qflags::default_, typename TMemberFnPtr, TMemberFnPtr Pmf, typename T>
        static inline constexpr auto
            wrap(T&& t) {
            constexpr auto cv_qualifiers = cv<no_ref<T> > | Flags;
            using decayed_fn = member_function_decay<no_ref<TMemberFnPtr> >;
            using wrapper = pmf_wrapper_slim<member_function_with_object_slim, 
                                cv_qualifiers, no_ref<T>, TMemberFnPtr, Pmf, decayed_fn>;
            return wrapper{ std::forward<T>(t) };
        }

        template<qualify_flags Flags, typename Invocation>
        static inline constexpr auto
            wrap_data(Invocation&& data) {
            return wrap<Flags, decltype(no_ref<Invocation>::pmf), no_ref<Invocation>::pmf>(data.object);
        }
    };
}

#endif