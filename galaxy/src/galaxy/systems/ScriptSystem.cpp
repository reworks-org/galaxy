///
/// ScriptSystem.cpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#include "galaxy/components/Script.hpp"
#include "galaxy/components/Flag.hpp"
#include "galaxy/flags/Enabled.hpp"
#include "galaxy/scene/Scene.hpp"
#include "galaxy/utils/Globals.hpp"

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

		void ScriptSystem::update(scene::Scene* scene)
		{
			const auto view = scene->m_world.m_registry.view<components::Script, components::Flag>();
			for (auto&& [entity, script, flag] : view.each())
			{
				if (flag.is_flag_set<flags::Enabled>() && script.m_update.valid())
				{
					script.m_update(script.m_self);
				}
			}
		}
	} // namespace systems
} // namespace galaxy