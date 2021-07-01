///
/// ScenePanel.cpp
/// supercluster
///
/// Refer to LICENSE.txt for more details.
///

#include <galaxy/core/ServiceLocator.hpp>
#include <galaxy/fs/FileSystem.hpp>
#include <imgui/addons/ToggleButton.h>
#include <imgui/imgui_stdlib.h>
#include <magic_enum.hpp>

#include "ScenePanel.hpp"

#define INPUT_WIDTH 150

using namespace galaxy;

namespace sc
{
	namespace panel
	{
		void ScenePanel::render(core::SceneStack& scene_stack, OpenGLOperationStack& gl_operations)
		{
			if (ImGui::Begin("Scenes"))
			{
				if (ImGui::Button("New Scene"))
				{
					ImGui::OpenPopup("NewScenePopup");
				}

				if (ImGui::Button("Pop"))
				{
					scene_stack.pop();
				}

				if (ImGui::BeginPopup("NewScenePopup"))
				{
					ImGui::Text("2D");
					ImGui::SameLine();

					// False = 2D, True = 3D.
					static bool s_type = true;
					ImGui::ToggleButton("2D3DToggleButton", &s_type);

					ImGui::SameLine();
					ImGui::Text("NYI");

					static std::string s_buff = "";
					if (ImGui::InputText("New Scene", &s_buff, ImGuiInputTextFlags_EnterReturnsTrue))
					{
						if (s_type)
						{
							GALAXY_LOG(GALAXY_WARNING, "Alternate scene structures not yet implemented.");
						}
						else
						{
							scene_stack.create(s_buff);
						}

						s_buff = "";
					}

					ImGui::EndPopup();
				}

				for (const auto& [name, scene] : scene_stack.get_scenes())
				{
					ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_SpanAvailWidth | ImGuiTreeNodeFlags_OpenOnDoubleClick;
					if (m_selected == name)
					{
						flags |= ImGuiTreeNodeFlags_Selected;
					}

					const bool is_open = ImGui::TreeNodeEx(name.c_str(), flags);
					if (ImGui::IsItemClicked())
					{
						m_selected = name;
					}

					if (is_open)
					{
						ImGui::Spacing();

						if (ImGui::Button("Push"))
						{
							scene_stack.push(name);
						}

						/*
						ImGui::Spacing();
						ImGui::Text("Camera:");
						ImGui::Spacing();

						ImGui::Text("Move");

						float s_move_x = scene->m_camera.get_pos().x;
						ImGui::SetNextItemWidth(INPUT_WIDTH);
						if (ImGui::InputFloat("X##01", &s_move_x, 1.0f, 10.0f, "%.1f", ImGuiInputTextFlags_CharsNoBlank | ImGuiInputTextFlags_EnterReturnsTrue))
						{
							scene->m_camera.move_x(s_move_x);
						}

						ImGui::SameLine();

						float s_move_y = scene->m_camera.get_pos().y;
						ImGui::SetNextItemWidth(INPUT_WIDTH);
						if (ImGui::InputFloat("Y##02", &s_move_y, 1.0f, 10.0f, "%.1f", ImGuiInputTextFlags_CharsNoBlank | ImGuiInputTextFlags_EnterReturnsTrue))
						{
							scene->m_camera.move_y(s_move_y);
						}

						int s_speed = static_cast<int>(scene->m_camera.get_speed());
						if (ImGui::InputInt("Speed##03", &s_speed, 1, 10, ImGuiInputTextFlags_CharsNoBlank | ImGuiInputTextFlags_EnterReturnsTrue))
						{
							scene->m_camera.set_speed(s_speed);
						}

						float s_zoom = scene->m_camera.get_scale();
						if (ImGui::SliderFloat("Scale##04", &s_zoom, 0.2, 10.0, "%.1f", ImGuiSliderFlags_AlwaysClamp | ImGuiSliderFlags_ClampOnInput))
						{
							scene->m_camera.zoom(s_zoom);
						}

						ImGui::Text("Projection");

						float s_proj_x = scene->m_camera.get_width();
						ImGui::SetNextItemWidth(INPUT_WIDTH);
						if (ImGui::InputFloat("X##05", &s_proj_x, 1.0f, 10.0f, "%.1f", ImGuiInputTextFlags_CharsNoBlank | ImGuiInputTextFlags_EnterReturnsTrue))
						{
							scene->m_camera.set_width(s_proj_x);
						}

						ImGui::SameLine();

						float s_proj_y = scene->m_camera.get_height();
						ImGui::SetNextItemWidth(INPUT_WIDTH);
						if (ImGui::InputFloat("Y##06", &s_proj_y, 1.0f, 10.0f, "%.1f", ImGuiInputTextFlags_CharsNoBlank | ImGuiInputTextFlags_EnterReturnsTrue))
						{
							scene->m_camera.set_height(s_proj_y);
						}
						*/

						ImGui::Spacing();
						ImGui::TreePop();
					}
				}
			}

			ImGui::End();
		}
	} // namespace panel
} // namespace sc