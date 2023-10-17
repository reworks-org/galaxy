///
/// UpdateStack.hpp
/// supercluster
///
/// Refer to LICENSE.txt for more details.
///

#ifndef SUPERCLUSTER_EDITOR_UPDATESTACK_HPP_
#define SUPERCLUSTER_EDITOR_UPDATESTACK_HPP_

#include <functional>

#include <galaxy/meta/Memory.hpp>

using namespace galaxy;

namespace sc
{
	using UpdateStackCallback = std::function<void(void)>;
	using UpdateStack         = meta::vector<UpdateStackCallback>;
} // namespace sc

#endif
