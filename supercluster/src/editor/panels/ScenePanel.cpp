///
/// ScenePanel.cpp
/// supercluster
///
/// Refer to LICENSE.txt for more details.
///

#include <imgui/imnotify/material_design_icons.h>

#include <galaxy/components/Tag.hpp>
#include <galaxy/core/ServiceLocator.hpp>
#include <galaxy/fs/VirtualFileSystem.hpp>
#include <galaxy/math/Base64.hpp>
#include <galaxy/math/ZLib.hpp>
#include <galaxy/resource/Prefabs.hpp>
#include <galaxy/ui/ImGuiHelpers.hpp>

#include "ScenePanel.hpp"

using namespace galaxy;

namespace sc
{
	namespace panel
	{
		void ScenePanel::render(scene::SceneManager& sm, Selected& selected_entity, UpdateStack& updates)
		{
			if (ImGui::Begin(ICON_MDI_LIST_BOX_OUTLINE " Scenes"))
			{
				if (ImGui::Button("New Scene"))
				{
					ImGui::OpenPopup("NewScenePopup", ImGuiPopupFlags_NoOpenOverExistingPopup);
					ui::imgui_center_next_window();
				}

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
							sm.add(s_buff);
							s_buff = "";
						}

						ImGui::CloseCurrentPopup();
					}

					ImGui::EndPopup();
				}

