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

#include "sl/core/StateManager.hpp"
#include "sl/core/ServiceLocator.hpp"
#include "sl/libs/imgui/imgui_impl_a5.h"
#include "sl/libs/imgui/imguinodegrapheditor.h"

#include "DebugInterface.hpp"

namespace sl
{
	DebugInterface::DebugInterface(ALLEGRO_DISPLAY* display, bool isDisabled)
	:m_reloadState(nullptr), m_disabled(isDisabled)
	{
		ImGui::CreateContext();
		ImGuiIO& io = ImGui::GetIO(); (void)io;
		ImGui_ImplA5_Init(display);

		ImGui::StyleColorsDark();
	}

	DebugInterface::~DebugInterface()
	{
		ImGui_ImplA5_Shutdown();
		ImGui::DestroyContext();
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
			ImGui_ImplA5_RenderDrawData(ImGui::GetDrawData());
		}
	}

	void DebugInterface::displayMenu()
	{
		ImGui::Begin("Debug Menu", (bool*)false, ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoSavedSettings);

		if (ImGui::Button("Reset game"))
		{
			m_reloadFunc();
			Locator::m_stateManager->reloadState(m_reloadState);
		}

		if(ImGui::Button("Open Console"))
		{
			// Open game console
		}

		// Console should take commands for geme

		ImGui::End();
	}

	void DebugInterface::setReloadState(std::shared_ptr<BaseState> s, std::function<void(void)> func)
	{
		m_reloadState = s;
		m_reloadFunc = func;
	}
}