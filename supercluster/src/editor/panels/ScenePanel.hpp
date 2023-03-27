///
/// ScenePanel.hpp
/// supercluster
///
/// Refer to LICENSE.txt for more details.
///

#ifndef SUPERCLUSTER_EDITOR_PANELS_SCENEPANEL_HPP_
#define SUPERCLUSTER_EDITOR_PANELS_SCENEPANEL_HPP_

#include <galaxy/scene/SceneManager.hpp>

#include "editor/Selected.hpp"
#include "editor/UpdateStack.hpp"

using namespace galaxy;

namespace sc
{
	namespace panel
	{
		class ScenePanel final
		{
		public:
			ScenePanel()  = default;
			~ScenePanel() = default;

			void render(scene::SceneManager& sm, Selected& selected, UpdateStack& updates);

		private:
			std::string m_selected;
			std::string m_selected_layer;
		};
	} // namespace panel
} // namespace sc

#endif