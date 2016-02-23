
#ifndef CLBL_CORE_H
#define CLBL_CORE_H

#include <CLBL/cv_of.hpp>
#include <CLBL/tags.hpp>
#include <CLBL/forward.hpp>
#include <CLBL/type_traits.hpp>
#include <CLBL/harden_cast.hpp>
#include <CLBL/constraints.hpp>
#include <CLBL/generalized_object.hpp>
#include <CLBL/internal/dispatch/pmf.hpp>
#include <CLBL/internal/dispatch/function_ptr.hpp>
#include <CLBL/internal/dispatch/function_reference.hpp>
#include <CLBL/internal/dispatch/function_object.hpp>
#include <CLBL/internal/partial_wrapper.hpp>
#include <CLBL/internal/bind/binding_wrapper.hpp>

namespace clbl { namespace internal {

template<typename Base>
class callable_wrapper;

}}

#include <CLBL/internal/function_wrapper_base.hpp>
#include <CLBL/internal/pmf_wrapper_base.hpp>
#include <CLBL/internal/bound_pmf_wrapper_base.hpp>
#include <CLBL/internal/function_object_wrapper_base.hpp>
#include <CLBL/internal/dispatch/dispatcher.hpp>
#endif