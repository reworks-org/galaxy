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
#include <galaxy/components/Polygon.hpp>
#include <galaxy/components/Renderable.hpp>
#include <galaxy/components/ShaderID.hpp>
#include <galaxy/components/Tag.hpp>
#include <galaxy/components/Text.hpp>
#include <galaxy/components/Transform.hpp>

#include <imgui/imgui_stdlib.h>
#include <magic_enum.hpp>

#include "EntityEditor.hpp"

using namespace galaxy;

namespace sc
{
	namespace panel
	{
		EntityEditor::EntityEditor()
		{
		}

		EntityEditor::~EntityEditor()
		{
			m_cur_scene = nullptr;
			m_selected  = std::nullopt;
		}

		void EntityEditor::render(OpenGLOperationStack& gl_operations)
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
						gl_operations.push_back([this, &file]() -> void {
							if (file == std::nullopt)
							{
								GALAXY_LOG(GALAXY_ERROR, "Failed to open json for creating an entity.");
							}
							else
							{
								m_cur_scene->world().create_from_json(file.value());
							}
						});
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
						ImGui::Text(fmt::format("ID: {0}.", entity).c_str());

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

						if (ImGui::BeginTable("AddRemoveTable", 4, ImGuiTableFlags_NoBordersInBody))
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
							ImGui::Text("Batch Sprite");
							ImGui::TableNextColumn();

							if (ImGui::Button(" + ##3"))
							{
								world.disable(entity);
								world.create_component<components::BatchedSprite>(entity);
							}

							ImGui::TableNextColumn();

							if (ImGui::Button(" - ##4"))
							{
								world.remove<components::BatchedSprite>(entity);
							}

							ImGui::TableNextRow();
							ImGui::TableNextColumn();
							ImGui::Text("Circle");
							ImGui::TableNextColumn();

							if (ImGui::Button(" + ##5"))
							{
								world.disable(entity);
								world.create_component<components::Circle>(entity);
							}

							ImGui::TableNextColumn();

							if (ImGui::Button(" - ##6"))
							{
								world.remove<components::Circle>(entity);
							}

							ImGui::TableNextRow();
							ImGui::TableNextColumn();
							ImGui::Text("Line");
							ImGui::TableNextColumn();

							if (ImGui::Button(" + ##7"))
							{
								world.disable(entity);
								world.create_component<components::Line>(entity);
							}

							ImGui::TableNextColumn();

							if (ImGui::Button(" - ##8"))
							{
								world.remove<components::Line>(entity);
							}

							ImGui::TableNextRow();
							ImGui::TableNextColumn();
							ImGui::Text("Point");
							ImGui::TableNextColumn();

							if (ImGui::Button(" + ##9"))
							{
								world.disable(entity);
								world.create_component<components::Point>(entity);
							}

							ImGui::TableNextColumn();

							if (ImGui::Button(" - ##10"))
							{
								world.remove<components::Point>(entity);
							}

							ImGui::TableNextRow();
							ImGui::TableNextColumn();
							ImGui::Text("Renderable");
							ImGui::TableNextColumn();

							if (ImGui::Button(" + ##11"))
							{
								world.disable(entity);
								world.create_component<components::Renderable>(entity);
							}

							ImGui::TableNextColumn();

							if (ImGui::Button(" - ##12"))
							{
								world.remove<components::Renderable>(entity);
							}

							ImGui::TableNextRow();
							ImGui::TableNextColumn();
							ImGui::Text("Shader ID");
							ImGui::TableNextColumn();

							if (ImGui::Button(" + ##13"))
							{
								world.disable(entity);
								world.create_component<components::ShaderID>(entity);
							}

							ImGui::TableNextColumn();

							if (ImGui::Button(" - ##14"))
							{
								world.remove<components::ShaderID>(entity);
							}

							ImGui::TableNextRow();
							ImGui::TableNextColumn();
							ImGui::Text("Sprite");
							ImGui::TableNextColumn();

