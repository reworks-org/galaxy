///
/// EntityEditor.hpp
/// supercluster
///
/// Refer to LICENSE.txt for more details.
///

#ifndef SUPERCLUSTER_EDITOR_PANELS_ENTITYEDITOR_HPP_
#define SUPERCLUSTER_EDITOR_PANELS_ENTITYEDITOR_HPP_

#include <optional>

#include <robin_hood.h>

#include <galaxy/core/Scene.hpp>

#include "editor/GLOperation.hpp"

using namespace galaxy;

namespace sc
{
	namespace panel
	{
		class EntityEditor final
		{
		public:
			EntityEditor();
			~EntityEditor();

			void render(OpenGLOperationStack& gl_operations);

			void set_scene(core::Scene* scene);
			void set_selected_entity(const std::optional<ecs::Entity>& entity);

		private:
			void render_components(const ecs::Entity entity, OpenGLOperationStack& gl_operations);

		private:
			core::Scene* m_cur_scene              = nullptr;
			std::optional<ecs::Entity> m_selected = std::nullopt;

			bool m_add_anim_popup = false;
		};
	} // namespace panel
} // namespace sc

#endif