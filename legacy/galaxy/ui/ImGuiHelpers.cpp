///
/// ImGuiHelpers.cpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

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
			io.ConfigDockingAlwaysTabBar = true;
			// clang-format on

			ImGui_ImplGlfw_InitForOpenGL(SL_HANDLE.window()->gl_window(), true);
			ImGui_ImplOpenGL3_Init("#version 450 core");

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

		void imgui_render() noexcept
		{
			ImGui::Render();
			ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
		}

		void imgui_destroy_context() noexcept
		{
			ImGui_ImplOpenGL3_Shutdown();
			ImGui_ImplGlfw_Shutdown();
			ImGui::DestroyContext();
		}
	} // namespace ui
} // namespace galaxy