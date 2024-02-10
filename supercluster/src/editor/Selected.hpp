///
/// Selected.hpp
/// supercluster
///
/// Refer to LICENSE.txt for more details.
///

#ifndef SUPERCLUSTER_EDITOR_SELECTED_HPP_
#define SUPERCLUSTER_EDITOR_SELECTED_HPP_

#include <galaxy/scene/Scene.hpp>

using namespace galaxy;

namespace sc
{
	class Selected
	{
	  public:
		Selected();
		~Selected();

	  public:
		entt::entity  m_selected;
		scene::Scene* m_scene;
	};
} // namespace sc

#endif
