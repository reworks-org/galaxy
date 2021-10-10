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

			void render(core::Scene2D* top, OpenGLOperationStack& gl_operations);
			void set_selected_entity(std::optional<ecs::Entity> entity);

		private:
			void render_components(core::Scene2D* top, const ecs::Entity entity, OpenGLOperationStack& gl_operations);

		private:
			std::optional<ecs::Entity> m_selected;
			std::string                m_label;
		};
	} // namespace panel
} // namespace sc

#endif