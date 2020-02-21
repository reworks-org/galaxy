///
/// ImGuiLayer.cpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#include <SFML/Graphics/RenderWindow.hpp>

#include "galaxy/core/ServiceLocator.hpp"

#include "ImGuiLayer.hpp"

///
/// Core namespace.
///
namespace galaxy
{
	ImGuiLayer::ImGuiLayer(bool* restart) noexcept
		:Layer("ImGuiLayer"), m_editor(galaxy::ServiceLocator::get().window()), m_restart(restart)
	{
	}

	ImGuiLayer::~ImGuiLayer()
	{
	}

	void ImGuiLayer::event(const sf::Event& event)
	{
		m_editor.event(event);
	}

	void ImGuiLayer::update(sf::Time& dt)
	{
	}

	void ImGuiLayer::render() noexcept
	{
		m_editor.display(m_restart);
		m_editor.render();
	}
}