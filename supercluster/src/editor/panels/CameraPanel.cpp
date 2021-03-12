///
/// CameraPanel.cpp
/// supercluster
///
/// Refer to LICENSE.txt for more details.
///

#include <imgui/imgui_stdlib.h>

#include "CameraPanel.hpp"

using namespace galaxy;

#define INPUT_WIDTH 150

namespace sc
{
	namespace panel
	{
		CameraPanel::~CameraPanel() noexcept
		{
			m_cur_instance = nullptr;
		}

		void CameraPanel::render()
		{
			if (ImGui::Begin("Camera Editor"))
			{
				auto& camera = m_cur_instance->get_stack().top()->camera();

				ImGui::Text("Move");

				float s_move_x = camera.get_pos().x;
				ImGui::SetNextItemWidth(INPUT_WIDTH);
				if (ImGui::InputFloat("X##01", &s_move_x, 1.0f, 10.0f, "%.1f", ImGuiInputTextFlags_CharsNoBlank | ImGuiInputTextFlags_EnterReturnsTrue))
				{
					camera.move_x(s_move_x);
				}

				ImGui::SameLine();

				float s_move_y = camera.get_pos().y;
				ImGui::SetNextItemWidth(INPUT_WIDTH);
				if (ImGui::InputFloat("Y##02", &s_move_y, 1.0f, 10.0f, "%.1f", ImGuiInputTextFlags_CharsNoBlank | ImGuiInputTextFlags_EnterReturnsTrue))
				{
					camera.move_y(s_move_y);
				}

				int s_speed = static_cast<int>(camera.get_speed());
				if (ImGui::InputInt("Speed##03", &s_speed, 1, 10, ImGuiInputTextFlags_CharsNoBlank | ImGuiInputTextFlags_EnterReturnsTrue))
				{
					camera.set_speed(s_speed);
				}

				float s_zoom = camera.get_scale();
				if (ImGui::SliderFloat("Scale##04", &s_zoom, 0.2, 10.0, "%.1f", ImGuiSliderFlags_AlwaysClamp | ImGuiSliderFlags_ClampOnInput))
				{
					camera.zoom(s_zoom);
				}

				ImGui::Text("Projection");

				float s_proj_x = camera.get_width();
				ImGui::SetNextItemWidth(INPUT_WIDTH);
				if (ImGui::InputFloat("X##05", &s_proj_x, 1.0f, 10.0f, "%.1f", ImGuiInputTextFlags_CharsNoBlank | ImGuiInputTextFlags_EnterReturnsTrue))
				{
					camera.set_width(s_proj_x);
				}

				ImGui::SameLine();

				float s_proj_y = camera.get_height();
				ImGui::SetNextItemWidth(INPUT_WIDTH);
				if (ImGui::InputFloat("Y##06", &s_proj_y, 1.0f, 10.0f, "%.1f", ImGuiInputTextFlags_CharsNoBlank | ImGuiInputTextFlags_EnterReturnsTrue))
				{
					camera.set_height(s_proj_y);
				}
			}

			ImGui::End();
		}

		void CameraPanel::set_instance(core::Instance* instance)
		{
			m_cur_instance = instance;
		}
	} // namespace panel
} // namespace sc