///
/// DebugInterface.cpp
/// starlight
///
/// Created by reworks on 20/04/2017.
/// MIT License.
/// Refer to LICENSE.txt for more details.
///

#ifndef NDEBUG

#include <allegro5/events.h>
#include <allegro5/display.h>

#include "sl/graphics/Window.hpp"
#include "sl/core/StateManager.hpp"
#include "sl/core/ServiceLocator.hpp"
#include "sl/libs/imgui/imgui_impl_a5.h"

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

	void DebugInterface::setReloadState(std::shared_ptr<BaseState> s, std::function<void(void)> func)
	{
		m_reloadState = s;
		m_reloadFunc = func;
	}

	void DebugInterface::displayMenu()
	{
		ImGui::Begin("Debug Menu", (bool*)false, ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_MenuBar);

		if (ImGui::BeginMenuBar())
		{
			if (ImGui::BeginMenu("File"))
			{
				if (ImGui::MenuItem("Reset State"))
				{
					m_reloadFunc();
					Locator::stateManager->reloadState(m_reloadState);
				}

				if (ImGui::MenuItem("Close Game"))
				{
					Locator::window->close();
				}

				ImGui::EndMenu();
			}

			if (ImGui::BeginMenu("Create"))
			{
				if (ImGui::MenuItem("New Entity(s)"))
				{
					showCreateEntityWindow();
				}

				ImGui::EndMenu();
			}

			if (ImGui::BeginMenu("Tools"))
			{
				if (ImGui::MenuItem("Show Console"))
				{

				}

				ImGui::EndMenu();
			}
			
			ImGui::EndMenuBar();
		}

		ImGui::End();
	}

	void DebugInterface::showCreateEntityWindow()
	{

	}
}

#endif