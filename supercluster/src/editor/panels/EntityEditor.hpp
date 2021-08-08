///
/// EntityEditor.hpp
/// supercluster
///
/// Refer to LICENSE.txt for more details.
///

#ifndef SUPERCLUSTER_EDITOR_PANELS_ENTITYEDITOR_HPP_
#define SUPERCLUSTER_EDITOR_PANELS_ENTITYEDITOR_HPP_

#include <galaxy/core/Scene2D.hpp>

#include "editor/GLOperation.hpp"

using namespace galaxy;

namespace sc
{
	namespace panel
	{
		class EntityEditor final
		{
		public:
			EntityEditor() noexcept;
			~EntityEditor() noexcept;

			void render(core::Scene2D* top_scene, OpenGLOperationStack& gl_operations);

		private:
			void render_components(const ecs::Entity entity, OpenGLOperationStack& gl_operations);
		};
	} // namespace panel
} // namespace sc

#endif