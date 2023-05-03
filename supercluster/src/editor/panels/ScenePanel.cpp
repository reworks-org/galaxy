///
/// ScenePanel.cpp
/// supercluster
///
/// Refer to LICENSE.txt for more details.
///

#include <filesystem>

#include <imgui_addons/imgui_notify.h>
#include <RmlUi/Core/Context.h>
#include <RmlUi/Core/ElementDocument.h>

#include <galaxy/algorithm/Base64.hpp>
#include <galaxy/algorithm/ZLib.hpp>
#include <galaxy/core/ServiceLocator.hpp>
#include <galaxy/components/Tag.hpp>
#include <galaxy/fs/VirtualFileSystem.hpp>
#include <galaxy/resource/Maps.hpp>
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
					static std::string s_buff;
					if (ImGui::InputText("Name", &s_buff, ImGuiInputTextFlags_EnterReturnsTrue))
					{
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
								ImGui_Notify::InsertNotification({ImGuiToastType_Warning, 2000, "Cannot remove active scene."});
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

						ImGui::Text("Camera:");
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

						ImGui::SameLine();

						if (ImGui::Button("Map Settings"))
						{
							ImGui::OpenPopup("LoadMapPopup");
						}

						ui::imgui_confirm("ClearWorldPopup", [&]() {
							scene->m_world.clear();
						});

						if (ImGui::BeginPopup("LoadMapPopup"))
						{
							ImGui::Text("Select Map to Load.");

							ImGui::Separator();
							ImGui::Spacing();

							static std::string s_search = "";
							ImGui::InputTextWithHint("##ModalLoadMapSearch", "Search...", &s_search, ImGuiInputTextFlags_AutoSelectAll);

							static std::string s_selected = scene->m_map.get_name();
							if (ImGui::BeginCombo("##ModalLoadMapComboList", s_selected.c_str()))
							{
								for (const auto& key : core::ServiceLocator<resource::Maps>::ref().keys())
								{
									if (key.find(s_search) != std::string::npos)
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

							if (ImGui::Button("Load Selected Map"))
							{
								if (!s_selected.empty())
								{
									updates.emplace_back([scene]() {
										scene->m_map.load_map(s_selected);
									});
								}
								else
								{
									ImGui_Notify::InsertNotification({ImGuiToastType_Error, 2000, "Please select a map before loading."});
								}
							}

							ImGui::Spacing();

							if (ImGui::Button("Enable Map"))
							{
								scene->m_map.enable_map();
							}

							ImGui::SameLine();

							if (ImGui::Button("Disable Map"))
							{
								scene->m_map.disable_map();
							}

							ImGui::EndPopup();
						}

						if (ImGui::Button("Load Prefab"))
						{
							ImGui::OpenPopup("PrefabListPopup");
						}

						ImGui::SameLine();

						if (ImGui::Button("Save as Prefab"))
						{
							if (selected.m_selected != entt::null && selected.m_world != nullptr)
							{
								const auto data   = scene->m_world.serialize_entity(selected.m_selected);
								const auto base64 = algorithm::encode_base64(data.dump(4));
								const auto zlib   = algorithm::encode_zlib(base64);

								auto& fs = core::ServiceLocator<fs::VirtualFileSystem>::ref();
								if (!fs.save_using_dialog(zlib, "untitled.gprefab", {"*.gprefab"}))
								{
									GALAXY_LOG(GALAXY_ERROR, "Failed to save prefab.");
									ImGui_Notify::InsertNotification({ImGuiToastType_Error, 2000, "Failed to save prefab."});
								}
							}
						}

						if (ImGui::BeginPopup("PrefabListPopup"))
						{
							static std::string s_search = "";
							ImGui::InputTextWithHint("##PrefabSearch", "Search...", &s_search, ImGuiInputTextFlags_AutoSelectAll);

							static std::string s_selected = "";
							if (ImGui::BeginCombo("##PrefabList", s_selected.c_str()))
							{
								for (const auto& key : core::ServiceLocator<resource::Prefabs>::ref().keys())
								{
									if (key.find(s_selected) != std::string::npos)
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
									ImGui_Notify::InsertNotification({ImGuiToastType_Warning, 2000, "Please select a prefab."});
								}
							}

							ImGui::EndPopup();
						}

						static std::string s_search = "";
						ImGui::InputTextWithHint("##EntitySearch", "Search Entity by Tag...", &s_search, ImGuiInputTextFlags_AutoSelectAll);

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

						if (ImGui::BeginListBox("##EntityList"))
						{
							scene->m_world.m_registry.each([&](const entt::entity entity) {
								auto tag = scene->m_world.m_registry.try_get<components::Tag>(entity);
								if (tag)
								{
									if (tag->m_tag.find(s_search) != std::string::npos)
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