///
/// DebugInterface.cpp
/// starlight
///
/// Created by reworks on 20/04/2017.
/// MIT License.
/// Refer to LICENSE.txt for more details.
///

#include <allegro5/events.h>
#include <allegro5/display.h>

#include "imgui/imgui_impl_a5.h"

#include "DebugInterface.hpp"

namespace sl
{
	DebugInterface::DebugInterface(ALLEGRO_DISPLAY* display, bool isDisabled)
	:m_reloadState(nullptr), m_disabled(isDisabled)
	{
		ImGui_ImplA5_Init(display);
	}

	DebugInterface::~DebugInterface()
	{
		ImGui_ImplA5_Shutdown();
	}

	void DebugInterface::event(ALLEGRO_EVENT* event)
	{
		if (!m_disabled)
		{
			ImGui_ImplA5_ProcessEvent(event);
		}
	}

	void DebugInterface::newFrame()
	{
		if (!m_disabled)
		{
			ImGui_ImplA5_NewFrame();
		}
	}

	void DebugInterface::render()
	{
		if (!m_disabled)
		{
			ImGui::Render();
		}
	}

	void DebugInterface::displayMenu()
	{
		ImGui::Begin("Debug Menu", (bool*)false, ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoSavedSettings);

		ImGui::End();
	}
}