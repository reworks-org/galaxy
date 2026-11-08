///
/// ScenePanel.cpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#include <entt/locator/locator.hpp>

#include "galaxy/resource/embedded/material_design_icons.h"
#include "galaxy/scene/SceneManager.hpp"
#include "galaxy/ui/ImGuiHelpers.hpp"

#include "ScenePanel.hpp"

namespace galaxy
{
	ScenePanel::ScenePanel() noexcept
		: m_new {"ScenePanelNewScene"}
		, m_clear {"ScenePanelClearScenes"}
	{
	}

	ScenePanel::~ScenePanel() noexcept
	{
	}

	void ScenePanel::draw()
	{
		if (ImGui::Begin(ICON_MDI_LIST_BOX_OUTLINE " Scenes"))
		{
			auto& sm = entt::locator<SceneManager>::value();

			m_new.draw_button("New");
			ImGui::SameLine();
			m_clear.draw_button("Clear");
			ImGui::SameLine();
			if (ImGui::Button("Pop"))
			{
				sm.pop();
			}
			ImGui::SameLine();
			if (ImGui::Button("Pop all"))
			{
				sm.pop_all();
			}

			m_new.draw([&]() {
				static std::string s_buff;
				ImGui::TextUnformatted("Create a new Scene");
				ImGui::Separator();

				if (ImGui::InputText("Name", &s_buff, ImGuiInputTextFlags_EnterReturnsTrue))
				{
					if (!s_buff.empty())
					{
						sm.add(s_buff);
						s_buff = "";

						ImGui::CloseCurrentPopup();
					}
					else
					{
						ui::imgui_notify_error("Scene name cannot be empty.");
					}
				}
			});

			m_clear.draw([&]() {
				ui::imgui_confirm("Are you sure you want to delete all scenes?", [&]() {
					sm.clear();
				});
			});

			static std::string top;
			for (auto&& [id, scene] : sm.storage())
			{
				ImGui::PushID(static_cast<int>(id));

				ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_SpanAvailWidth | ImGuiTreeNodeFlags_OpenOnDoubleClick;

				std::string tree_id = scene->name();
				if (sm.top())
				{
					if (sm.top()->name() == scene->name())
					{
						flags   |= ImGuiTreeNodeFlags_Selected;
						tree_id += " (top)";
					}
				}

				if (ImGui::TreeNodeEx(tree_id.c_str(), flags))
				{
					if (ImGui::Button("Push"))
					{
						sm.push(scene->name());
					}

					// draw_camera(scene_ptr);
					// draw_physics(scene_ptr);
					// draw_mapping(tasks, scene_ptr, selected);
					// draw_registry(selected, scene_ptr, scene_ptr->m_registry, m_entity_filter);

					ImGui::TreePop();
				}

				ImGui::PopID();
			}
		}

		ImGui::End();
	}
} // namespace galaxy

