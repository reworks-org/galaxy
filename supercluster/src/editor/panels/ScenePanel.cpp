///
/// ScenePanel.cpp
/// supercluster
///
/// Refer to LICENSE.txt for more details.
///

#include <portable-file-dialogs.h>

#include <galaxy/state/layers/RuntimeLayer.hpp>
#include <galaxy/state/layers/UILayer.hpp>
#include <galaxy/ui/ImGuiHelpers.hpp>

#include "ScenePanel.hpp"

using namespace galaxy;

namespace sc
{
	namespace panel
	{
		void ScenePanel::render(state::SceneManager& sm)
		{
			if (ImGui::Begin("Scenes"))
			{
				if (ImGui::Button("New"))
				{
					ImGui::OpenPopup("NewScenePopup", ImGuiPopupFlags_NoOpenOverExistingPopup);
					ui::imgui_center_next_window();
				}

				ImGui::SameLine();

				if (ImGui::Button("Clear All"))
				{
					ui::imgui_open_confirm("ClearAllScenesPopup");
				}

				ui::imgui_confirm("ClearAllScenesPopup", [&]() {
					sm.clear();
				});

				if (ImGui::BeginPopup("NewScenePopup"))
				{
					static std::string s_buff;
					if (ImGui::InputText("Name", &s_buff, ImGuiInputTextFlags_EnterReturnsTrue))
					{
						if (s_buff.empty())
						{
							GALAXY_LOG(GALAXY_ERROR, "Scene name cannot be empty.");
						}
						else
						{
							sm.make(s_buff);
							s_buff = "";
						}

						ImGui::CloseCurrentPopup();
					}

					ImGui::EndPopup();
				}

				ImGui::Spacing();

				if (sm.all().size() > 0)
				{
					ImGui::Text(std::format("Current: {0}", sm.current().get_name()).c_str());
				}
				else
				{
					ImGui::Text("Current: ");
				}

				ImGui::Separator();

				for (auto&& [name, scene] : sm.all())
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
						if (ImGui::Button("Set"))
						{
							sm.change(name);
						}

						ImGui::SameLine();

						if (ImGui::Button("Remove"))
						{
							ui::imgui_open_confirm("TreeRemovePopup");
						}

						ui::imgui_confirm("TreeRemovePopup", [&]() {
							if (!sm.remove(name))
							{
								ui::imgui_open_alert("TreeRemoveAlert");
							}
						});

						ui::imgui_alert("TreeRemoveAlert", "Cant remove the active scene.");

						/*
						ImGui::Text("Camera:");
						float camera_x = scene->get_camera().get_pos().x;
						if (ImGui::InputFloat("X##01", &camera_x, 1.0f, 10.0f, "%.1f", ImGuiInputTextFlags_CharsNoBlank | ImGuiInputTextFlags_EnterReturnsTrue))
						{
							scene->get_camera().move(camera_x, 0.0f);
						}

						float camera_y = scene->get_camera().get_pos().y;
						if (ImGui::InputFloat("Y##02", &camera_y, 1.0f, 10.0f, "%.1f", ImGuiInputTextFlags_CharsNoBlank | ImGuiInputTextFlags_EnterReturnsTrue))
						{
							scene->get_camera().move(0.0f, camera_y);
						}

						float camera_speed = scene->get_camera().get_speed();
						if (ImGui::InputFloat("Speed##03",
								&camera_speed,
								1.0f,
								10.0f,
								"%.1f",
								ImGuiInputTextFlags_CharsNoBlank | ImGuiInputTextFlags_EnterReturnsTrue))
						{
							scene->get_camera().set_speed(camera_speed);
						}

						float camera_zoom = scene->get_camera().get_scale();
						if (ImGui::SliderFloat("Zoom##04", &camera_zoom, 1.0f, 10.0f, "%.1f", ImGuiSliderFlags_AlwaysClamp))
						{
							scene->get_camera().zoom(camera_zoom);
						}
						*/

						ImGui::Spacing();

						if (ImGui::Button("New Runtime"))
						{
							ImGui::OpenPopup("NewRuntimeLayerPopup", ImGuiPopupFlags_NoOpenOverExistingPopup);
							ui::imgui_center_next_window();
						}

						ImGui::SameLine();

						if (ImGui::Button("New UI"))
						{
							ImGui::OpenPopup("NewUILayerPopup", ImGuiPopupFlags_NoOpenOverExistingPopup);
							ui::imgui_center_next_window();
						}

						if (ImGui::Button("Clear All Layers"))
						{
							ui::imgui_open_confirm("ClearAllLayersPopup");
						}

						ImGui::SameLine();

						if (ImGui::Button("Pop"))
						{
							scene->layers().pop();
						}

						ui::imgui_confirm("ClearAllLayersPopup", [&]() {
							scene->layers().clear();
						});

						if (ImGui::BeginPopup("NewRuntimeLayerPopup"))
						{
							static std::string s_buff;
							if (ImGui::InputText("Name", &s_buff, ImGuiInputTextFlags_EnterReturnsTrue))
							{
								if (s_buff.empty())
								{
									GALAXY_LOG(GALAXY_ERROR, "Runtime layer name cannot be empty.");
								}
								else
								{
									scene->layers().make<state::RuntimeLayer>(s_buff);
									s_buff = "";
								}

								ImGui::CloseCurrentPopup();
							}

							ImGui::EndPopup();
						}

						if (ImGui::BeginPopup("NewUILayerPopup"))
						{
							static std::string s_buff;
							if (ImGui::InputText("Name", &s_buff, ImGuiInputTextFlags_EnterReturnsTrue))
							{
								if (s_buff.empty())
								{
									GALAXY_LOG(GALAXY_ERROR, "UI layer name cannot be empty.");
								}
								else
								{
									scene->layers().make<state::UILayer>(s_buff);
									s_buff = "";
								}

								ImGui::CloseCurrentPopup();
							}

							ImGui::EndPopup();
						}

						ImGui::Spacing();

						if (ImGui::CollapsingHeader("Stack"))
						{
							for (const auto& layer : scene->layers().stack())
							{
								ImGui::BulletText(layer->get_name().c_str());
							}
						}

						ImGui::Spacing();
						ImGui::Text("Layers: ");

						for (auto&& [layer_key, layer] : scene->layers().cache())
						{
							ImGuiTreeNodeFlags layer_flags =
								ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_SpanAvailWidth | ImGuiTreeNodeFlags_OpenOnDoubleClick;
							if (m_selected_layer == layer_key)
							{
								layer_flags |= ImGuiTreeNodeFlags_Selected;
							}

							const bool is_open_layertree = ImGui::TreeNodeEx(layer_key.c_str(), layer_flags);
							if (ImGui::IsItemClicked())
							{
								m_selected_layer = layer_key;
							}

							if (is_open_layertree)
							{
								ImGui::Spacing();

								if (ImGui::Button("Push"))
								{
									scene->layers().push(layer_key);
								}

								ImGui::SameLine();

								if (ImGui::Button("Pop"))
								{
									scene->layers().pop(layer_key);
								}

								ImGui::TreePop();
							}
						}

						ImGui::TreePop();
					}
				}
			}

			ImGui::End();
		}
	} // namespace panel
} // namespace sc