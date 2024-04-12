///
/// ScriptSystem.cpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#include "galaxy/components/Script.hpp"
#include "galaxy/flags/Disabled.hpp"
#include "galaxy/scene/Scene.hpp"

#include "ScriptSystem.hpp"

namespace galaxy
{
	namespace systems
	{
		ScriptSystem::ScriptSystem()
		{
		}

		ScriptSystem::~ScriptSystem()
		{
		}

		void ScriptSystem::update(entt::registry& registry)
		{
			const auto group = registry.view<components::Script>(entt::exclude<flags::Disabled>);
			for (auto&& [entity, script] : group.each())
			{
				if (script.m_update.valid())
				{
					script.m_update(script.m_self);
				}
			}
		}
	} // namespace systems
} // namespace galaxy
