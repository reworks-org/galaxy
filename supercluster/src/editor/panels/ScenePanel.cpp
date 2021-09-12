///
/// ScenePanel.cpp
/// supercluster
///
/// Refer to LICENSE.txt for more details.
///

#include <imgui/imgui_stdlib.h>

#include "ScenePanel.hpp"

using namespace galaxy;

namespace sc
{
	namespace panel
	{
		void ScenePanel::render(core::SceneStack& scene_stack)
		{
			if (ImGui::Begin("Scenes"))
			{
				if (ImGui::Button("New Scene"))
				{
					ImGui::OpenPopup("NewScenePopup", ImGuiPopupFlags_NoOpenOverExistingPopup);
				}

				if (ImGui::BeginPopup("NewScenePopup"))
				{
					static std::string s_buff;
					if (ImGui::InputText("New Scene", &s_buff, ImGuiInputTextFlags_EnterReturnsTrue))
					{
						if (s_buff.empty())
						{
							GALAXY_LOG(GALAXY_ERROR, "Scene name cannot be empty.");
						}
						else
						{
							scene_stack.create(s_buff);
							s_buff = "";
						}
					}

					ImGui::EndPopup();
				}

				ImGui::Spacing();
				if (ImGui::Button("Pop"))
				{
					scene_stack.pop();
				}
				ImGui::Spacing();

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

						ImGui::Spacing();
						ImGui::InputText("Name", &scene->m_name, ImGuiInputTextFlags_EnterReturnsTrue);
						ImGui::Spacing();

						ImGui::Text("Camera:");
						// clang-format off
						float camera_x = scene->m_camera.get_pos().x;
						if (ImGui::InputFloat("X##01", &camera_x, 1.0f, 10.0f, "%.1f", ImGuiInputTextFlags_CharsNoBlank | ImGuiInputTextFlags_EnterReturnsTrue))
						{
							scene->m_camera.move(camera_x, 0.0f);
						}

						float camera_y = scene->m_camera.get_pos().y;
						if (ImGui::InputFloat("Y##02", &camera_y, 1.0f, 10.0f, "%.1f", ImGuiInputTextFlags_CharsNoBlank | ImGuiInputTextFlags_EnterReturnsTrue))
						{
							scene->m_camera.move(0.0f, camera_y);
						}

						float camera_speed = scene->m_camera.get_speed();
						if (ImGui::InputFloat("Speed##03", &camera_speed, 1.0f, 10.0f, "%.1f", ImGuiInputTextFlags_CharsNoBlank | ImGuiInputTextFlags_EnterReturnsTrue))
						{
							scene->m_camera.set_speed(camera_speed);
						}

						float camera_zoom = scene->m_camera.get_scale();
						if (ImGui::SliderFloat("Zoom##04", &camera_zoom, 1.0f, 10.0f, "%.1f", ImGuiSliderFlags_AlwaysClamp))
						{
							scene->m_camera.zoom(camera_zoom);
						}
						// clang-format on

						ImGui::Spacing();
						ImGui::TreePop();
					}
				}
			}

			ImGui::End();
		}
	} // namespace panel
} // namespace sc