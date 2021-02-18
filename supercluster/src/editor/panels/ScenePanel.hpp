///
/// ScenePanel.hpp
/// supercluster
///
/// Refer to LICENSE.txt for more details.
///

#ifndef SUPERCLUSTER_EDITOR_PANELS_SCENEPANEL_HPP_
#define SUPERCLUSTER_EDITOR_PANELS_SCENEPANEL_HPP_

#include <robin_hood.h>

#include <galaxy/core/Scene.hpp>

using namespace galaxy;

namespace sc
{
	using Scenemap = robin_hood::unordered_flat_map<std::string, std::unique_ptr<core::Scene>>;

	namespace panel
	{
		class ScenePanel final
		{
		public:
			void render(Scenemap& scene_map);

		private:
			std::string m_selected = "";
		};
	} // namespace panel
} // namespace sc

#endif