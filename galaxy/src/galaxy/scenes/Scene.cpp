///
/// Scene.cpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#include "galaxy/core/ServiceLocator.hpp"

#include "Scene.hpp"

namespace galaxy
{
	namespace scene
	{
		Scene::Scene(std::string_view name) noexcept
		    : Serializable {this}, m_name {name}
		{
			m_gui_theme.m_camera.create(0.0f, (float)SL_HANDLE.window()->get_width(), (float)SL_HANDLE.window()->get_height(), 0.0f);
			m_gui.set_theme(&m_gui_theme);

			m_dispatcher.subscribe<events::MouseMoved>(m_gui);
			m_dispatcher.subscribe<events::MousePressed>(m_gui);
			m_dispatcher.subscribe<events::MouseReleased>(m_gui);
			m_dispatcher.subscribe<events::KeyDown>(m_gui);
		}
	} // namespace scene
} // namespace galaxy