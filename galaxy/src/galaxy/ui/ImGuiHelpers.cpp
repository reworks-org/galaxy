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
#include "galaxy/graphics/Texture.hpp"
#include "galaxy/platform/Pragma.hpp"

#include "ImGuiHelpers.hpp"

#ifdef GALAXY_WIN_PLATFORM
#pragma warning(push)
#pragma warning(disable : 4312)
#endif

constexpr const ImVec2 mid = {0.5f, 0.5f};
robin_hood::unordered_flat_map<const char*, bool> popup_state;

namespace galaxy
{
	namespace ui
	{
		ImGuiIO& imgui_init_context() noexcept
		{
			popup_state.clear();

			// clang-format off
			IMGUI_CHECKVERSION();
			ImGui::CreateContext();
			ImGuiIO& io = ImGui::GetIO(); (void)io;
			io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
			io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
            io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;
			io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;
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

			ImGui::SetColorEditOptions(ImGuiColorEditFlags_Uint8 | ImGuiColorEditFlags_DisplayRGB | ImGuiColorEditFlags_InputRGB |
									   ImGuiColorEditFlags_PickerHueBar | ImGuiColorEditFlags_AlphaBar);

			return io;
		}

		void imgui_new_frame() noexcept
		{
			ImGui_ImplOpenGL3_NewFrame();
			ImGui_ImplGlfw_NewFrame();
			ImGui::NewFrame();

			const ImGuiViewport* imgui_viewport = ImGui::GetMainViewport();
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
			// For this specific demo app we could also call glfwMakeContextCurrent(window) directly)
			const auto& io = ImGui::GetIO();
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
			popup_state.clear();

			ImGui_ImplOpenGL3_Shutdown();
			ImGui_ImplGlfw_Shutdown();
			ImGui::DestroyContext();
		}

		void imgui_open_confirm(const char* popup) noexcept
		{
			popup_state[popup] = true;
		}

		void imgui_confirm(const char* popup, const std::function<void(void)>& yes, const std::function<void(void)>& no) noexcept
		{
			if (popup_state[popup])
			{
				ImGui::OpenPopup(popup);
				imgui_center_next_window();

				popup_state[popup] = false;
			}

			if (ImGui::BeginPopup(popup))
			{
				ImGui::Text("Are you sure you want to proceed?");

				ImGui::Separator();
				ImGui::Spacing();

				if (ImGui::Button("Yes"))
				{
					if (yes)
					{
						yes();
					}

					ImGui::CloseCurrentPopup();
				}

				ImGui::SameLine();

				if (ImGui::Button("No"))
				{
					if (no)
					{
						no();
					}

					ImGui::CloseCurrentPopup();
				}

				ImGui::EndPopup();
			}
		}

		void imgui_center_next_window() noexcept
		{
			const auto center = ImGui::GetMainViewport()->GetCenter();
			ImGui::SetNextWindowPos(center, ImGuiCond_Appearing, mid);
		}

		bool imgui_shortcut(ImGuiKeyChord mods, ImGuiKey key, ImGuiFocusedFlags flag) noexcept
		{
			if (ImGui::GetIO().KeyMods != mods)
			{
				return false;
			}
			else if (!ImGui::IsWindowFocused(flag))
			{
				return false;
			}
			else
			{
				return ImGui::IsKeyPressed(key, false);
			}
		}

		bool imgui_imagebutton(const graphics::Texture& texture, const ImVec2& size) noexcept
		{
			std::string id = std::to_string(texture.handle()) + std::to_string(texture.get_width()) + std::to_string(texture.get_height());
			return ImGui::ImageButton(id.c_str(), reinterpret_cast<void*>(texture.handle()), size, {0, 1}, {1, 0});
		}
	} // namespace ui
} // namespace galaxy

#ifdef GALAXY_WIN_PLATFORM
#pragma warning(pop)
#endif