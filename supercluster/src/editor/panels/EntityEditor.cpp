///
/// EntityEditor.cpp
/// supercluster
///
/// Refer to LICENSE.txt for more details.
///

#include <galaxy/core/ServiceLocator.hpp>
#include <galaxy/fs/FileSystem.hpp>
#include <galaxy/ui/ImGuiHelpers.hpp>

#include "EntityEditor.hpp"

using namespace galaxy;

namespace sc
{
	namespace panel
	{
		EntityEditor::EntityEditor() noexcept
		{
		}

		EntityEditor::~EntityEditor() noexcept
		{
		}

		void EntityEditor::render(core::Scene2D* top_scene, OpenGLOperationStack& gl_operations)
		{
			if (ImGui::Begin("Entity Manager", NULL, ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_AlwaysAutoResize))
			{
				if (ImGui::BeginMenuBar())
				{
					if (ImGui::MenuItem("Create"))
					{
						//m_cur_layer->get_stack().top()->m_world.create();
					}

					if (ImGui::MenuItem("Create from JSON"))
					{
						const auto file = SL_HANDLE.vfs()->show_open_dialog("*.json");
						gl_operations.push_back(
							[this, file]()
							{
								if (file == std::nullopt)
								{
									GALAXY_LOG(GALAXY_ERROR, "Failed to open json for creating an entity.");
								}
								else
								{
									//m_cur_layer->get_stack().top()->m_world.create_from_json(file.value());
								}
							});
					}

					ImGui::EndMenuBar();
				}

				static std::string s_entity_label;
				/*
				auto& world = m_cur_layer->get_stack().top()->m_world;
				world.each(
					[&](const ecs::Entity entity)
					{
						auto* tag = world.get<components::Tag>(entity);
						if (tag)
						{
							s_entity_label = tag->m_tag;
						}
						else
						{
							s_entity_label = "Unknown";
						}

						ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_SpanAvailWidth | ImGuiTreeNodeFlags_OpenOnDoubleClick;
						if (m_selected != std::nullopt)
						{
							if (m_selected.value() == entity)
							{
								flags |= ImGuiTreeNodeFlags_Selected;
							}
						}

						if (!m_selected || m_selected.value() != entity)
						{
							ImGui::SetNextItemOpen(false);
						}

						const bool is_open = ImGui::TreeNodeEx(reinterpret_cast<void*>(entity), flags, s_entity_label.c_str());
						if (ImGui::IsItemClicked())
						{
							m_selected = std::make_optional(entity);
						}

						if (is_open)
						{
							ImGui::Text(std::format("ID: {0}.", entity).c_str());

							ImGui::Spacing();
							ImGui::Separator();
							ImGui::Spacing();

							bool enabled = world.is_enabled(entity);
							if (ImGui::Checkbox("Enabled?", &enabled))
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

							ImGui::SameLine();
							ImGui::Spacing();
							ImGui::SameLine();

							bool allow_serialize = world.is_flag_set<flags::AllowSerialize>(entity);
							if (ImGui::Checkbox("Serialize?", &allow_serialize))
							{
								if (allow_serialize)
								{
									world.set_flag<flags::AllowSerialize>(entity);
								}
								else
								{
									world.unset_flag<flags::AllowSerialize>(entity);
								}
							}

							ImGui::Spacing();
							ImGui::Separator();
							ImGui::Spacing();

							if (ImGui::BeginTable("AddRemoveTable1", 4, ImGuiTableFlags_NoBordersInBody))
							{
								ImGui::TableNextRow();
								ImGui::TableNextColumn();
								ImGui::Text("Name");
								ImGui::TableNextColumn();
								ImGui::Text("Add");
								ImGui::TableNextColumn();
								ImGui::Text("Remove");

								ImGui::TableNextRow();
								ImGui::TableNextColumn();
								ImGui::Text("Animated");
								ImGui::TableNextColumn();

								if (ImGui::Button(" + ##1"))
								{
									world.disable(entity);
									world.create_component<components::Animated>(entity);
								}

								ImGui::TableNextColumn();

								if (ImGui::Button(" - ##2"))
								{
									world.remove<components::Animated>(entity);
								}

								ImGui::TableNextRow();
								ImGui::TableNextColumn();
								ImGui::Text("On Collision");
								ImGui::TableNextColumn();

								if (ImGui::Button(" + ##5"))
								{
									world.disable(entity);
									world.create_component<components::OnCollision>(entity);
								}

								ImGui::TableNextColumn();

								if (ImGui::Button(" - ##6"))
								{
									world.remove<components::OnCollision>(entity);
								}

								ImGui::TableNextRow();
								ImGui::TableNextColumn();
								ImGui::Text("2D Primitive");
								ImGui::TableNextColumn();

								if (ImGui::Button(" + ##7"))
								{
									world.disable(entity);
									world.create_component<components::Primitive2D>(entity);
								}

								ImGui::TableNextColumn();

								if (ImGui::Button(" - ##8"))
								{
									world.remove<components::Primitive2D>(entity);
								}

								ImGui::TableNextRow();
								ImGui::TableNextColumn();
								ImGui::Text("Rigid Body");
								ImGui::TableNextColumn();

								if (ImGui::Button(" + ##9"))
								{
									world.disable(entity);
									world.create_component<components::RigidBody>(entity);
								}

								ImGui::TableNextColumn();

								if (ImGui::Button(" - ##10"))
								{
									world.remove<components::RigidBody>(entity);
								}

								ImGui::TableNextRow();
								ImGui::TableNextColumn();
								ImGui::Text("Tag");
								ImGui::TableNextColumn();

								if (ImGui::Button(" + ##13"))
								{
									world.disable(entity);
									world.create_component<components::Tag>(entity);
								}

								ImGui::TableNextColumn();

								if (ImGui::Button(" - ##14"))
								{
									world.remove<components::Tag>(entity);
								}

								ImGui::TableNextRow();
								ImGui::TableNextColumn();
								ImGui::Text("Transform2D");
								ImGui::TableNextColumn();

								if (ImGui::Button(" + ##15"))
								{
									world.disable(entity);
									world.create_component<components::Transform2D>(entity);
								}

								ImGui::TableNextColumn();

								if (ImGui::Button(" - ##16"))
								{
									world.remove<components::Transform2D>(entity);
								}

								ImGui::EndTable();
							}

							ImGui::SetCursorPosX(230.0f);
							ImGui::SetCursorPosY(143.0f);

							if (ImGui::BeginTable("AddRemoveTable2", 4, ImGuiTableFlags_NoBordersInBody))
							{
								ImGui::TableNextRow();
								ImGui::TableNextColumn();
								ImGui::Text("Name");
								ImGui::TableNextColumn();
								ImGui::Text("Add");
								ImGui::TableNextColumn();
								ImGui::Text("Remove");

								ImGui::TableNextRow();
								ImGui::TableNextColumn();
								ImGui::Text("Batch Sprite");
								ImGui::TableNextColumn();

								if (ImGui::Button(" + ##17"))
								{
									world.disable(entity);
									world.create_component<components::BatchSprite>(entity);
								}

								ImGui::TableNextColumn();

								if (ImGui::Button(" - ##18"))
								{
									world.remove<components::BatchSprite>(entity);
								}

								ImGui::TableNextRow();
								ImGui::TableNextColumn();
								ImGui::Text("Renderable");
								ImGui::TableNextColumn();

								if (ImGui::Button(" + ##23"))
								{
									world.disable(entity);
									world.create_component<components::Renderable>(entity);
								}

								ImGui::TableNextColumn();

								if (ImGui::Button(" - ##24"))
								{
									world.remove<components::Renderable>(entity);
								}

								ImGui::TableNextRow();
								ImGui::TableNextColumn();
								ImGui::Text("Sprite2D");
								ImGui::TableNextColumn();

								if (ImGui::Button(" + ##27"))
								{
									world.disable(entity);
									world.create_component<components::Sprite>(entity);
								}

								ImGui::TableNextColumn();

								if (ImGui::Button(" - ##28"))
								{
									world.remove<components::Sprite>(entity);
								}

								ImGui::TableNextRow();
								ImGui::TableNextColumn();
								ImGui::Text("Text");
								ImGui::TableNextColumn();

								if (ImGui::Button(" + ##29"))
								{
									world.disable(entity);
									gl_operations.emplace_back(
										[&world, entity]() -> void
										{
											world.create_component<components::Text>(entity);
										});
								}

								ImGui::TableNextColumn();

								if (ImGui::Button(" - ##30"))
								{
									gl_operations.emplace_back(
										[&world, entity]() -> void
										{
											world.remove<components::Text>(entity);
										});
								}

								ImGui::EndTable();
							}

							ImGui::TreePop();
						}
					});

				*/

				/*
				if (ImGui::IsMouseDown(0) && ImGui::IsWindowHovered())
				{
					m_selected = std::nullopt;
				}

				if (ImGui::Begin("Component Editor", NULL, ImGuiWindowFlags_AlwaysAutoResize))
				{
					if (m_selected != std::nullopt)
					{
						const auto entity = m_selected.value();
						render_components(entity, gl_operations);
					}
				}

				ImGui::End();
				*/
			}

			ImGui::End();
		}