							if (ImGui::Button(" + ##15"))
							{
								world.disable(entity);
								world.create_component<components::Sprite>(entity);
							}

							ImGui::TableNextColumn();

							if (ImGui::Button(" - ##16"))
							{
								world.remove<components::Sprite>(entity);
							}

							ImGui::TableNextRow();
							ImGui::TableNextColumn();
							ImGui::Text("Tag");
							ImGui::TableNextColumn();

							if (ImGui::Button(" + ##17"))
							{
								world.disable(entity);
								world.create_component<components::Tag>(entity);
							}

							ImGui::TableNextColumn();

							if (ImGui::Button(" - ##18"))
							{
								world.remove<components::Tag>(entity);
							}

							ImGui::TableNextRow();
							ImGui::TableNextColumn();
							ImGui::Text("Text");
							ImGui::TableNextColumn();

							if (ImGui::Button(" + ##19"))
							{
								world.disable(entity);
								gl_operations.emplace_back([&world, entity]() -> void {
									world.create_component<components::Text>(entity);
								});
							}

							ImGui::TableNextColumn();

							if (ImGui::Button(" - ##20"))
							{
								gl_operations.emplace_back([&world, entity]() -> void {
									world.remove<components::Text>(entity);
								});
							}

							ImGui::TableNextRow();
							ImGui::TableNextColumn();
							ImGui::Text("Transform");
							ImGui::TableNextColumn();

							if (ImGui::Button(" + ##21"))
							{
								world.disable(entity);
								world.create_component<components::Transform>(entity);
							}

							ImGui::TableNextColumn();

