///
/// UI.cpp
/// celestial
///
/// Refer to LICENSE.txt for more details.
///

#include "UI.hpp"

namespace celestial
{
	UI::UI() noexcept
	{
	}

	UI::~UI() noexcept
	{
		// Ensure unique_ptr's are deconstructed and detroyed.
		m_panels.clear();
	}
	
	void UI::update(const double dt)
	{
		// Iterate over panels, update, and call each widgets update function.
		for (auto& panel : m_panels)
		{
			panel->update(dt);
		}
	}

	void UI::render(celestial::Renderer* renderer)
	{
		// Iterate over panels, render, and call each widgets render function.
		for (auto& panel : m_panels)
		{
			panel->render(renderer);
		}
	}
}