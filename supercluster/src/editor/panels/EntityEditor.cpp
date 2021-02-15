/*
///
/// EntityEditor.cpp
/// supercluster
///
/// Refer to LICENSE.txt for more details.
///

#include <galaxy/components/Animated.hpp>
#include <galaxy/components/BatchedSprite.hpp>
#include <galaxy/components/Circle.hpp>
#include <galaxy/components/Line.hpp>
#include <galaxy/components/OnEvent.hpp>
#include <galaxy/components/Physics.hpp>
#include <galaxy/components/Point.hpp>
#include <galaxy/components/Renderable.hpp>
#include <galaxy/components/ShaderID.hpp>
#include <galaxy/components/Sprite.hpp>
#include <galaxy/components/Text.hpp>
#include <galaxy/components/Transform.hpp>

#include <imgui/imgui_stdlib.h>

#include "EntityEditor.hpp"

namespace sc
{
	namespace panel
	{
		void EntityEditor::pre_render()
		{
			if (!m_sprites_to_create.empty())
			{
				for (auto& [sprite, path] : m_sprites_to_create)
				{
					sprite->load(path);
					sprite->create();
				}

				m_sprites_to_create.clear();
			}
		}

		void EntityEditor::render()
		{
			ImGui::Begin("Entity Manager", &m_draw_entity_editor, ImGuiWindowFlags_AlwaysAutoResize);

			ImGui::Text("Input name and press enter to create. Cannot be empty.");
			ImGui::Separator();
			ImGui::Spacing();

			if (ImGui::InputText("Create", &m_edn_buffer, ImGuiInputTextFlags_EnterReturnsTrue | ImGuiInputTextFlags_AutoSelectAll))
			{
				if (!m_edn_buffer.empty())
				{
					m_active_entity     = m_world.create(m_edn_buffer);
					m_entity_debug_name = m_edn_buffer;
					m_edn_buffer        = "";
				}
			}

			ImGui::SameLine();

			if (ImGui::Button("Create from JSON"))
			{
				const auto file = SL_HANDLE.vfs()->show_open_dialog("*.json");
				m_active_entity = m_world.create_from_json(file);
			}

			if (ImGui::BeginCombo("Select Entity", m_entity_debug_name.c_str()))
			{
				for (const auto& [name, id] : m_world.get_debug_name_map())
				{
					const bool selected = (m_entity_debug_name == name);
					if (ImGui::Selectable(name.c_str(), selected))
					{
						m_entity_debug_name = name;
						m_active_entity     = id;
					}

					if (selected)
					{
						ImGui::SetItemDefaultFocus();
					}
				}

				ImGui::EndCombo();
			}

			ImGui::Text(fmt::format("Numeric ID: {0}.", m_active_entity).c_str());

			if (m_world.has(m_active_entity))
			{
				bool enabled = m_world.is_enabled(m_active_entity);
				if (ImGui::Checkbox("Is Enabled?", &enabled))
				{
					if (enabled)
					{
						m_world.enable(m_active_entity);
					}
					else
					{
						m_world.disable(m_active_entity);
					}
				}

				if (ImGui::Button("Add BatchedSprite"))
				{
					m_world.create_component<components::BatchedSprite>(m_active_entity);
				}

				ImGui::SameLine();

				if (ImGui::Button("Remove BatchedSprite"))
				{
					m_world.remove<components::BatchedSprite>(m_active_entity);
				}

				if (ImGui::Button("Add Circle"))
				{
					m_world.create_component<components::Circle>(m_active_entity);
				}

				ImGui::SameLine();

				if (ImGui::Button("Remove Circle"))
				{
					m_world.remove<components::Circle>(m_active_entity);
				}

				if (ImGui::Button("Add Line"))
				{
					m_world.create_component<components::Line>(m_active_entity);
				}

				ImGui::SameLine();

				if (ImGui::Button("Remove Line"))
				{
					m_world.remove<components::Line>(m_active_entity);
				}

				if (ImGui::Button("Add Point"))
				{
					m_world.create_component<components::Point>(m_active_entity);
				}

				ImGui::SameLine();

				if (ImGui::Button("Remove Point"))
				{
					m_world.remove<components::Point>(m_active_entity);
				}

				if (ImGui::Button("Add Renderable"))
				{
					m_world.create_component<components::Renderable>(m_active_entity);
				}

				ImGui::SameLine();

				if (ImGui::Button("Remove Renderable"))
				{
					m_world.remove<components::Renderable>(m_active_entity);
				}

				if (ImGui::Button("Add ShaderID"))
				{
					m_world.create_component<components::ShaderID>(m_active_entity);
				}

				ImGui::SameLine();

				if (ImGui::Button("Remove ShaderID"))
				{
					m_world.remove<components::ShaderID>(m_active_entity);
				}

				if (ImGui::Button("Add Sprite"))
				{
					m_world.create_component<components::Sprite>(m_active_entity);
				}

				ImGui::SameLine();

				if (ImGui::Button("Remove Sprite"))
				{
					m_world.remove<components::Sprite>(m_active_entity);
				}

				if (ImGui::Button("Add Text"))
				{
					m_world.create_component<components::Text>(m_active_entity);
				}

				ImGui::SameLine();

				if (ImGui::Button("Remove Text"))
				{
					m_world.remove<components::Text>(m_active_entity);
				}

				if (ImGui::Button("Add Transform"))
				{
					m_world.create_component<components::Transform>(m_active_entity);
				}

				ImGui::SameLine();

				if (ImGui::Button("Remove Transform"))
				{
					m_world.remove<components::Transform>(m_active_entity);
				}

				ImGui::Spacing();
				ImGui::Spacing();
				if (enabled)
				{
					component_ui(enabled, m_active_entity);
				}
			}

			ImGui::End();
		}

		void EntityEditor::display_components(bool enabled, std::uint64_t entity)
		{
			auto [batchedsprite, circle, line, point, renderable, shaderid, sprite, text, transform] = m_world.get_multi<
				components::BatchedSprite,
				components::Circle,
				components::Line,
				components::Point,
				components::Renderable,
				components::ShaderID,
				components::Sprite,
				components::Text,
				components::Transform>(entity);

			ImGui::BeginTabBar("hi");
			ImGui::TabItemButton("Bth");
			ImGui::EndTabBar();
		}
	} // namespace panel
} // namespace sc
*/