							if (ImGui::Button(" - ##22"))
							{
								world.remove<components::Transform>(entity);
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
						const auto entity = m_selected.value();
						render_components(entity, gl_operations);
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

		void EntityEditor::render_components(const ecs::Entity entity, OpenGLOperationStack& gl_operations)
		{
			// clang-format off
			auto [animated, batchedsprite, circle, line, physics, point, polygon, renderable, shaderid, sprite, tag, text, transform]
			= m_cur_scene->world().get_multi<components::Animated, components::BatchedSprite, components::Circle, 
			components::Line, components::Physics, components::Point, components::Polygon, components::Renderable, components::ShaderID, 
			components::Sprite, components::Tag, components::Text, components::Transform>(entity);
			// clang-format on

			if (ImGui::BeginTabBar("EntityComponentsTabBar", ImGuiTabBarFlags_NoCloseWithMiddleMouseButton | ImGuiTabBarFlags_Reorderable))
			{
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
							static std::string s_id                      = "";
							static bool s_loop                           = false;
							static float s_speed                         = 1.0f;
							static std::vector<graphics::Frame> s_frames = {};

							if (ImGui::BeginPopup("AddAnimation", ImGuiWindowFlags_AlwaysAutoResize))
							{
								ImGui::InputText("ID", &s_id, ImGuiInputTextFlags_AutoSelectAll | ImGuiInputTextFlags_CharsNoBlank | ImGuiInputTextFlags_EnterReturnsTrue);
								ImGui::Checkbox("Is Looping?", &s_loop);
								ImGui::SliderFloat("Speed", &s_speed, 0.1f, 10.0f, "%.1f", ImGuiSliderFlags_AlwaysClamp | ImGuiSliderFlags_ClampOnInput);

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
										static std::string s_tex_id = "";
										static double s_tpf         = 0.1;

										ImGui::InputText("Texture Atlas ID", &s_tex_id, ImGuiInputTextFlags_AutoSelectAll | ImGuiInputTextFlags_CharsNoBlank | ImGuiInputTextFlags_EnterReturnsTrue);

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

						if (ImGui::Button("Pause"))
						{
							animated->pause();
						}

						if (ImGui::Button("Stop"))
						{
							animated->stop();
						}

						ImGui::EndTabItem();
					}
				}

				if (batchedsprite)
				{
					if (ImGui::BeginTabItem("Batched Sprite"))
					{
						static float s_cw = 1.0f;
						if (ImGui::InputFloat("Custom Width", &s_cw, 1.0f, 10.0f, "%.1f", ImGuiInputTextFlags_CharsNoBlank | ImGuiInputTextFlags_EnterReturnsTrue))
						{
							batchedsprite->set_custom_width(s_cw);
						}

						static float s_ch = 1.0f;
						if (ImGui::InputFloat("Custom Height", &s_ch, 1.0f, 10.0f, "%.1f", ImGuiInputTextFlags_CharsNoBlank | ImGuiInputTextFlags_EnterReturnsTrue))
						{
							batchedsprite->set_custom_height(s_ch);
						}

						float opacity = batchedsprite->opacity();
						if (ImGui::SliderFloat("Opacity", &opacity, 0.0f, 1.0f, "%.1f", ImGuiSliderFlags_AlwaysClamp | ImGuiSliderFlags_ClampOnInput))
						{
							batchedsprite->set_opacity(opacity);
						}

						static std::string s_bs_tex = "";
						if (ImGui::InputText("Set Region", &s_bs_tex, ImGuiInputTextFlags_CharsNoBlank | ImGuiInputTextFlags_EnterReturnsTrue))
						{
							if (!s_bs_tex.empty())
							{
								batchedsprite->set_region(s_bs_tex);
								s_bs_tex.clear();
							}
						}

						ImGui::EndTabItem();
					}
				}

				if (circle)
				{
					if (ImGui::BeginTabItem("Circle"))
					{
						float opacity = circle->opacity();
						if (ImGui::SliderFloat("Opacity", &opacity, 0.0f, 1.0f, "%.1f", ImGuiSliderFlags_AlwaysClamp | ImGuiSliderFlags_ClampOnInput))
						{
							circle->set_opacity(opacity);
						}

						float radius = circle->radius();
						if (ImGui::InputFloat("Radius", &radius, 0.1f, 1.0f, "%.1f", ImGuiInputTextFlags_CharsNoBlank))
						{
							circle->set_radius(radius);
						}

						float fragments = circle->fragments();
						if (ImGui::InputFloat("Fragments", &fragments, 0.1f, 1.0f, "%.1f", ImGuiInputTextFlags_CharsNoBlank))
						{
							circle->set_fragments(fragments);
						}

						static float colour[4] = {circle->get_colour().m_red, circle->get_colour().m_green, circle->get_colour().m_blue, circle->get_colour().m_alpha};
						if (ImGui::ColorEdit4("Colour", colour, ImGuiColorEditFlags_DisplayRGB | ImGuiColorEditFlags_InputRGB | ImGuiColorEditFlags_AlphaBar | ImGuiColorEditFlags_Uint8))
						{
							circle->change_colour({static_cast<std::uint8_t>(colour[0]), static_cast<std::uint8_t>(colour[1]), static_cast<std::uint8_t>(colour[2]), static_cast<std::uint8_t>(colour[3])});
						}

						if (ImGui::Button("Update"))
						{
							gl_operations.emplace_back([circle]() -> void {
								circle->update();
							});
						}

						ImGui::EndTabItem();
					}
				}

				if (line)
				{
					if (ImGui::BeginTabItem("Line"))
					{
						float opacity = line->opacity();
						if (ImGui::SliderFloat("Opacity", &opacity, 0.0f, 1.0f, "%.1f", ImGuiSliderFlags_AlwaysClamp | ImGuiSliderFlags_ClampOnInput))
						{
							line->set_opacity(opacity);
						}

						static float colour[4] = {line->get_colour().m_red, line->get_colour().m_green, line->get_colour().m_blue, line->get_colour().m_alpha};
						if (ImGui::ColorEdit4("Colour", colour, ImGuiColorEditFlags_DisplayRGB | ImGuiColorEditFlags_InputRGB | ImGuiColorEditFlags_AlphaBar | ImGuiColorEditFlags_Uint8))
						{
							line->change_colour({static_cast<std::uint8_t>(colour[0]), static_cast<std::uint8_t>(colour[1]), static_cast<std::uint8_t>(colour[2]), static_cast<std::uint8_t>(colour[3])});
						}

						if (ImGui::Button("Update"))
						{
							gl_operations.emplace_back([line]() -> void {
								line->update();
							});
						}

						ImGui::EndTabItem();
					}
				}

				if (physics)
				{
					if (ImGui::BeginTabItem("Physics"))
					{
						if (ImGui::Button("Create from JSON"))
						{
							const auto file = SL_HANDLE.vfs()->open_with_dialog("*.json");
							if (file == std::nullopt)
							{
								GALAXY_LOG(GALAXY_ERROR, "Failed to find a file to open for physics component.");
							}
							else
							{
								physics->create_from_json(file.value());
							}
						}

						if (physics->body())
						{
							static bool enable_body = physics->body()->IsEnabled();
							if (ImGui::Checkbox("Enable", &enable_body))
							{
								physics->body()->SetEnabled(enable_body);
							}

							static bool fixed_rotation = physics->body()->IsFixedRotation();
							if (ImGui::Checkbox("Fixed Rotation", &fixed_rotation))
							{
								physics->body()->SetFixedRotation(fixed_rotation);
							}

							static bool is_bullet = physics->body()->IsBullet();
							if (ImGui::Checkbox("Is Bullet", &is_bullet))
							{
								physics->body()->SetBullet(is_bullet);
							}

							static const constexpr auto s_types = magic_enum::enum_names<b2BodyType>();

							std::string s_selected = static_cast<std::string>(magic_enum::enum_name(physics->body()->GetType()));
							if (ImGui::BeginCombo("Type", s_selected.c_str()))
							{
								for (const auto& name : s_types)
								{
									const bool selected = (s_selected == name);
									if (ImGui::Selectable(static_cast<std::string>(name).c_str(), selected))
									{
										s_selected = name;
										physics->body()->SetType(magic_enum::enum_cast<b2BodyType>(s_selected).value());
									}

									if (selected)
									{
										ImGui::SetItemDefaultFocus();
									}
								}

								ImGui::EndCombo();
							}
						}

						ImGui::EndTabItem();
					}
				}

				if (point)
				{
					if (ImGui::BeginTabItem("Point"))
					{
						int size = point->get_size();
						if (ImGui::InputInt("Size", &size, 1, 2, ImGuiInputTextFlags_CharsNoBlank))
						{
							point->set_size(size);
						}

						float opacity = point->opacity();
						if (ImGui::SliderFloat("Opacity", &opacity, 0.0f, 1.0f, "%.1f", ImGuiSliderFlags_AlwaysClamp | ImGuiSliderFlags_ClampOnInput))
						{
							point->set_opacity(opacity);
						}

						static float colour[4] = {point->get_colour().m_red, point->get_colour().m_green, point->get_colour().m_blue, point->get_colour().m_alpha};
						if (ImGui::ColorEdit4("Colour", colour, ImGuiColorEditFlags_DisplayRGB | ImGuiColorEditFlags_InputRGB | ImGuiColorEditFlags_AlphaBar | ImGuiColorEditFlags_Uint8))
						{
							point->change_colour({static_cast<std::uint8_t>(colour[0]), static_cast<std::uint8_t>(colour[1]), static_cast<std::uint8_t>(colour[2]), static_cast<std::uint8_t>(colour[3])});
						}

						if (ImGui::Button("Update"))
						{
							gl_operations.emplace_back([point]() -> void {
								point->update();
							});
						}

						ImGui::EndTabItem();
					}
				}

				if (polygon)
				{
					if (ImGui::BeginTabItem("Polygon"))
					{
						float opacity = polygon->opacity();
						if (ImGui::SliderFloat("Opacity", &opacity, 0.0f, 1.0f, "%.1f", ImGuiSliderFlags_AlwaysClamp | ImGuiSliderFlags_ClampOnInput))
						{
							polygon->set_opacity(opacity);
						}

						ImGui::Text("Point Coords");

						float s_point[2] = {0.0f, 0.0f};
						ImGui::SetNextItemWidth(150);
						ImGui::InputFloat("X##01", &s_point[0], 1.0f, 10.0f, "%.1f", ImGuiInputTextFlags_CharsNoBlank);

						ImGui::SameLine();

						ImGui::SetNextItemWidth(150);
						ImGui::InputFloat("Y##02", &s_point[1], 1.0f, 10.0f, "%.1f", ImGuiInputTextFlags_CharsNoBlank);

						if (ImGui::Button("Add Point"))
						{
							polygon->add_point(s_point[0], s_point[1]);
							s_point[0] = 0.0f;
							s_point[1] = 0.0f;
						}

						static float colour[4] = {polygon->get_colour().m_red, polygon->get_colour().m_green, polygon->get_colour().m_blue, polygon->get_colour().m_alpha};
						if (ImGui::ColorEdit4("Colour", colour, ImGuiColorEditFlags_DisplayRGB | ImGuiColorEditFlags_InputRGB | ImGuiColorEditFlags_AlphaBar | ImGuiColorEditFlags_Uint8))
						{
							polygon->change_colour({static_cast<std::uint8_t>(colour[0]), static_cast<std::uint8_t>(colour[1]), static_cast<std::uint8_t>(colour[2]), static_cast<std::uint8_t>(colour[3])});
						}

						if (ImGui::Button("Update"))
						{
							gl_operations.emplace_back([polygon]() -> void {
								polygon->create();
							});
						}

						ImGui::EndTabItem();
					}
				}

				if (renderable)
				{
					if (ImGui::BeginTabItem("Renderable"))
					{
						static const constexpr auto s_types = magic_enum::enum_names<graphics::Renderables>();

						std::string s_selected = static_cast<std::string>(magic_enum::enum_name(renderable->m_type));
						if (ImGui::BeginCombo("Type", s_selected.c_str()))
						{
							for (const auto& name : s_types)
							{
								const bool selected = (s_selected == name);
								if (ImGui::Selectable(static_cast<std::string>(name).c_str(), selected))
								{
									s_selected         = name;
									renderable->m_type = magic_enum::enum_cast<graphics::Renderables>(s_selected).value();
								}

								if (selected)
								{
									ImGui::SetItemDefaultFocus();
								}
							}

							ImGui::EndCombo();
						}

						ImGui::InputInt("Z Level", &renderable->m_z_level, 1, 2, ImGuiInputTextFlags_CharsNoBlank);

						ImGui::EndTabItem();
					}
				}

				if (shaderid)
				{
					if (ImGui::BeginTabItem("ShaderID"))
					{
						static std::string s_sid_buff = shaderid->m_shader_id;
						if (ImGui::InputText("Shader ID", &s_sid_buff, ImGuiInputTextFlags_CharsNoBlank | ImGuiInputTextFlags_EnterReturnsTrue))
						{
							shaderid->m_shader_id = s_sid_buff;
							s_sid_buff            = "";
						}

						ImGui::EndTabItem();
					}
				}

				if (sprite)
				{
					if (ImGui::BeginTabItem("Sprite"))
					{
						if (ImGui::Button("Load"))
						{
							const auto file = SL_HANDLE.vfs()->open_with_dialog("*.png");
							gl_operations.push_back([sprite, &file]() -> void {
								if (file == std::nullopt)
								{
									GALAXY_LOG(GALAXY_ERROR, "Failed to select file to open for sprite component.");
								}
								else
								{
									sprite->load(file.value());
									sprite->create();
								}
							});
						}

						static float opacity = sprite->opacity();
						if (ImGui::SliderFloat("Opacity", &opacity, 0.0f, 1.0f))
						{
							sprite->set_opacity(opacity);
						}

						static int ansio = sprite->get_aniso_level();
						if (ImGui::SliderInt("Set Ansiotrophy", &ansio, 0, 16))
						{
							gl_operations.push_back([sprite]() -> void {
								sprite->set_anisotropy(ansio);
							});
						}

						ImGui::Text("Clamping");

						if (ImGui::Button("Border"))
						{
							gl_operations.push_back([sprite]() -> void {
							});
							sprite->clamp_to_border();
						}

						ImGui::SameLine();

						if (ImGui::Button("Edge"))
						{
							gl_operations.push_back([sprite]() -> void {
								sprite->clamp_to_edge();
							});
						}

						ImGui::Text("Stretch Mode");

						if (ImGui::Button("Repeat"))
						{
							gl_operations.push_back([sprite]() -> void {
								sprite->set_repeated();
							});
						}

						ImGui::SameLine();

						if (ImGui::Button("Mirror"))
						{
							gl_operations.push_back([sprite]() -> void {
								sprite->set_mirrored();
							});
						}

						ImGui::Text("Filtering");

						if (ImGui::Button("Nearest"))
						{
							gl_operations.push_back([sprite]() -> void {
								sprite->set_minify_filter<graphics::NearestTexFilter>();
								sprite->set_magnify_filter<graphics::NearestTexFilter>();
							});
						}

						ImGui::SameLine();

						if (ImGui::Button("Linear"))
						{
							gl_operations.push_back([sprite]() -> void {
								sprite->set_minify_filter<graphics::LinearTexFilter>();
								sprite->set_magnify_filter<graphics::LinearTexFilter>();
							});
						}

						ImGui::EndTabItem();
					}
				}

				if (tag)
				{
					if (ImGui::BeginTabItem("Tag"))
					{
						ImGui::InputText("Tag", &tag->m_tag);
						ImGui::EndTabItem();
					}
				}

				if (text)
				{
					if (ImGui::BeginTabItem("Text"))
					{
						static std::string s_text_buff = text->get_text();
						ImGui::InputText("Text", &s_text_buff);

						static std::string s_font_buff = text->get_font_id();
						if (ImGui::InputText("Font ID", &s_font_buff, ImGuiInputTextFlags_EnterReturnsTrue))
						{
							text->set_font(s_font_buff);
						}

						static float colour[4] = {text->get_colour().m_red, text->get_colour().m_green, text->get_colour().m_blue, text->get_colour().m_alpha};
						if (ImGui::ColorEdit4("Colour", colour, ImGuiColorEditFlags_DisplayRGB | ImGuiColorEditFlags_InputRGB | ImGuiColorEditFlags_AlphaBar | ImGuiColorEditFlags_Uint8))
						{
							text->change_colour({static_cast<std::uint8_t>(colour[0]), static_cast<std::uint8_t>(colour[1]), static_cast<std::uint8_t>(colour[2]), static_cast<std::uint8_t>(colour[3])});
						}

						if (ImGui::Button("Update"))
						{
							gl_operations.emplace_back([text]() -> void {
								text->create(s_text_buff);
							});
						}

						ImGui::EndTabItem();
					}
				}

				if (transform)
				{
					if (ImGui::BeginTabItem("Transform"))
					{
						float pos[2] = {transform->get_pos().x, transform->get_pos().y};
						if (ImGui::InputFloat2("Position", pos, "%.1f", ImGuiInputTextFlags_CharsNoBlank | ImGuiInputTextFlags_EnterReturnsTrue))
						{
							transform->set_pos(pos[0], pos[1]);
						}

						float rotation = transform->get_rotation();
						if (ImGui::SliderAngle("Rotate", &rotation, 0.0f, 360.0f))
						{
							transform->rotate(rotation);
						}

						static float origin[2] = {0.0f, 0.0f};
						if (ImGui::InputFloat2("Rotation Origin", origin, "%.1f", ImGuiInputTextFlags_CharsNoBlank | ImGuiInputTextFlags_EnterReturnsTrue))
						{
							transform->set_rotation_origin(origin[0], origin[1]);
						}

						ImGui::EndTabItem();
					}
				}

				ImGui::EndTabBar();
			}
		}
	} // namespace panel
} // namespace sc