/*
ScenePanel::ScenePanel()
	{
		for (const auto& key : core::ServiceLocator<resource::Prefabs>::ref().keys())
		{
			m_prefab_data.items.push_back(key);
		}
	}

	void ScenePanel::draw_camera(scene::Scene* scene)
	{
		if (ImGui::CollapsingHeader("Camera"))
		{
			if (ImGui::Button("Reset"))
			{
				scene->m_camera.reset();
			}

			ImGui::Text("Pos:      %.2f, %.2f", scene->m_camera.get_pos().x, scene->m_camera.get_pos().y);
			ImGui::Text("Rotation: %.1f", scene->m_camera.get_rotation());
			ImGui::Text("Zoom:     %.1f", scene->m_camera.get_scale().x);
			ImGui::Text("Origin:   %.2f, %.2f", scene->m_camera.get_origin().x, scene->m_camera.get_origin().y);

			ImGui::Checkbox("Allow Rotation", &scene->m_camera.m_allow_rotation);
			ImGui::InputFloat("Move Speed", &scene->m_camera.m_translation_speed, 0.1f, 1.0f, "%.1f");
			ImGui::InputFloat("Rotation Speed", &scene->m_camera.m_rotation_speed, 0.1f, 1.0f, "%.1f");
		}
	}

	void ScenePanel::draw_physics(scene::Scene* scene)
	{
		if (ImGui::CollapsingHeader("Physics"))
		{
			float gravity[2] = {scene->m_b2world.GetGravity().x, scene->m_b2world.GetGravity().y};
			if (ImGui::InputFloat2("Gravity", &gravity[0], "%.2f", ImGuiInputTextFlags_AutoSelectAll | ImGuiInputTextFlags_CharsNoBlank | ImGuiInputTextFlags_EnterReturnsTrue))
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

			ImGui::SliderInt("Position Iterations", &scene->m_velocity_iterations, 1, 14);
			ImGui::SliderInt("Velocity Iterations", &scene->m_position_iterations, 1, 14);
		}
	}

	void ScenePanel::draw_mapping(meta::vector<std::move_only_function<void(void)>>& tasks, scene::Scene* scene, Selected& selected)
	{
		if (ImGui::CollapsingHeader("Mapping"))
		{
			if (ImGui::Button("Load World"))
			{
				auto& fs   = core::ServiceLocator<fs::VirtualFileSystem>::ref();
				auto  file = fs.open_file_dialog({".ldtk"});
				if (!file.empty())
				{
					tasks.push_back([&]() {
						if (scene->load_world(file))
						{
							m_map_data.items.clear();

							for (const auto& [key, value] : scene->m_world.maps())
							{
								m_map_data.items.push_back(key);
							}
						}
					});
				}
			}

			ImGui::SameLine();

			if (ImGui::Button("Clear"))
			{
				scene->m_world.clear();
			}

			ImGui::Spacing();

			if (scene->m_world.loaded())
			{
				const auto name = scene->m_world.name().c_str();

				ImGui::PushID(name);
				ImGui::Text("Name: %s", name);

				if (ImGui::ComboAutoSelect("Map", m_map_data))
				{
					scene->m_world.set_active(m_map_data.input);
				}

				auto* active = scene->m_world.get_active();
				if (active)
				{
					ImGui::Text("Name: %s", active->name().c_str());
					draw_registry(selected, scene, active->m_registry, m_map_entity_filter);
				}

				ImGui::PopID();
			}
		}
	}

	void ScenePanel::draw_registry(Selected& selected, scene::Scene* scene, core::Registry& registry, ImGuiTextFilter& filter)
	{
		if (ImGui::CollapsingHeader("Registry"))
		{
			if (ImGui::Button("New Entity"))
			{
				selected.entity = registry.create();
				selected.scene  = scene;
			}

			if (ImGui::CollapsingHeader("Prefabs"))
			{
				ImGui::ComboAutoSelect("List", m_prefab_data);
				if (ImGui::Button("Load"))
				{
					auto selected = std::string {m_prefab_data.input};
					if (!selected.empty())
					{
						registry.create_from_prefab(selected);
					}
					else
					{
						ui::imgui_notify_warning("Please select a prefab.");
					}
				}

				ImGui::SameLine();

				if (ImGui::Button("Save"))
				{
					if (selected.entity != entt::null && selected.scene != nullptr)
					{
						core::Prefab prefab;
						prefab.from_entity(selected.entity, selected.scene->m_registry.m_entt);

						const auto base64 = math::encode_base64(prefab.to_json().dump(4));
						const auto zlib   = math::encode_zlib(base64);

						auto& fs   = core::ServiceLocator<fs::VirtualFileSystem>::ref();
						auto  dest = fs.open_save_dialog("untitled.galaxyprefab", {"*.galaxyprefab"});

						if (!fs.write(zlib, dest))
						{
							GALAXY_LOG(GALAXY_ERROR, "Failed to save prefab.");
							ui::imgui_notify_error("Failed to save prefab.");
						}
					}
				}
			}

			filter.DrawWithHint("###EntitySearch", ICON_MDI_MAGNIFY "Search by tag...", ImGui::GetContentRegionAvail().x);

			const auto entity_listbox_item = [&](entt::entity entity, components::Tag* tag) {
				const auto is_selected = (selected.entity == entity);
				auto       id          = std::to_string(static_cast<std::uint32_t>(entity));
				if (tag)
				{
					id = tag->m_tag + "##" + id;
				}

				if (ImGui::Selectable(id.c_str(), is_selected))
				{
					selected.entity = entity;
					selected.scene  = scene;
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
						registry.m_entt.destroy(entity);
						if (selected.entity == entity)
						{
							selected.entity = entt::null;
							selected.scene  = scene;
						}
					}
				}
			};

			ImGui::SetNextItemWidth(ImGui::GetContentRegionAvail().x);
			if (ImGui::BeginListBox("##EntityList"))
			{
				for (const auto& [entity] : registry.m_entt.storage<entt::entity>().each())
				{
					auto tag = registry.m_entt.try_get<components::Tag>(entity);
					if (tag)
					{
						if (filter.PassFilter(tag->m_tag.c_str()))
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
		}
	}
*/
