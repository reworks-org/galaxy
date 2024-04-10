///
/// RenderSystem.cpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#include "galaxy/components/RenderCommand.hpp"
#include "galaxy/components/Transform.hpp"
#include "galaxy/flags/Disabled.hpp"
#include "galaxy/graphics/Renderer.hpp"
#include "galaxy/scene/Scene.hpp"

#include "RenderSystem.hpp"

namespace galaxy
{
	namespace systems
	{
		RenderSystem::RenderSystem()
		{
		}

		RenderSystem::~RenderSystem()
		{
		}

		void RenderSystem::update(scene::Scene* scene)
		{
			graphics::Renderer::ref().flush();

			const auto cmds = scene->m_registry.m_entt.group<components::RenderCommand>(entt::get<components::Transform>, entt::exclude<flags::Disabled>);

			for (auto&& [entity, cmd, tf] : cmds.each())
			{
				cmd.m_command.uniforms.transform = tf.m_tf.get_transform();
				graphics::Renderer::ref().submit_cmd(cmd.m_command);
			}
		}
	} // namespace systems
} // namespace galaxy
