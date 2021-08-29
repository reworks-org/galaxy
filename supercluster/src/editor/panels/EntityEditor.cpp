///
/// EntityEditor.cpp
/// supercluster
///
/// Refer to LICENSE.txt for more details.
///

#include <galaxy/components/Actions.hpp>
#include <galaxy/components/Animated.hpp>
#include <galaxy/components/BatchSprite.hpp>
#include <galaxy/components/OnCollision.hpp>
#include <galaxy/components/ParticleEffect.hpp>
#include <galaxy/components/Primitive2D.hpp>
#include <galaxy/components/Renderable.hpp>
#include <galaxy/components/RigidBody.hpp>
#include <galaxy/components/Sprite.hpp>
#include <galaxy/components/Tag.hpp>
#include <galaxy/components/Text.hpp>
#include <galaxy/components/Transform2D.hpp>
#include <galaxy/core/ServiceLocator.hpp>
#include <galaxy/flags/AllowSerialize.hpp>
#include <galaxy/flags/Enabled.hpp>
#include <galaxy/fs/FileSystem.hpp>
#include <galaxy/ui/ImGuiHelpers.hpp>

#include "EntityEditor.hpp"

using namespace galaxy;

namespace sc
{
	namespace panel
	{
		EntityEditor::EntityEditor() noexcept
			: m_selected {std::nullopt}
			, m_label {"Unnamed"}
		{
		}

		EntityEditor::~EntityEditor() noexcept
		{
		}