		void EntityEditor::render_components(const ecs::Entity entity, OpenGLOperationStack& gl_operations)
		{
			/*
			// clang-format off
			auto [animated,
				batchsprite,
				oncollision,
				primitive2d,
				renderable,
				rigidbody,
				sprite,
				tag,
				text,
				transform2d
			] = m_cur_layer->get_stack().top()->m_world.get_multi<
				components::Animated,
				components::BatchSprite,
				components::OnCollision,
				components::Primitive2D,
				components::Renderable,
				components::RigidBody,
				components::Sprite,
				components::Tag,
				components::Text,
				components::Transform2D
			>(entity);
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

				if (batchsprite)
				{
					if (ImGui::BeginTabItem("Batch Sprite"))
					{
						ImGui::EndTabItem();
					}
				}

				if (oncollision)
				{
					if (ImGui::BeginTabItem("On Collision"))
					{
						ImGui::EndTabItem();
					}
				}

				if (primitive2d)
				{
					if (ImGui::BeginTabItem("Primitive2D"))
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

				if (rigidbody)
				{
					if (ImGui::BeginTabItem("Rigid Body"))
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

				if (transform2d)
				{
					if (ImGui::BeginTabItem("Transform2D"))
					{
						ImGui::EndTabItem();
					}
				}

				ImGui::EndTabBar();
			}
			*/
		}
	} // namespace panel
} // namespace sc