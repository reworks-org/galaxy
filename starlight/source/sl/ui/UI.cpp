///
/// UI.cpp
/// starlight
///
/// Created by reworks on 14/09/2018.
/// MIT License.
/// Refer to LICENSE.txt for more details.
///

#include "UI.hpp"

namespace sl
{
	UI::~UI()
	{
		// Ensure unique_ptr's are deconstructed and detroyed.
		m_panels.clear();
	}

	void UI::render()
	{
		// Iterate over panels and call each widgets render function.
		for (auto& panel : m_panels)
		{
			panel->render();
		}
	}
}