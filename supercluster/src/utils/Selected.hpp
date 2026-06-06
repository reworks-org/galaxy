///
/// Selected.hpp
/// supercluster
///
/// Refer to LICENSE.txt for more details.
///

#ifndef SUPERCLUSTER_SELECTED_HPP_
#define SUPERCLUSTER_SELECTED_HPP_

#include <galaxy/scene/Scene.hpp>

using namespace galaxy;

namespace sc
{
	struct Selected
	{
		entt::entity  entity;
		scene::Scene* scene;
	};
} // namespace sc

#endif
