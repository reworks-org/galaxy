///
/// ScenePanel.cpp
/// supercluster
///
/// Refer to LICENSE.txt for more details.
///

#include <imgui_addons/material_design_icons.h>

#include <galaxy/algorithm/Base64.hpp>
#include <galaxy/algorithm/ZLib.hpp>
#include <galaxy/core/ServiceLocator.hpp>
#include <galaxy/components/Tag.hpp>
#include <galaxy/fs/VirtualFileSystem.hpp>
#include <galaxy/resource/Prefabs.hpp>
#include <galaxy/ui/ImGuiHelpers.hpp>

#include "ScenePanel.hpp"

using namespace galaxy;

namespace sc
{
	namespace panel
	{
		void ScenePanel::render(scene::SceneManager& sm, Selected& selected, UpdateStack& updates)
		{
			if (ImGui::Begin(ICON_MDI_LIST_BOX_OUTLINE " Scenes"))
			{
				if (ImGui::Button("New Scene"))
				{
					ImGui::OpenPopup("NewScenePopup", ImGuiPopupFlags_NoOpenOverExistingPopup);
					ui::imgui_center_next_window();
				}

				ImGui::SameLine();

				if (ImGui::Button("Delete all Scenes"))
				{
					ui::imgui_open_confirm("ClearAllScenesPopup");
				}

				ui::imgui_confirm("ClearAllScenesPopup", [&]() {
					sm.clear();
				});

				if (ImGui::BeginPopup("NewScenePopup"))
				{
					static auto make = false;

					static std::string s_buff;
					if (ImGui::InputText("##Name", &s_buff, ImGuiInputTextFlags_EnterReturnsTrue))
					{
						make = true;
					}

					if (ImGui::Button("Create"))
					{
						make = true;
					}

					if (make)
					{
						make = false;

						if (s_buff.empty())
						{
							GALAXY_LOG(GALAXY_ERROR, "Scene name cannot be empty.");
						}
						else
						{
							sm.make_scene(s_buff);
							s_buff = "";
						}

						ImGui::CloseCurrentPopup();
					}

					ImGui::EndPopup();
				}

				if (sm.has_current())
				{
					ImGui::Text(std::format("Current: {0}", sm.current().m_name).c_str());
				}
				else
				{
					ImGui::Text("Current: ");
				}

				ImGui::Separator();

				static auto s_exit_early = false;
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
						if (ImGui::Button("Push"))
						{
							sm.set_scene(name);
						}

						ImGui::SameLine();

						if (ImGui::Button("Pop"))
						{
							sm.unload_scene();
						}

						ImGui::SameLine();

						if (ImGui::Button("Delete"))
						{
							ui::imgui_open_confirm("TreeRemovePopup");
						}

						ui::imgui_confirm("TreeRemovePopup", [&]() {
							if (sm.remove(name))
							{
								s_exit_early = true;
							}
							else
							{
								ui::imgui_notify_warning("Cannot remove active scene.");
							}
						});

						if (s_exit_early)
						{
							s_exit_early = false;
							ImGui::TreePop();

							break;
						}

						ImGui::Spacing();
						ImGui::Separator();
						ImGui::Spacing();

						ImGui::TextUnformatted("Camera:");
						ImGui::Text("Pos: %.2f, %.2f", scene->m_camera.get_x(), scene->m_camera.get_y());
						ImGui::SameLine();
						ImGui::Text("Rotation: %.1f", scene->m_camera.get_rotation());
						ImGui::SameLine();
						ImGui::Text("Zoom: %.1f", scene->m_camera.get_zoom());

						float vp[2] = {scene->m_camera.get_viewport().x, scene->m_camera.get_viewport().y};
						if (ImGui::InputFloat2("Viewport", vp, "%.1f", ImGuiInputTextFlags_CharsNoBlank))
						{
							scene->m_camera.set_viewport(vp[0], vp[1]);
						}

						ImGui::Checkbox("Allow Rotation", &scene->m_camera.m_allow_rotate);
						ImGui::InputFloat("Move Speed", &scene->m_camera.m_translation_speed, 0.1f, 1.0f, "%.1f");
						ImGui::InputFloat("Rotation Speed", &scene->m_camera.m_rotation_speed, 0.1f, 1.0f, "%.1f");

						ImGui::Spacing();
						ImGui::Separator();
						ImGui::Spacing();

						ImGui::TextUnformatted("Physics:");
						if (scene->m_world.m_b2world)
						{
							auto* b2 = scene->m_world.m_b2world.get();
							if (ImGui::Button("Dump"))
							{
								b2->Dump();
							}

							float gravity[2] = {b2->GetGravity().x, b2->GetGravity().y};
							if (ImGui::InputFloat2("Gravity",
									&gravity[0],
									"%.2f",
									ImGuiInputTextFlags_AutoSelectAll | ImGuiInputTextFlags_CharsNoBlank | ImGuiInputTextFlags_EnterReturnsTrue))
							{
								b2->SetGravity({gravity[0], gravity[1]});
							}

							auto allow_sleeping = b2->GetAllowSleeping();
							if (ImGui::Checkbox("Allow Sleeping", &allow_sleeping))
							{
								b2->SetAllowSleeping(allow_sleeping);
							}

							ImGui::SameLine();

							auto allow_autoclear = b2->GetAutoClearForces();
							if (ImGui::Checkbox("Auto Clear Forces", &allow_autoclear))
							{
								b2->SetAutoClearForces(allow_autoclear);
							}

							ImGui::SliderInt("Position Iterations", &scene->m_world.m_velocity_iterations, 1, 10);
							ImGui::SliderInt("Velocity Iterations", &scene->m_world.m_position_iterations, 1, 10);
							ImGui::InputFloat("Pixels Per Meter",
								&scene->m_world.m_pixels_per_meter,
								1.0f,
								4.0f,
								"%.0f",
								ImGuiInputTextFlags_AutoSelectAll | ImGuiInputTextFlags_CharsNoBlank);
						}

						ImGui::Spacing();
						ImGui::Separator();
						ImGui::Spacing();

						float al[4] = {scene->m_lighting.ambient_light_colour.x,
							scene->m_lighting.ambient_light_colour.y,
							scene->m_lighting.ambient_light_colour.z,
							scene->m_lighting.ambient_light_colour.w};

						if (ImGui::ColorEdit4("Ambient Light", al))
						{
							scene->m_lighting.ambient_light_colour.x = al[0];
							scene->m_lighting.ambient_light_colour.y = al[1];
							scene->m_lighting.ambient_light_colour.z = al[2];
							scene->m_lighting.ambient_light_colour.w = al[3];
						}

						ImGui::Spacing();
						ImGui::Separator();
						ImGui::Spacing();

						// Right-click on blank space.
						if (ImGui::BeginPopupContextWindow("##CreateNewEntityContext",
								ImGuiPopupFlags_MouseButtonRight | ImGuiPopupFlags_NoOpenOverExistingPopup))
						{
							if (ImGui::MenuItem("Create New Entity"))
							{
								selected.m_selected = scene->m_world.create();
								selected.m_world    = &scene->m_world;
							}

							ImGui::EndPopup();
						}

						if (ImGui::Button("New Entity"))
						{
							selected.m_selected = scene->m_world.create();
							selected.m_world    = &scene->m_world;
						}

						ImGui::SameLine();

						if (ImGui::Button("Clear World"))
						{
							ui::imgui_open_confirm("ClearWorldPopup");
						}

						ui::imgui_confirm("ClearWorldPopup", [&]() {
							scene->m_world.clear();
						});

						if (ImGui::Button("Load Prefab"))
						{
							ImGui::OpenPopup("PrefabListPopup");
						}

						ImGui::SameLine();

						if (ImGui::Button("Save as Prefab"))
						{
							if (selected.m_selected != entt::null && selected.m_world != nullptr)
							{
								core::Prefab prefab {selected.m_selected, selected.m_world->m_registry};
								const auto base64 = algorithm::encode_base64(prefab.to_json().dump(4));
								const auto zlib   = algorithm::encode_zlib(base64);

								auto& fs = core::ServiceLocator<fs::VirtualFileSystem>::ref();
								if (!fs.save_using_dialog(zlib, "untitled.gprefab", {"*.gprefab"}))
								{
									GALAXY_LOG(GALAXY_ERROR, "Failed to save prefab.");
									ui::imgui_notify_error("Failed to save prefab.");
								}
							}
						}

						if (ImGui::BeginPopup("PrefabListPopup"))
						{
							m_filter_prefabs.DrawWithHint("###PrefabSearch", ICON_MDI_MAGNIFY "Search...", ImGui::GetContentRegionAvail().x);

							static std::string s_selected = "";
							if (ImGui::BeginCombo("##PrefabList", s_selected.c_str()))
							{
								for (const auto& key : core::ServiceLocator<resource::Prefabs>::ref().keys())
								{
									if (m_filter_prefabs.PassFilter(key.c_str()))
									{
										const bool selected = (s_selected == key);
										if (ImGui::Selectable(key.c_str(), selected))
										{
											s_selected = key;
										}

										if (selected)
										{
											ImGui::SetItemDefaultFocus();
										}
									}
								}

								ImGui::EndCombo();
							}

							if (ImGui::Button("Load"))
							{
								if (!s_selected.empty())
								{
									scene->m_world.create_from_prefab(s_selected);
								}
								else
								{
									ui::imgui_notify_warning("Please select a prefab.");
								}
							}

							ImGui::EndPopup();
						}

						m_filter_tags.DrawWithHint("###EntitySearch", ICON_MDI_MAGNIFY "Search by tag...", ImGui::GetContentRegionAvail().x);

						const auto entity_listbox_item = [&](entt::entity entity, components::Tag* tag) {
							const auto is_selected = (selected.m_selected == entity);
							auto id                = std::to_string(static_cast<std::uint32_t>(entity));
							if (tag)
							{
								id = tag->m_tag + "##" + id;
							}

							if (ImGui::Selectable(id.c_str(), is_selected))
							{
								selected.m_selected = entity;
								selected.m_world    = &scene->m_world;
							}

							if (is_selected)
							{
								ImGui::SetItemDefaultFocus();

								auto deleted = false;
								if (ImGui::BeginPopupContextItem())
								{
									if (ImGui::MenuItem("Delete Entity"))
									{
										deleted = true;
									}

									ImGui::EndPopup();
								}

								if (deleted)
								{
									scene->m_world.m_registry.destroy(entity);
									if (selected.m_selected == entity)
									{
										selected.m_selected = entt::null;
										selected.m_world    = nullptr;
									}
								}
							}
						};

						ImGui::SetNextItemWidth(ImGui::GetContentRegionAvail().x);
						if (ImGui::BeginListBox("##EntityList"))
						{
							scene->m_world.m_registry.each([&](const entt::entity entity) {
								auto tag = scene->m_world.m_registry.try_get<components::Tag>(entity);
								if (tag)
								{
									if (m_filter_tags.PassFilter(tag->m_tag.c_str()))
									{
										entity_listbox_item(entity, tag);
									}
								}
								else
								{
									entity_listbox_item(entity, tag);
								}
							});

							ImGui::EndListBox();
						}

						ImGui::TreePop();
					}
				}
			}

			ImGui::End();
		}
	} // namespace panel
} // namespace sc