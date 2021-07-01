///
/// ScenePanel.hpp
/// supercluster
///
/// Refer to LICENSE.txt for more details.
///

#ifndef SUPERCLUSTER_EDITOR_PANELS_SCENEPANEL_HPP_
#define SUPERCLUSTER_EDITOR_PANELS_SCENEPANEL_HPP_

#include <galaxy/core/SceneStack.hpp>

#include "editor/GLOperation.hpp"

using namespace galaxy;

namespace sc
{
	namespace panel
	{
		class ScenePanel final
		{
		public:
			void render(core::SceneStack& scene_stack, OpenGLOperationStack& gl_operations);

		private:
			std::string m_selected = "";
		};
	} // namespace panel
} // namespace sc

#endif