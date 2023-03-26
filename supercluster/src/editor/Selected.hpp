///
/// Selected.hpp
/// supercluster
///
/// Refer to LICENSE.txt for more details.
///

#ifndef SUPERCLUSTER_EDITOR_SELECTED_HPP_
#define SUPERCLUSTER_EDITOR_SELECTED_HPP_

#include <galaxy/core/World.hpp>

using namespace galaxy;

namespace sc
{
	class Selected
	{
	public:
		Selected() noexcept;
		~Selected() noexcept;

	public:
		entt::entity m_selected;
		core::World* m_world;
	};
} // namespace sc

#endif