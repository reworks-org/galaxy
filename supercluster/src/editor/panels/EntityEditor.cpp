///
/// EntityEditor.cpp
/// supercluster
///
/// Refer to LICENSE.txt for more details.
///

#include <galaxy/core/ServiceLocator.hpp>
#include <galaxy/fs/FileSystem.hpp>

#include <galaxy/components/Animated.hpp>
#include <galaxy/components/BatchedSprite.hpp>
#include <galaxy/components/Circle.hpp>
#include <galaxy/components/Line.hpp>
#include <galaxy/components/OnEvent.hpp>
#include <galaxy/components/Physics.hpp>
#include <galaxy/components/Point.hpp>
#include <galaxy/components/Renderable.hpp>
#include <galaxy/components/ShaderID.hpp>
#include <galaxy/components/Tag.hpp>
#include <galaxy/components/Text.hpp>
#include <galaxy/components/Transform.hpp>

#include <imgui/imgui_stdlib.h>

#include "EntityEditor.hpp"

using namespace galaxy;

namespace sc
{
	namespace panel
	{
		EntityEditor::EntityEditor()
		    : m_cur_scene {nullptr}, m_selected {std::nullopt}
		{
		}

		EntityEditor::~EntityEditor()
		{
			m_cur_scene = nullptr;
			m_selected  = std::nullopt;
		}

		void EntityEditor::pre_render()
		{
		}

		void EntityEditor::render()
		{
			if (ImGui::Begin("Entity Manager", NULL, ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_AlwaysAutoResize))
			{
				if (ImGui::BeginMenuBar())
				{
					if (ImGui::MenuItem("Create"))
					{
						m_cur_scene->world().create();
					}

					if (ImGui::MenuItem("Create from JSON"))
					{
						const auto file = SL_HANDLE.vfs()->show_open_dialog("*.json");
						m_cur_scene->world().create_from_json(file);
					}

					ImGui::EndMenuBar();
				}

				static std::string s_entity_label;
				auto& world = m_cur_scene->world();
				world.each([&](const ecs::Entity entity) {
					auto* tag = world.get<components::Tag>(entity);
					if (tag)
					{
						s_entity_label = tag->m_tag;
					}
					else
					{
						s_entity_label = "Unknown";
					}

					ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_SpanAvailWidth;
					if (m_selected != std::nullopt)
					{
						if (m_selected.value() == entity)
						{
							flags |= ImGuiTreeNodeFlags_Selected;
						}
					}

					const bool is_open = ImGui::TreeNodeEx(reinterpret_cast<void*>(entity), flags, s_entity_label.c_str());
					if (ImGui::IsItemClicked())
					{
						m_selected = std::make_optional(entity);
					}

					if (is_open)
					{
						ImGui::Text(fmt::format("Numeric ID: {0}.", entity).c_str());

						bool enabled = world.is_enabled(entity);
						if (ImGui::Checkbox("Is Enabled?", &enabled))
						{
							if (enabled)
							{
								world.enable(entity);
							}
							else
							{
								world.disable(entity);
							}
						}

						if (ImGui::Button("Add Animated"))
						{
							world.create_component<components::Animated>(entity);
						}

						ImGui::SameLine();

						if (ImGui::Button("Remove Animated"))
						{
							world.remove<components::Animated>(entity);
						}

						if (ImGui::Button("Add BatchedSprite"))
						{
							world.create_component<components::BatchedSprite>(entity);
						}

						ImGui::SameLine();

						if (ImGui::Button("Remove BatchedSprite"))
						{
							world.remove<components::BatchedSprite>(entity);
						}

						if (ImGui::Button("Add Circle"))
						{
							world.create_component<components::Circle>(entity);
						}

						ImGui::SameLine();

						if (ImGui::Button("Remove Circle"))
						{
							world.remove<components::Circle>(entity);
						}

						if (ImGui::Button("Add Line"))
						{
							world.create_component<components::Line>(entity);
						}

						ImGui::SameLine();

						if (ImGui::Button("Remove Line"))
						{
							world.remove<components::Line>(entity);
						}

						if (ImGui::Button("Add Point"))
						{
							world.create_component<components::Point>(entity);
						}

						ImGui::SameLine();

						if (ImGui::Button("Remove Point"))
						{
							world.remove<components::Point>(entity);
						}

						if (ImGui::Button("Add Renderable"))
						{
							world.create_component<components::Renderable>(entity);
						}

						ImGui::SameLine();

						if (ImGui::Button("Remove Renderable"))
						{
							world.remove<components::Renderable>(entity);
						}

						if (ImGui::Button("Add ShaderID"))
						{
							world.create_component<components::ShaderID>(entity);
						}

						ImGui::SameLine();

						if (ImGui::Button("Remove ShaderID"))
						{
							world.remove<components::ShaderID>(entity);
						}

						if (ImGui::Button("Add Sprite"))
						{
							world.create_component<components::Sprite>(entity);
						}

						ImGui::SameLine();

						if (ImGui::Button("Remove Sprite"))
						{
							world.remove<components::Sprite>(entity);
						}

						if (ImGui::Button("Add Tag"))
						{
							world.create_component<components::Tag>(entity);
						}

						ImGui::SameLine();

						if (ImGui::Button("Remove Tag"))
						{
							world.remove<components::Tag>(entity);
						}

						if (ImGui::Button("Add Text"))
						{
							world.create_component<components::Text>(entity);
						}

						ImGui::SameLine();

						if (ImGui::Button("Remove Text"))
						{
							world.remove<components::Text>(entity);
						}

						if (ImGui::Button("Add Transform"))
						{
							world.create_component<components::Transform>(entity);
						}

						ImGui::SameLine();

						if (ImGui::Button("Remove Transform"))
						{
							world.remove<components::Transform>(entity);
						}

						ImGui::TreePop();
					}
				});

				if (ImGui::Begin("Component Editor", NULL, ImGuiWindowFlags_AlwaysAutoResize))
				{
					if (m_selected != std::nullopt)
					{
						const auto entity = m_selected.value();
						render_components(entity);
					}
				}

				ImGui::End();
			}

			ImGui::End();
		}

