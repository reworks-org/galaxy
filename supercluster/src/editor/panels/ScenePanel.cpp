///
/// ScenePanel.cpp
/// supercluster
///
/// Refer to LICENSE.txt for more details.
///

#include <imgui_addons/notify/imgui_notify.h>
#include <portable-file-dialogs.h>

#include <galaxy/core/ServiceLocator.hpp>
#include <galaxy/components/Tag.hpp>
#include <galaxy/fs/VirtualFileSystem.hpp>
#include <galaxy/state/layers/RuntimeLayer.hpp>
#include <galaxy/state/layers/UILayer.hpp>
#include <galaxy/ui/ImGuiHelpers.hpp>

#include "ScenePanel.hpp"

using namespace galaxy;

namespace sc
{
	namespace panel
	{
		void ScenePanel::render(state::SceneManager& sm, Selected& selected)
		{
			if (ImGui::Begin("Scenes"))
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
							sm.make(s_buff);
							s_buff = "";
						}

						ImGui::CloseCurrentPopup();
					}

					ImGui::EndPopup();
				}

				if (sm.has_current())
				{
					ImGui::Text(std::format("Current: {0}", sm.current().get_name()).c_str());
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
						if (ImGui::Button("Set Scene"))
						{
							sm.change(name);
						}

						ImGui::SameLine();

						if (ImGui::Button("Remove Scene"))
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

						auto& camera = scene->get_camera();
						ImGui::Text("Camera:");
						ImGui::Text("Pos: %.2f, %.2f", camera.get_x(), camera.get_y());
						ImGui::SameLine();
						ImGui::Text("Rotation: %.1f", camera.get_rotation());
						ImGui::SameLine();
						ImGui::Text("Zoom: %.1f", camera.get_zoom());

						ImGui::Checkbox("Allow Rotation", &camera.m_allow_rotate);
						ImGui::InputFloat("Speed", &camera.m_translation_speed, 0.1f, 1.0f, "%.1f");
						ImGui::InputFloat("Rotation Speed", &camera.m_rotation_speed, 0.1f, 1.0f, "%.1f");

						ImGui::Spacing();
						ImGui::Separator();
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

						ImGui::SameLine();

						if (ImGui::Button("Clear All Layers"))
						{
							ui::imgui_open_confirm("ClearAllLayersPopup");
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

						if (ImGui::CollapsingHeader("Layers"))
						{
							if (ImGui::Button("Pop"))
							{
								scene->layers().pop();
							}

							for (const auto& layer : scene->layers().stack())
							{
								ImGui::BulletText(layer->get_name().c_str());
							}
						}

						for (auto&& [layer_key, layer] : scene->layers().cache())
						{
							ImGuiTreeNodeFlags layer_flags =
								ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_SpanAvailWidth | ImGuiTreeNodeFlags_OpenOnDoubleClick;

							auto& world = layer->world();

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
								// Right-click on blank space
								if (ImGui::BeginPopupContextWindow("##CreateNewEntityContext",
										ImGuiPopupFlags_MouseButtonRight | ImGuiPopupFlags_NoOpenOverExistingPopup))
								{
									if (ImGui::MenuItem("Create New Entity"))
									{
										selected.m_selected = world.create();
										selected.m_world    = &world;
									}

									ImGui::EndPopup();
								}

								if (ImGui::Button("Push Layer"))
								{
									scene->layers().push(layer_key);
								}

								ImGui::SameLine();

								if (ImGui::Button("Pop Layer"))
								{
									scene->layers().pop(layer_key);
								}

								if (ImGui::Button("New Entity"))
								{
									selected.m_selected = world.create();
									selected.m_world    = &world;
								}

								ImGui::SameLine();

								if (ImGui::Button("Load Entity"))
								{
									auto file = core::ServiceLocator<fs::VirtualFileSystem>::ref().show_open_dialog("*.json");
									if (file.has_value())
									{
										selected.m_selected = world.create_from_file(file.value());
										selected.m_world    = &world;
									}
								}

								ImGui::SameLine();

								if (ImGui::Button("Clear World"))
								{
									ui::imgui_open_confirm("ClearWorldPopup");
								}

								ui::imgui_confirm("ClearWorldPopup", [&]() {
									world.clear();
								});

								ImGui::Text("Entities");
								ImGui::SameLine();

								static std::string s_search = "";
								ImGui::InputTextWithHint("##EntitySearch", "Search...", &s_search, ImGuiInputTextFlags_AutoSelectAll);

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
										selected.m_world    = &world;
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
											world.m_registry.destroy(entity);
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
									world.m_registry.each([&](const entt::entity entity) {
										auto tag = world.m_registry.try_get<components::Tag>(entity);
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

						ImGui::TreePop();
					}
				}
			}

			ImGui::End();
		}
	} // namespace panel
} // namespace sc