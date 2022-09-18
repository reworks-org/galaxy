///
/// EntityEditor.cpp
/// supercluster
///
/// Refer to LICENSE.txt for more details.
///

#include <magic_enum.hpp>
#include <imgui/addons/ToggleButton.h>
#include <imgui/imgui_stdlib.h>

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
						gl_operations.push_back([file, top]() {
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

				top->m_world.each([&](const ecs::Entity entity) {
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

								auto* world = &top->m_world;
								gl_operations.emplace_back([world, entity]() {
									world->create_component<components::Text>(entity);
								});
							}

							ImGui::TableNextColumn();

							if (ImGui::Button(" - ##22"))
							{
								auto* world = &top->m_world;
								gl_operations.emplace_back([world, entity]() {
									world->remove<components::Text>(entity);
								});
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
			auto [actions, animated, batchsprite, oncollision, particleffect, primitive2d, renderable, rigidbody, sprite, tag, text, transform2d] =
				top->m_world.get_multi<components::Actions,
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

			if (ImGui::BeginTabBar("EntityComponentsTabBar", ImGuiTabBarFlags_NoCloseWithMiddleMouseButton | ImGuiTabBarFlags_Reorderable))
			{
				if (actions)
				{
					if (ImGui::BeginTabItem("Actions"))
					{
						if (ImGui::Button("New"))
						{
							ImGui::OpenPopup("NewActionsPopup");
						}

						if (ImGui::BeginPopup("NewActionsPopup", ImGuiWindowFlags_AlwaysAutoResize))
						{
							static std::string s_func;
							ImGui::InputText("Lua Function", &s_func, ImGuiInputTextFlags_AutoSelectAll | ImGuiInputTextFlags_CharsNoBlank);

							static bool s_mode = false;

							// Key = false, MouseButton = true.
							ImGui::Text("Key Action");
							ImGui::SameLine();
							ImGui::ToggleButton("##NewActionsPopupMode", &s_mode);
							ImGui::SameLine();
							ImGui::Text("MouseButton Action");

							if (!s_mode)
							{
								static constexpr auto& keys  = magic_enum::enum_names<input::Keys>();
								static const char* s_current = "...";
								if (ImGui::BeginCombo("Keys", s_current))
								{
									for (const auto& key : keys)
									{
										bool is_selected = std::strcmp(s_current, key.data());
										if (ImGui::Selectable(key.data(), is_selected))
										{
											s_current = key.data();
										}

										if (is_selected)
										{
											ImGui::SetItemDefaultFocus();
										}
									}

									ImGui::EndCombo();
								}

								if (ImGui::Button("Create"))
								{
									const auto& input = magic_enum::enum_cast<input::Keys>(s_current);
									if (input.has_value())
									{
										actions->add_key_action(input.value(), s_func);
										ImGui::CloseCurrentPopup();
									}
									else
									{
										GALAXY_LOG(GALAXY_ERROR, "Failed to convert key string to key enum.");
									}
								}
							}
							else
							{
								constexpr auto& mouse_buttons = magic_enum::enum_names<input::MouseButtons>();
								static const char* s_current  = "...";
								if (ImGui::BeginCombo("Mouse Buttons", s_current))
								{
									for (const auto& mb : mouse_buttons)
									{
										bool is_selected = std::strcmp(s_current, mb.data());
										if (ImGui::Selectable(mb.data(), is_selected))
										{
											s_current = mb.data();
										}

										if (is_selected)
										{
											ImGui::SetItemDefaultFocus();
										}
									}

									ImGui::EndCombo();
								}

								if (ImGui::Button("Create"))
								{
									const auto& input = magic_enum::enum_cast<input::MouseButtons>(s_current);
									if (input.has_value())
									{
										actions->add_mouse_action(input.value(), s_func);
										ImGui::CloseCurrentPopup();
									}
									else
									{
										GALAXY_LOG(GALAXY_ERROR, "Failed to convert key string to key enum.");
									}
								}
							}

							ImGui::EndPopup();
						}

						static auto s_selected_key = input::Keys::UNDEFINED;
						for (const auto& [key, func] : actions->m_key_actions)
						{
							ImGuiTreeNodeFlags flags =
								ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_SpanAvailWidth | ImGuiTreeNodeFlags_OpenOnDoubleClick;
							if (s_selected_key == key)
							{
								flags |= ImGuiTreeNodeFlags_Selected;
							}
							else
							{
								ImGui::SetNextItemOpen(false);
							}

							const char* key_name = magic_enum::enum_name(key).data();
							const bool is_open   = ImGui::TreeNodeEx(reinterpret_cast<void*>(key), flags, key_name);
							if (ImGui::IsItemClicked())
							{
								s_selected_key = key;
							}

							if (is_open)
							{
								ImGui::Text(std::format("Key: {0}, Lua Function: {1}.", key_name, func).c_str());

								if (ImGui::Button("Delete"))
								{
									actions->m_key_actions.erase(key);
								}
							}
						}

						ImGui::Spacing();

						static auto s_selected_mb = input::MouseButtons::UNDEFINED;
						for (const auto& [mb, func] : actions->m_mouse_actions)
						{
							ImGuiTreeNodeFlags flags =
								ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_SpanAvailWidth | ImGuiTreeNodeFlags_OpenOnDoubleClick;
							if (s_selected_mb == mb)
							{
								flags |= ImGuiTreeNodeFlags_Selected;
							}
							else
							{
								ImGui::SetNextItemOpen(false);
							}

							const char* mb_name = magic_enum::enum_name(mb).data();
							const bool is_open  = ImGui::TreeNodeEx(reinterpret_cast<void*>(mb), flags, mb_name);
							if (ImGui::IsItemClicked())
							{
								s_selected_mb = mb;
							}

							if (is_open)
							{
								ImGui::Text(std::format("Mouse Button: {0}, Lua Function: {1}.", mb_name, func).c_str());

								if (ImGui::Button("Delete"))
								{
									actions->m_mouse_actions.erase(mb);
								}
							}
						}

						ImGui::EndTabItem();
					}
				}

				if (animated)
				{
					if (ImGui::BeginTabItem("Animated"))
					{
						static bool s_add = false;
						if (ImGui::Button("Add"))
						{
							s_add = !s_add;
							ImGui::OpenPopup("AddAnimation");
						}

						if (s_add)
						{
							static std::string s_id;
							static bool s_loop                           = false;
							static float s_speed                         = 1.0f;
							static std::vector<graphics::Frame> s_frames = {};

							if (ImGui::BeginPopup("AddAnimation", ImGuiWindowFlags_AlwaysAutoResize))
							{
								ImGui::InputText("ID",
									&s_id,
									ImGuiInputTextFlags_AutoSelectAll | ImGuiInputTextFlags_CharsNoBlank | ImGuiInputTextFlags_EnterReturnsTrue);
								ImGui::Checkbox("Is Looping?", &s_loop);
								ImGui::SliderFloat("Speed", &s_speed, 0.1f, 10.0f, "%.1f", ImGuiSliderFlags_AlwaysClamp);

								static bool s_add_frame = false;
								if (ImGui::Button("Add Frame"))
								{
									s_add_frame = !s_add_frame;
								}

								if (s_add_frame)
								{
									if (ImGui::BeginPopup("Add Frame", ImGuiWindowFlags_AlwaysAutoResize))
									{
										static graphics::Frame s_frame;
										static std::string s_tex_id;
										static double s_tpf = 0.1;

										ImGui::InputText("Texture Atlas ID",
											&s_tex_id,
											ImGuiInputTextFlags_AutoSelectAll | ImGuiInputTextFlags_CharsNoBlank | ImGuiInputTextFlags_EnterReturnsTrue);

										const static double s_min = 0.1;
										const static double s_max = 10.0;
										ImGui::SliderScalar("Time Per Frame", ImGuiDataType_Double, &s_tpf, &s_min, &s_max, "%.1f");

										if (ImGui::Button("Add##frameaddbutton01"))
										{
											s_frame.set_region(s_tex_id);
											s_frame.m_time_per_frame = s_tpf;
											s_frames.push_back(s_frame);

											s_frame  = {};
											s_tex_id = "";
											s_tpf    = 0.1;

											ImGui::CloseCurrentPopup();
										}

										ImGui::EndPopup();
									}
								}

								if (ImGui::Button("Add"))
								{
									animated->add_animation(s_id, s_id, s_loop, static_cast<double>(s_speed), s_frames);

									s_id    = "";
									s_loop  = false;
									s_speed = 1.0f;
									s_frames.clear();

									ImGui::CloseCurrentPopup();
								}

								ImGui::EndPopup();
							}
						}

						static std::string s_selected = "None";
						if (animated->get_cur_animation())
						{
							s_selected = animated->get_cur_animation()->get_name();
						}

						if (ImGui::BeginCombo("Animation", s_selected.c_str()))
						{
							for (const auto& [name, anim] : animated->get_all())
							{
								const bool selected = (s_selected == name);
								if (ImGui::Selectable(name.c_str(), selected))
								{
									s_selected = name;
									animated->set_animation(s_selected);
								}

								if (selected)
								{
									ImGui::SetItemDefaultFocus();
								}
							}

							ImGui::EndCombo();
						}

						ImGui::LabelText("Current Animation", s_selected.c_str());

						if (ImGui::Button("Play"))
						{
							animated->play();
						}

						ImGui::SameLine();

						if (ImGui::Button("Pause"))
						{
							animated->pause();
						}

						ImGui::SameLine();

						if (ImGui::Button("Stop"))
						{
							animated->stop();
						}

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