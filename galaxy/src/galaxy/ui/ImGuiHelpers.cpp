///
/// ImGuiHelpers.cpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#include <ankerl/unordered_dense.h>
#include <glad/glad.h>
#include <imgui/imgui_impl_glfw.h>
#include <imgui/imgui_impl_opengl3.h>
#include <imgui/imnotify/ImGuiNotify.hpp>

#include "galaxy/core/ServiceLocator.hpp"
#include "galaxy/core/Window.hpp"
#include "galaxy/graphics/gl/Texture2D.hpp"
#include "galaxy/graphics/RenderTexture.hpp"
#include "galaxy/platform/Pragma.hpp"

#include "ImGuiHelpers.hpp"

#ifdef GALAXY_WIN_PLATFORM
#pragma warning(push)
#pragma warning(disable : 4312)
#endif

constexpr const ImVec2                          mid = {0.5f, 0.5f};
ankerl::unordered_dense::map<const char*, bool> popup_state;

namespace galaxy
{
	void* ImGuiMemAllocFunc(size_t sz, void* user_data)
	{
		return mi_malloc(sz);
	}

	void ImGuiMemFreeFunc(void* ptr, void* user_data)
	{
		mi_free(ptr);
	}

	namespace ui
	{
		ImGuiIO& imgui_init_context()
		{
			popup_state.clear();

			// clang-format off
			IMGUI_CHECKVERSION();
			ImGui::SetAllocatorFunctions(ImGuiMemAllocFunc, ImGuiMemFreeFunc, nullptr);
			ImGui::CreateContext();
			ImGuiIO& io = ImGui::GetIO(); (void)io;
			// clang-format on

			io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
			io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;
			io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
			io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;

			io.ConfigInputTextCursorBlink = true;
			io.ConfigDockingWithShift     = true;
			io.ConfigDockingAlwaysTabBar  = true;
			io.MouseDrawCursor            = false;

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

		void imgui_new_frame()
		{
			ImGui_ImplOpenGL3_NewFrame();
			ImGui_ImplGlfw_NewFrame();
			ImGui::NewFrame();

			const ImGuiViewport* imgui_viewport = ImGui::GetMainViewport();
			ImGui::SetNextWindowPos(imgui_viewport->WorkPos);
			ImGui::SetNextWindowSize(imgui_viewport->WorkSize);
			ImGui::SetNextWindowViewport(imgui_viewport->ID);
		}

		void imgui_render()
		{
			ImGui::Render();
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

		void imgui_destroy_context()
		{
			popup_state.clear();

			ImGui_ImplOpenGL3_Shutdown();
			ImGui_ImplGlfw_Shutdown();
			ImGui::DestroyContext();
		}

		void imgui_open_confirm(const char* popup)
		{
			popup_state[popup] = true;
		}

		void imgui_confirm(const char* popup, const std::function<void(void)>& yes, const std::function<void(void)>& no)
		{
			if (popup_state[popup])
			{
				ImGui::OpenPopup(popup);
				imgui_center_next_window();

				popup_state[popup] = false;
			}

			if (ImGui::BeginPopup(popup))
			{
				ImGui::Text("Are you sure you want to?");

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

		void draw_rows_background(int row_count, float line_height, float x1, float x2, float y_offset, ImU32 col_even, ImU32 col_odd)
		{
			// https://github.com/ocornut/imgui/issues/2668#issuecomment-1900456751

			ImDrawList* draw_list = ImGui::GetWindowDrawList();
			float       y0        = ImGui::GetCursorScreenPos().y + (float)(int)y_offset;

			const auto       pos = ImGui::GetCursorPos();
			ImGuiListClipper clipper;
			clipper.Begin(row_count, line_height);
			while (clipper.Step())
			{
				for (int row_n = clipper.DisplayStart; row_n < clipper.DisplayEnd; ++row_n)
				{
					ImU32 col = (row_n & 1) ? col_odd : col_even;
					if ((col & IM_COL32_A_MASK) == 0)
						continue;
					float y1 = y0 + (line_height * static_cast<float>(row_n));
					float y2 = y1 + line_height;
					draw_list->AddRectFilled(ImVec2(x1, y1), ImVec2(x2, y2), col);
				}
			}
			ImGui::SetCursorPos(pos);
		}

		void imgui_center_next_window()
		{
			const auto center = ImGui::GetMainViewport()->GetCenter();
			ImGui::SetNextWindowPos(center, ImGuiCond_Appearing, mid);
		}

		bool imgui_imagebutton(const graphics::Texture2D& texture, const ImVec2& size, const ImVec4& bg_col, const ImVec4& tint_col)
		{
			const auto id     = std::to_string(texture.id()) + std::to_string(texture.width()) + std::to_string(texture.height());
			const auto upcast = static_cast<std::uint64_t>(texture.id());

			return ImGui::ImageButton(id.c_str(), reinterpret_cast<void*>(upcast), size, {0, 1}, {1, 0}, bg_col, tint_col);
		}

		void imgui_image(const graphics::Texture2D& texture, const ImVec2& size)
		{
			const auto upcast = static_cast<std::uint64_t>(texture.id());
			ImGui::Image(reinterpret_cast<void*>(upcast), size, {0, 1}, {1, 0});
		}

		void imgui_image(const graphics::RenderTexture& texture, const ImVec2& size)
		{
			const auto upcast = static_cast<std::uint64_t>(texture.texture());
			ImGui::Image(reinterpret_cast<void*>(upcast), size, {0, 1}, {1, 0});
		}

		void imgui_notify_success(const char* msg)
		{
			ImGui::InsertNotification({ImGuiToastType::Warning, 2000, msg});
		}

		void imgui_notify_info(const char* msg)
		{
			ImGui::InsertNotification({ImGuiToastType::Info, 2000, msg});
		}

		void imgui_notify_warning(const char* msg)
		{
			ImGui::InsertNotification({ImGuiToastType::Warning, 2000, msg});
		}

		void imgui_notify_error(const char* msg)
		{
			ImGui::InsertNotification({ImGuiToastType::Error, 2000, msg});
		}

		bool imgui_glm_vec2(const char* label, glm::vec2& vec)
		{
			auto clicked = false;

			ImGui::PushID(label);

			ImGui::TextUnformatted(label);

			ImGui::SameLine();

			ImGui::SetNextItemWidth(150);
			clicked |= ImGui::InputFloat("X", &vec.x, 1.0f, 10.0f, "%.1f", ImGuiInputTextFlags_CharsNoBlank);

			ImGui::SameLine();

			ImGui::SetNextItemWidth(150);
			clicked |= ImGui::InputFloat("Y", &vec.y, 1.0f, 10.0f, "%.1f", ImGuiInputTextFlags_CharsNoBlank);

			ImGui::PopID();
			return clicked;
		}

		bool imgui_glm_vec3(const char* label, glm::vec3& vec)
		{
			auto clicked = false;

			ImGui::PushID(label);

			ImGui::TextUnformatted(label);

			ImGui::SameLine();

			ImGui::SetNextItemWidth(100);
			clicked |= ImGui::InputFloat("X", &vec.x, 1.0f, 10.0f, "%.1f", ImGuiInputTextFlags_CharsNoBlank);

			ImGui::SameLine();

			ImGui::SetNextItemWidth(100);
			clicked |= ImGui::InputFloat("Y", &vec.y, 1.0f, 10.0f, "%.1f", ImGuiInputTextFlags_CharsNoBlank);

			ImGui::SameLine();

			ImGui::SetNextItemWidth(100);
			clicked |= ImGui::InputFloat("Z", &vec.z, 1.0f, 10.0f, "%.1f", ImGuiInputTextFlags_CharsNoBlank);

			ImGui::PopID();
			return clicked;
		}

		bool imgui_glm_vec4(const char* label, glm::vec4& vec)
		{
			auto clicked = false;

			ImGui::PushID(label);

			ImGui::TextUnformatted(label);

			ImGui::SameLine();

			ImGui::SetNextItemWidth(75);
			clicked |= ImGui::InputFloat("X", &vec.x, 1.0f, 10.0f, "%.1f", ImGuiInputTextFlags_CharsNoBlank);

			ImGui::SameLine();

			ImGui::SetNextItemWidth(75);
			clicked |= ImGui::InputFloat("Y", &vec.y, 1.0f, 10.0f, "%.1f", ImGuiInputTextFlags_CharsNoBlank);

			ImGui::SameLine();

			ImGui::SetNextItemWidth(75);
			clicked |= ImGui::InputFloat("Z", &vec.z, 1.0f, 10.0f, "%.1f", ImGuiInputTextFlags_CharsNoBlank);

			ImGui::SameLine();

			ImGui::SetNextItemWidth(75);
			clicked |= ImGui::InputFloat("W", &vec.w, 1.0f, 10.0f, "%.1f", ImGuiInputTextFlags_CharsNoBlank);

			ImGui::PopID();
			return clicked;
		}
	} // namespace ui
} // namespace galaxy

#ifdef GALAXY_WIN_PLATFORM
#pragma warning(pop)
#endif