		void EntityEditor::set_scene(core::Scene* scene)
		{
			m_cur_scene = scene;
		}

		void EntityEditor::render_components(const ecs::Entity entity)
		{
			// clang-format off
			auto [animated, batchedsprite, circle, line, physics, point, renderable, shaderid, sprite, tag, text, transform]
			= m_cur_scene->world().get_multi<components::Animated, components::BatchedSprite, components::Circle, 
			components::Line, components::Physics, components::Point, components::Renderable, components::ShaderID, 
			components::Sprite, components::Tag, components::Text, components::Transform>(entity);
			// clang-format on

			if (ImGui::BeginTabBar("EntityComponentsTabBar", ImGuiTabBarFlags_NoCloseWithMiddleMouseButton | ImGuiTabBarFlags_Reorderable))
			{
				if (animated)
				{
					if (ImGui::BeginTabItem("Animated"))
					{
						ImGui::EndTabItem();
					}
				}

				if (batchedsprite)
				{
					if (ImGui::BeginTabItem("Batched Sprite"))
					{
						ImGui::EndTabItem();
					}
				}

				if (circle)
				{
					if (ImGui::BeginTabItem("Circle"))
					{
						ImGui::EndTabItem();
					}
				}

				if (line)
				{
					if (ImGui::BeginTabItem("Line"))
					{
						ImGui::EndTabItem();
					}
				}

				if (physics)
				{
					if (ImGui::BeginTabItem("Physics"))
					{
						ImGui::EndTabItem();
					}
				}

				if (point)
				{
					if (ImGui::BeginTabItem("Point"))
					{
						ImGui::EndTabItem();
					}
				}

				if (renderable)
				{
					if (ImGui::BeginTabItem("Renderable"))
					{
						ImGui::EndTabItem();
					}
				}

				if (shaderid)
				{
					if (ImGui::BeginTabItem("ShaderID"))
					{
						ImGui::EndTabItem();
					}
				}

				if (sprite)
				{
					if (ImGui::BeginTabItem("Sprite"))
					{
						ImGui::EndTabItem();
					}
				}

				if (tag)
				{
					if (ImGui::BeginTabItem("Tag"))
					{
						ImGui::InputText("Tag", &tag->m_tag, ImGuiInputTextFlags_AutoSelectAll | ImGuiInputTextFlags_EnterReturnsTrue);
						ImGui::EndTabItem();
					}
				}

				if (text)
				{
					if (ImGui::BeginTabItem("Text"))
					{
						ImGui::EndTabItem();
					}
				}

				if (transform)
				{
					if (ImGui::BeginTabItem("Transform"))
					{
						ImGui::EndTabItem();
					}
				}

				ImGui::EndTabBar();
			}
		}
	} // namespace panel
} // namespace sc