		void EntityEditor::render(core::Scene2D* top, OpenGLOperationStack& gl_operations)
		{
			if (ImGui::Begin("Entity Manager", NULL, ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_AlwaysAutoResize))
			{
				if (ImGui::BeginMenuBar())
				{
					if (ImGui::MenuItem("Create"))
					{
						top->m_world.create();
					}

					if (ImGui::MenuItem("Create from JSON"))
					{
						const auto file = SL_HANDLE.vfs()->show_open_dialog("*.json");
						gl_operations.push_back(
							[file, top]()
							{
								if (file == std::nullopt)
								{
									GALAXY_LOG(GALAXY_ERROR, "Failed to open json for creating an entity.");
								}
								else
								{
									top->m_world.create_from_json(file.value());
								}
							});
					}

					ImGui::EndMenuBar();
				}

				top->m_world.each(
					[&](const ecs::Entity entity)
					{
						auto* tag = top->m_world.get<components::Tag>(entity);
						if (tag)
						{
							m_label = tag->m_tag;
						}
						else
						{
							m_label = "Unnamed";
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

						const bool is_open = ImGui::TreeNodeEx(reinterpret_cast<void*>(entity), flags, m_label.c_str());
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

							bool enabled = top->m_world.is_enabled(entity);
							if (ImGui::Checkbox("Enabled?", &enabled))
							{
								if (enabled)
								{
									top->m_world.enable(entity);
								}
								else
								{
									top->m_world.disable(entity);
								}
							}

							ImGui::SameLine();
							ImGui::Spacing();
							ImGui::SameLine();

							bool allow_serialize = top->m_world.is_flag_set<flags::AllowSerialize>(entity);
							if (ImGui::Checkbox("Serialize?", &allow_serialize))
							{
								if (allow_serialize)
								{
									top->m_world.set_flag<flags::AllowSerialize>(entity);
								}
								else
								{
									top->m_world.unset_flag<flags::AllowSerialize>(entity);
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
									top->m_world.disable(entity);
									top->m_world.create_component<components::Animated>(entity);
								}

								ImGui::TableNextColumn();

								if (ImGui::Button(" - ##2"))
								{
									top->m_world.remove<components::Animated>(entity);
								}

								ImGui::TableNextRow();
								ImGui::TableNextColumn();
								ImGui::Text("On Collision");
								ImGui::TableNextColumn();

								if (ImGui::Button(" + ##3"))
								{
									top->m_world.disable(entity);
									top->m_world.create_component<components::OnCollision>(entity);
								}

								ImGui::TableNextColumn();

								if (ImGui::Button(" - ##4"))
								{
									top->m_world.remove<components::OnCollision>(entity);
								}

								ImGui::TableNextRow();
								ImGui::TableNextColumn();
								ImGui::Text("2D Primitive");
								ImGui::TableNextColumn();

								if (ImGui::Button(" + ##5"))
								{
									top->m_world.disable(entity);
									top->m_world.create_component<components::Primitive2D>(entity);
								}

								ImGui::TableNextColumn();

								if (ImGui::Button(" - ##6"))
								{
									top->m_world.remove<components::Primitive2D>(entity);
								}

								ImGui::TableNextRow();
								ImGui::TableNextColumn();
								ImGui::Text("Rigid Body");
								ImGui::TableNextColumn();

								if (ImGui::Button(" + ##7"))
								{
									top->m_world.disable(entity);
									top->m_world.create_component<components::RigidBody>(entity);
								}

								ImGui::TableNextColumn();

								if (ImGui::Button(" - ##8"))
								{
									top->m_world.remove<components::RigidBody>(entity);
								}

								ImGui::TableNextRow();
								ImGui::TableNextColumn();
								ImGui::Text("Tag");
								ImGui::TableNextColumn();

								if (ImGui::Button(" + ##9"))
								{
									top->m_world.disable(entity);
									top->m_world.create_component<components::Tag>(entity);
								}

								ImGui::TableNextColumn();

								if (ImGui::Button(" - ##10"))
								{
									top->m_world.remove<components::Tag>(entity);
								}

								ImGui::TableNextRow();
								ImGui::TableNextColumn();
								ImGui::Text("Transform2D");
								ImGui::TableNextColumn();

								if (ImGui::Button(" + ##11"))
								{
									top->m_world.disable(entity);
									top->m_world.create_component<components::Transform2D>(entity);
								}

								ImGui::TableNextColumn();

								if (ImGui::Button(" - ##12"))
								{
									top->m_world.remove<components::Transform2D>(entity);
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
								ImGui::Text("Action");
								ImGui::TableNextColumn();

								if (ImGui::Button(" + ##13"))
								{
									top->m_world.disable(entity);
									top->m_world.create_component<components::Actions>(entity);
								}

								ImGui::TableNextColumn();

								if (ImGui::Button(" - ##14"))
								{
									top->m_world.remove<components::Actions>(entity);
								}

								ImGui::TableNextRow();
								ImGui::TableNextColumn();
								ImGui::Text("Batch Sprite");
								ImGui::TableNextColumn();

								if (ImGui::Button(" + ##15"))
								{
									top->m_world.disable(entity);
									top->m_world.create_component<components::BatchSprite>(entity);
								}

								ImGui::TableNextColumn();

								if (ImGui::Button(" - ##16"))
								{
									top->m_world.remove<components::BatchSprite>(entity);
								}

								ImGui::TableNextRow();
								ImGui::TableNextColumn();
								ImGui::Text("Renderable");
								ImGui::TableNextColumn();

								if (ImGui::Button(" + ##17"))
								{
									top->m_world.disable(entity);
									top->m_world.create_component<components::Renderable>(entity);
								}

								ImGui::TableNextColumn();

								if (ImGui::Button(" - ##18"))
								{
									top->m_world.remove<components::Renderable>(entity);
								}

								ImGui::TableNextRow();
								ImGui::TableNextColumn();
								ImGui::Text("Sprite2D");
								ImGui::TableNextColumn();

								if (ImGui::Button(" + ##19"))
								{
									top->m_world.disable(entity);
									top->m_world.create_component<components::Sprite>(entity);
								}

								ImGui::TableNextColumn();

								if (ImGui::Button(" - ##20"))
								{
									top->m_world.remove<components::Sprite>(entity);
								}

								ImGui::TableNextRow();
								ImGui::TableNextColumn();
								ImGui::Text("Text");
								ImGui::TableNextColumn();

								if (ImGui::Button(" + ##21"))
								{
									top->m_world.disable(entity);

									// clang-format off
									auto* world = &top->m_world;
									gl_operations.emplace_back([world, entity]() {
										world->create_component<components::Text>(entity);
									});
									// clang-format on
								}

								ImGui::TableNextColumn();

								if (ImGui::Button(" - ##22"))
								{
									// clang-format off
									auto* world = &top->m_world;
									gl_operations.emplace_back([world, entity]() {
										world->remove<components::Text>(entity);
									});
									// clang-format on
								}

								ImGui::EndTable();
							}

							ImGui::TreePop();
						}
					});

				if (ImGui::IsMouseDown(0) && ImGui::IsWindowHovered())
				{
					m_selected = std::nullopt;
				}

				if (ImGui::Begin("Component Editor", NULL, ImGuiWindowFlags_AlwaysAutoResize))
				{
					if (m_selected != std::nullopt)
					{
						render_components(top, m_selected.value(), gl_operations);
					}
				}

				ImGui::End();
			}

			ImGui::End();
		}

		void EntityEditor::set_selected_entity(std::optional<ecs::Entity> entity)
		{
			m_selected = entity;
		}

		void EntityEditor::render_components(core::Scene2D* top, const ecs::Entity entity, OpenGLOperationStack& gl_operations)
		{
			// clang-format off
			auto [actions, animated, batchsprite,
				oncollision, particleffect, primitive2d,
				renderable, rigidbody, sprite, tag,
				text, transform2d] =
				top->m_world.get_multi<
				components::Actions,
				components::Animated,
				components::BatchSprite,
				components::OnCollision,
				components::ParticleEffect,
				components::Primitive2D,
				components::Renderable,
				components::RigidBody,
				components::Sprite,
				components::Tag,
				components::Text,
				components::Transform2D>(entity);
			// clang-format on

			if (ImGui::BeginTabBar("EntityComponentsTabBar", ImGuiTabBarFlags_NoCloseWithMiddleMouseButton | ImGuiTabBarFlags_Reorderable))
			{
				if (actions)
				{
					if (ImGui::BeginTabItem("Actions"))
					{
						ImGui::EndTabItem();
					}
				}

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

				if (particleffect)
				{
					if (ImGui::BeginTabItem("Particle Effect"))
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
		}
	} // namespace panel
} // namespace sc