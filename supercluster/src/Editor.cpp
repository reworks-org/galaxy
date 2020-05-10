///
/// Editor.cpp
/// supercluster
///
/// Refer to LICENSE.txt for more details.
///

#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

#include <qs/core/Window.hpp>
#include <galaxy/core/ServiceLocator.hpp>

#include "Editor.hpp"

namespace sc
{
	Editor::Editor() noexcept
		:m_name("Editor"), m_open(true)
	{
		IMGUI_CHECKVERSION();
		ImGui::CreateContext();
		ImGuiIO& io = ImGui::GetIO(); (void)io;
		ImGui::StyleColorsDark();

		auto* window = SL_HANDLE.window();
		ImGui_ImplGlfw_InitForOpenGL(window->getWindow(), true);
		ImGui_ImplOpenGL3_Init("#version 450 core");
	}

	Editor::~Editor() noexcept
	{
	}

	void Editor::onPush() noexcept
	{
	}

	void Editor::onPop() noexcept
	{
		ImGui_ImplOpenGL3_Shutdown();
		ImGui_ImplGlfw_Shutdown();
		ImGui::DestroyContext();
	}

	void Editor::events() noexcept
	{
		auto* window = SL_HANDLE.window();

		int esc = glfwGetKey(window->getWindow(), GLFW_KEY_ESCAPE);
		if (esc == GLFW_PRESS)
		{
			window->close();
		}
	}

	void Editor::update(protostar::ProtectedDouble* deltaTime) noexcept
	{
	}

	void Editor::render() noexcept
	{
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();

		ImGui::Begin(m_name.c_str(), &m_open, ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_MenuBar);









		ImGui::Button("push");

		ImGui::End();

		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
	}
}