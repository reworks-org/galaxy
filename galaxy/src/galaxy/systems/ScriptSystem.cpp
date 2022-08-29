///
/// ScriptSystem.cpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#include "galaxy/components/Script.hpp"
#include "galaxy/state/Layer.hpp"
#include "galaxy/utils/Globals.hpp"

#include "ScriptSystem.hpp"

namespace galaxy
{
	namespace systems
	{
		ScriptSystem::ScriptSystem() noexcept
		{
		}

		ScriptSystem::~ScriptSystem() noexcept
		{
		}

		void ScriptSystem::update(state::Layer* layer)
		{
			const auto view = layer->world().m_registry.view<components::Script>();
			for (auto&& [entity, script] : view.each())
			{
				if (script.m_update.valid())
				{
					script.m_update(script.m_self);
				}
			}
		}
	} // namespace systems
} // namespace galaxy