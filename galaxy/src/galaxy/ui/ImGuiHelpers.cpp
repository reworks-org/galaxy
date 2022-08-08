///
/// ImGuiHelpers.cpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#include <glfw/glfw3.h>
#include <imgui_impl_opengl3.h>
#include <imgui_impl_glfw.h>

#include "galaxy/core/ServiceLocator.hpp"
#include "galaxy/core/Window.hpp"

#include "ImGuiHelpers.hpp"

namespace galaxy
{
	namespace ui
	{
		ImGuiIO& imgui_init_context() noexcept
		{
			// clang-format off
			IMGUI_CHECKVERSION();
			ImGui::CreateContext();
			ImGuiIO& io = ImGui::GetIO(); (void)io;
			io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
			//io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
            //io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;
			//io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;
			io.ConfigDockingAlwaysTabBar = true;
			// clang-format on

			// When viewports are enabled we tweak WindowRounding/WindowBg so platform windows can look identical to regular ones.
			ImGuiStyle& style = ImGui::GetStyle();
			if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
			{
				style.WindowRounding              = 0.0f;
				style.Colors[ImGuiCol_WindowBg].w = 1.0f;
			}

			auto& window = core::ServiceLocator<core::Window>::ref();
			ImGui_ImplGlfw_InitForOpenGL(window.handle(), true);
			ImGui_ImplOpenGL3_Init("#version 460 core");

			return io;
		}

		void imgui_new_frame() noexcept
		{
			ImGui_ImplOpenGL3_NewFrame();
			ImGui_ImplGlfw_NewFrame();
			ImGui::NewFrame();

			auto* const imgui_viewport = ImGui::GetMainViewport();
			ImGui::SetNextWindowPos(imgui_viewport->WorkPos);
			ImGui::SetNextWindowSize(imgui_viewport->WorkSize);
			ImGui::SetNextWindowViewport(imgui_viewport->ID);
		}

		void imgui_prerender() noexcept
		{
			ImGui::Render();
		}

		void imgui_render() noexcept
		{
			ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

			// Update and Render additional Platform Windows
			// (Platform functions may change the current OpenGL context, so we save/restore it to make it easier to paste this code elsewhere.
			//  For this specific demo app we could also call glfwMakeContextCurrent(window) directly)
			ImGuiIO& io = ImGui::GetIO();
			if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
			{
				GLFWwindow* backup_current_context = glfwGetCurrentContext();
				ImGui::UpdatePlatformWindows();
				ImGui::RenderPlatformWindowsDefault();
				glfwMakeContextCurrent(backup_current_context);
			}
		}

		void imgui_destroy_context() noexcept
		{
			ImGui_ImplOpenGL3_Shutdown();
			ImGui_ImplGlfw_Shutdown();
			ImGui::DestroyContext();
		}
	} // namespace ui
} // namespace galaxy