				for (auto&& [name, scene] : sm.map())
				{
					ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_SpanAvailWidth | ImGuiTreeNodeFlags_OpenOnDoubleClick;
					if (m_selected == scene->m_name)
					{
						flags |= ImGuiTreeNodeFlags_Selected;
					}

					const bool is_open = ImGui::TreeNodeEx(scene->m_name.c_str(), flags);
					if (ImGui::IsItemClicked())
					{
						m_selected = scene->m_name;
					}

					if (is_open)
					{
						if (scene->enabled())
						{
							if (ImGui::Button("Disable"))
							{
								scene->disable();
							}
						}
						else
						{
							if (ImGui::Button("Enable"))
							{
								scene->enable();
							}
						}

						ImGui::SameLine();

						if (ImGui::Button("Delete"))
						{
							sm.remove(scene->m_name);
						}

						ImGui::Spacing();
						ImGui::Separator();
						ImGui::Spacing();
						/*
						ImGui::TextUnformatted("Mapping:");

						m_filter_maps.DrawWithHint("###MapSearch", ICON_MDI_MAGNIFY "Filter maps...", ImGui::GetContentRegionAvail().x);

						ImGui::SetNextItemWidth(ImGui::GetContentRegionAvail().x);
						if (ImGui::BeginCombo("##AssignableMaps", "Assign a Map"))
						{
							for (const auto& key : core::ServiceLocator<resource::Maps>::ref().keys())
							{
								auto begin = scene->m_assigned_maps.begin();
								auto end   = scene->m_assigned_maps.end();

								if (m_filter_maps.PassFilter(key.c_str()) && (std::find(begin, end, key) == end))
								{
									if (ImGui::Selectable(key.c_str()))
									{
										scene->m_assigned_maps.emplace_back(key);
									}
								}
							}

							ImGui::EndCombo();
						}

						if (ImGui::Button("Load Maps"))
						{
							scene->load_maps();
						}

						m_filter_assigned_maps.DrawWithHint("###AssignedMapsSearch",
							ICON_MDI_MAGNIFY "Filter Assigned Maps...",
							ImGui::GetContentRegionAvail().x);

						static std::string name = scene->m_map ? scene->m_map->get_name() : "Set Active Map";

						ImGui::SetNextItemWidth(ImGui::GetContentRegionAvail().x);
						if (ImGui::BeginCombo("##SetActiveMap", name.c_str()))
						{
							for (const auto& key : scene->m_assigned_maps)
							{
								if (m_filter_assigned_maps.PassFilter(key.c_str()))
								{
									const bool selected = (name == key);
									if (ImGui::Selectable(key.c_str(), selected))
									{
										name = key;
										scene->set_active_map(name);
									}

									if (selected)
									{
										ImGui::SetItemDefaultFocus();
									}
								}
							}

							ImGui::EndCombo();
						}
						*/
						ImGui::Spacing();
						ImGui::Separator();
						ImGui::Spacing();

						ImGui::TextUnformatted("Camera:");
						// ImGui::Text("Pos: %.2f, %.2f", scene->m_camera.get_x(), scene->m_camera.get_y());
						// ImGui::SameLine();
						// ImGui::Text("Rotation: %.1f", scene->m_camera.get_rotation());
						// ImGui::SameLine();
						// ImGui::Text("Zoom: %.1f", scene->m_camera.get_scale().x);

						float vp[2] = {scene->m_camera.get_viewport().x, scene->m_camera.get_viewport().y};
						if (ImGui::InputFloat2("Viewport", vp, "%.1f", ImGuiInputTextFlags_CharsNoBlank))
						{
							scene->m_camera.set_viewport(vp[0], vp[1]);
						}

						ImGui::Checkbox("Allow Rotation", &scene->m_camera.m_allow_rotation);
						ImGui::InputFloat("Move Speed", &scene->m_camera.m_translation_speed, 0.1f, 1.0f, "%.1f");
						ImGui::InputFloat("Rotation Speed", &scene->m_camera.m_rotation_speed, 0.1f, 1.0f, "%.1f");

						ImGui::Spacing();
						ImGui::Separator();
						ImGui::Spacing();

						ImGui::TextUnformatted("Physics:");
						if (ImGui::Button("Dump"))
						{
							scene->m_b2world.Dump();
						}

						float gravity[2] = {scene->m_b2world.GetGravity().x, scene->m_b2world.GetGravity().y};
						if (ImGui::InputFloat2("Gravity",
								&gravity[0],
								"%.2f",
								ImGuiInputTextFlags_AutoSelectAll | ImGuiInputTextFlags_CharsNoBlank | ImGuiInputTextFlags_EnterReturnsTrue))
						{
							scene->m_b2world.SetGravity({gravity[0], gravity[1]});
						}

						auto allow_sleeping = scene->m_b2world.GetAllowSleeping();
						if (ImGui::Checkbox("Allow Sleeping", &allow_sleeping))
						{
							scene->m_b2world.SetAllowSleeping(allow_sleeping);
						}

						ImGui::SameLine();

						auto allow_autoclear = scene->m_b2world.GetAutoClearForces();
						if (ImGui::Checkbox("Auto Clear Forces", &allow_autoclear))
						{
							scene->m_b2world.SetAutoClearForces(allow_autoclear);
						}

						ImGui::SliderInt("Position Iterations", &scene->m_velocity_iterations, 1, 10);
						ImGui::SliderInt("Velocity Iterations", &scene->m_position_iterations, 1, 10);

						ImGui::Spacing();
						ImGui::Separator();
						ImGui::Spacing();

						// Right-click on blank space.
						if (ImGui::BeginPopupContextWindow("##CreateNewEntityContext",
								ImGuiPopupFlags_MouseButtonRight | ImGuiPopupFlags_NoOpenOverExistingPopup))
						{
							if (ImGui::MenuItem("Create New Entity"))
							{
								selected_entity.m_selected = scene->create();
								selected_entity.m_scene    = scene.get();
							}

							ImGui::EndPopup();
						}

						if (ImGui::Button("New Entity"))
						{
							selected_entity.m_selected = scene->create();
							selected_entity.m_scene    = scene.get();
						}

						ImGui::SameLine();

						if (ImGui::Button("Clear World"))
						{
							ui::imgui_open_confirm("ClearWorldPopup");
						}

						if (ImGui::Button("Load Prefab"))
						{
							ImGui::OpenPopup("PrefabListPopup");
						}

						ImGui::SameLine();

						if (ImGui::Button("Save as Prefab"))
						{
							if (selected_entity.m_selected != entt::null && selected_entity.m_scene != nullptr)
							{
								core::Prefab prefab {selected_entity.m_selected, selected_entity.m_scene->m_registry};
								const auto   base64 = math::encode_base64(prefab.to_json().dump(4));
								const auto   zlib   = math::encode_zlib(base64);

								auto& fs   = core::ServiceLocator<fs::VirtualFileSystem>::ref();
								auto  dest = fs.open_save_dialog("untitled.gprefab", {"*.gprefab"});

								if (!fs.write(zlib, dest))
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
									scene->create_from_prefab(s_selected);
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
							const auto is_selected = (selected_entity.m_selected == entity);
							auto       id          = std::to_string(static_cast<std::uint32_t>(entity));
							if (tag)
							{
								id = tag->m_tag + "##" + id;
							}

							if (ImGui::Selectable(id.c_str(), is_selected))
							{
								selected_entity.m_selected = entity;
								selected_entity.m_scene    = scene.get();
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
									scene->m_registry.destroy(entity);
									if (selected_entity.m_selected == entity)
									{
										selected_entity.m_selected = entt::null;
										selected_entity.m_scene    = scene.get();
									}
								}
							}
						};

						ImGui::SetNextItemWidth(ImGui::GetContentRegionAvail().x);
						if (ImGui::BeginListBox("##EntityList"))
						{
							for (const auto& [entity] : scene->m_registry.storage<entt::entity>().each())
							{
								auto tag = scene->m_registry.try_get<components::Tag>(entity);
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
							}

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
