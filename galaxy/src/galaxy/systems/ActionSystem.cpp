///
/// ActionSystem.cpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#include <sol/sol.hpp>

#include "galaxy/components/Actions.hpp"
#include "galaxy/core/ServiceLocator.hpp"
#include "galaxy/core/Window.hpp"

#include "ActionSystem.hpp"

namespace galaxy
{
	namespace systems
	{
		ActionSystem::ActionSystem() noexcept
		{
		}

		ActionSystem::~ActionSystem() noexcept
		{
		}

		void ActionSystem::update(core::Scene2D* scene)
		{
			// clang-format off
			scene->m_world.operate<components::Actions>([&](const ecs::Entity entity, components::Actions* actions) {
				for (const auto& [key, func] : actions->m_key_actions)
				{
					if (SL_HANDLE.window()->key_down(key))
					{
						(*SL_HANDLE.lua())[func](entity);
					}
				}

				for (const auto& [mouse, func] : actions->m_mouse_actions)
				{
					if (SL_HANDLE.window()->mouse_button_pressed(mouse))
					{
						(*SL_HANDLE.lua())[func](entity);
					}
				}
			});
			// clang-format on
		}
	} // namespace systems
} // namespace galaxy