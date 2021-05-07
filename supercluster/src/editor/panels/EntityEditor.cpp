///
/// EntityEditor.cpp
/// supercluster
///
/// Refer to LICENSE.txt for more details.
///

#include <chrono>
#include <optional>

#include <galaxy/components/Animated.hpp>
#include <galaxy/components/BatchSprite.hpp>
#include <galaxy/components/DirectionLight.hpp>
#include <galaxy/components/Model.hpp>
#include <galaxy/components/OnCollision.hpp>
#include <galaxy/components/PointLight.hpp>
#include <galaxy/components/Primitive2D.hpp>
#include <galaxy/components/Renderable.hpp>
#include <galaxy/components/RigidBody.hpp>
#include <galaxy/components/ShaderID.hpp>
#include <galaxy/components/SpotLight.hpp>
#include <galaxy/components/Sprite.hpp>
#include <galaxy/components/Tag.hpp>
#include <galaxy/components/Text.hpp>
#include <galaxy/components/Transform2D.hpp>
#include <galaxy/components/Transform3D.hpp>

#include <galaxy/core/ServiceLocator.hpp>
#include <galaxy/flags/AllowSerialize.hpp>
#include <galaxy/fs/FileSystem.hpp>
#include <galaxy/ui/ImGuiHelpers.hpp>

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
			m_cur_instance = nullptr;
			m_selected     = std::nullopt;
		}

		void EntityEditor::render(OpenGLOperationStack& gl_operations)
		{
			if (ImGui::Begin("Entity Manager", NULL, ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_AlwaysAutoResize))
			{
				if (ImGui::BeginMenuBar())
				{
					if (ImGui::MenuItem("Create"))
					{
						m_cur_instance->get_stack().top()->m_world.create();
					}

					if (ImGui::MenuItem("Create from JSON"))
					{
						const auto file = SL_HANDLE.vfs()->show_open_dialog("*.json");
						gl_operations.push_back([this, file]() -> void {
							if (file == std::nullopt)
							{
								GALAXY_LOG(GALAXY_ERROR, "Failed to open json for creating an entity.");
							}
							else
							{
								m_cur_instance->get_stack().top()->m_world.create_from_json(file.value());
							}
						});
					}

					ImGui::EndMenuBar();
				}

				static std::string s_entity_label;
				auto& world = m_cur_instance->get_stack().top()->m_world;
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
							ImGui::Text("Directional Light");
							ImGui::TableNextColumn();

							if (ImGui::Button(" + ##3"))
							{
								world.disable(entity);
								world.create_component<components::DirectionLight>(entity);
							}

							ImGui::TableNextColumn();

							if (ImGui::Button(" - ##4"))
							{
								world.remove<components::DirectionLight>(entity);
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
							ImGui::Text("Spot Light");
							ImGui::TableNextColumn();

							if (ImGui::Button(" + ##11"))
							{
								world.disable(entity);
								world.create_component<components::SpotLight>(entity);
							}

							ImGui::TableNextColumn();

							if (ImGui::Button(" - ##12"))
							{
								world.remove<components::SpotLight>(entity);
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
							ImGui::Text("Model");
							ImGui::TableNextColumn();

							if (ImGui::Button(" + ##19"))
							{
								world.disable(entity);
								world.create_component<components::Model>(entity);
							}

							ImGui::TableNextColumn();

							if (ImGui::Button(" - ##20"))
							{
								world.remove<components::Model>(entity);
							}

							ImGui::TableNextRow();
							ImGui::TableNextColumn();
							ImGui::Text("Point Light");
							ImGui::TableNextColumn();

							if (ImGui::Button(" + ##21"))
							{
								world.disable(entity);
								world.create_component<components::PointLight>(entity);
							}

							ImGui::TableNextColumn();

							if (ImGui::Button(" - ##22"))
							{
								world.remove<components::PointLight>(entity);
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
							ImGui::Text("Shader");
							ImGui::TableNextColumn();

							if (ImGui::Button(" + ##25"))
							{
								world.disable(entity);
								world.create_component<components::ShaderID>(entity);
							}

							ImGui::TableNextColumn();

							if (ImGui::Button(" - ##26"))
							{
								world.remove<components::ShaderID>(entity);
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
								gl_operations.emplace_back([&world, entity]() -> void {
									world.create_component<components::Text>(entity);
								});
							}

							ImGui::TableNextColumn();

							if (ImGui::Button(" - ##30"))
							{
								gl_operations.emplace_back([&world, entity]() -> void {
									world.remove<components::Text>(entity);
								});
							}

							ImGui::TableNextRow();
							ImGui::TableNextColumn();
							ImGui::Text("Transform3D");
							ImGui::TableNextColumn();

							if (ImGui::Button(" + ##31"))
							{
								world.disable(entity);
								world.create_component<components::Transform3D>(entity);
							}

							ImGui::TableNextColumn();

							if (ImGui::Button(" - ##32"))
							{
								world.remove<components::Transform3D>(entity);
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

		void EntityEditor::set_instance(core::Instance* instance)
		{
			m_cur_instance = instance;
		}

		void EntityEditor::set_selected_entity(const std::optional<ecs::Entity>& entity)
		{
			m_selected = entity;
		}

		void EntityEditor::render_components(const ecs::Entity entity, OpenGLOperationStack& gl_operations)
		{
			// clang-format off
			auto [animated,
				batchsprite,
				directionlight,
				model,
				oncollision,
				pointlight,
				primitive2d,
				renderable,
				rigidbody,
				shaderid,
				spotlight,
				sprite,
				tag,
				text,
				transform2d,
				transform3d
			] = m_cur_instance->get_stack().top()->m_world.get_multi<
				components::Animated,
				components::BatchSprite,
				components::DirectionLight,
				components::Model,
				components::OnCollision,
				components::PointLight,
				components::Primitive2D,
				components::Renderable,
				components::RigidBody,
				components::ShaderID,
				components::SpotLight,
				components::Sprite,
				components::Tag,
				components::Text,
				components::Transform2D,
				components::Transform3D
			>(entity);
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
						static float s_cw = static_cast<float>(batchsprite->get_width());
						if (ImGui::InputFloat("Custom Width", &s_cw, 1.0f, 10.0f, "%.1f", ImGuiInputTextFlags_CharsNoBlank | ImGuiInputTextFlags_EnterReturnsTrue))
						{
							batchsprite->set_custom_width(s_cw);
						}

						static float s_ch = static_cast<float>(batchsprite->get_height());
						if (ImGui::InputFloat("Custom Height", &s_ch, 1.0f, 10.0f, "%.1f", ImGuiInputTextFlags_CharsNoBlank | ImGuiInputTextFlags_EnterReturnsTrue))
						{
							batchsprite->set_custom_height(s_ch);
						}

						static float opacity = batchsprite->get_opacity();
						if (ImGui::SliderFloat("Opacity", &opacity, 0.0f, 1.0f, "%.1f", ImGuiSliderFlags_AlwaysClamp | ImGuiSliderFlags_ClampOnInput))
						{
							batchsprite->set_opacity(opacity);
						}

						static std::string s_bs_tex = batchsprite->get_tex_id();
						if (ImGui::InputText("Set Region", &s_bs_tex, ImGuiInputTextFlags_CharsNoBlank | ImGuiInputTextFlags_EnterReturnsTrue))
						{
							if (!s_bs_tex.empty())
							{
								batchsprite->set_region(s_bs_tex);
								s_bs_tex = batchsprite->get_tex_id();
							}
						}

						ImGui::EndTabItem();
					}
				}

				if (directionlight)
				{
					if (ImGui::BeginTabItem("Directional Light"))
					{
						ui::im_draw_class(directionlight->m_light.m_ambient_intensity, "Ambient");
						ui::im_draw_class(directionlight->m_light.m_diffuse_intensity, "Diffuse");
						ui::im_draw_class(directionlight->m_light.m_specular_intensity, "Specular");
						ui::im_draw_class(directionlight->m_light.m_dir, "Direction");

						ImGui::EndTabItem();
					}
				}

				if (model)
				{
					if (ImGui::BeginTabItem("Model"))
					{
						if (ImGui::Button("Load"))
						{
							const auto file = SL_HANDLE.vfs()->open_with_dialog();
							gl_operations.push_back([model, &file]() {
								if (file == std::nullopt)
								{
									GALAXY_LOG(GALAXY_ERROR, "Failed to select file to open for model component.");
								}
								else
								{
									model->load(file.value());
									model->create();
								}
							});
						}

						ImGui::EndTabItem();
					}
				}

				if (oncollision)
				{
					if (ImGui::BeginTabItem("On Collision"))
					{
						ImGui::InputText("Script", &oncollision->m_script);
						ImGui::EndTabItem();
					}
				}

				if (pointlight)
				{
					if (ImGui::BeginTabItem("Point Light"))
					{
						ui::im_draw_class(pointlight->m_light.m_ambient_intensity, "Ambient");
						ui::im_draw_class(pointlight->m_light.m_diffuse_intensity, "Diffuse");
						ui::im_draw_class(pointlight->m_light.m_specular_intensity, "Specular");
						ui::im_draw_class(pointlight->m_light.m_pos, "Position");

						ImGui::EndTabItem();
					}
				}

				if (primitive2d)
				{
					if (ImGui::BeginTabItem("2D Primitive"))
					{
						static constexpr const auto s_types = magic_enum::enum_names<graphics::Primitives>();

						static std::string s_selected = static_cast<std::string>(magic_enum::enum_name(primitive2d->get_type()));
						static auto s_type            = primitive2d->get_type();
						if (ImGui::BeginCombo("Type", s_selected.c_str()))
						{
							for (const auto& name : s_types)
							{
								const bool selected = (s_selected == name);
								if (ImGui::Selectable(static_cast<std::string>(name).c_str(), selected))
								{
									s_selected = name;
									s_type     = magic_enum::enum_cast<graphics::Primitives>(s_selected).value();
								}

								if (selected)
								{
									ImGui::SetItemDefaultFocus();
								}
							}

							ImGui::EndCombo();
						}

						auto& data             = primitive2d->get_data();
						static float colour[4] = {data.m_colour.m_red, data.m_colour.m_green, data.m_colour.m_blue, data.m_colour.m_alpha};
						if (ImGui::ColorEdit4("Colour", colour, ImGuiColorEditFlags_DisplayRGB | ImGuiColorEditFlags_InputRGB | ImGuiColorEditFlags_AlphaBar | ImGuiColorEditFlags_Uint8))
						{
							data.m_colour = {static_cast<std::uint8_t>(colour[0]), static_cast<std::uint8_t>(colour[1]), static_cast<std::uint8_t>(colour[2]), static_cast<std::uint8_t>(colour[3])};
						}

						if (data.m_fragments != std::nullopt)
						{
							static float fragments = data.m_fragments.value();
							if (ImGui::InputFloat("Fragments", &fragments, 0.1f, 1.0f, "%.1f", ImGuiInputTextFlags_CharsNoBlank))
							{
								data.m_fragments = fragments;
							}
						}

						if (data.m_points != std::nullopt)
						{
							static float s_point[2] = {0.0f, 0.0f};
							ImGui::SetNextItemWidth(150);
							ImGui::InputFloat("X##01", &s_point[0], 1.0f, 10.0f, "%.1f", ImGuiInputTextFlags_CharsNoBlank);

							ImGui::SameLine();

							ImGui::SetNextItemWidth(150);
							ImGui::InputFloat("Y##02", &s_point[1], 1.0f, 10.0f, "%.1f", ImGuiInputTextFlags_CharsNoBlank);

							static std::vector<glm::vec2> points = {};
							if (ImGui::Button("Add Point"))
							{
								points.emplace_back(s_point[0], s_point[1]);

								s_point[0] = 0.0f;
								s_point[1] = 0.0f;
								points.clear();
							}
						}

						if (data.m_pointsize != std::nullopt)
						{
							static int size = data.m_pointsize.value();
							if (ImGui::InputInt("Size", &size, 1, 2, ImGuiInputTextFlags_CharsNoBlank))
							{
								data.m_pointsize = size;
							}
						}

						if (data.m_radii != std::nullopt)
						{
							ImGui::SetNextItemWidth(150);
							ImGui::InputFloat("Hor Rad##Ellipse01", &data.m_radii.value().x, 1.0f, 10.0f, "%.1f", ImGuiInputTextFlags_CharsNoBlank);

							ImGui::SameLine();

							ImGui::SetNextItemWidth(150);
							ImGui::InputFloat("Vert Rad##Ellipse02", &data.m_radii.value().y, 1.0f, 10.0f, "%.1f", ImGuiInputTextFlags_CharsNoBlank);
						}

						if (data.m_radius != std::nullopt)
						{
							static float radius = data.m_radius.value();
							if (ImGui::InputFloat("Radius", &radius, 0.1f, 1.0f, "%.1f", ImGuiInputTextFlags_CharsNoBlank))
							{
								data.m_radius = radius;
							}
						}

						if (data.m_start_end != std::nullopt)
						{
							ImGui::Text("First Point");
							static float s_point_xy[2] = {0.0f, 0.0f};
							ImGui::SetNextItemWidth(150);
							ImGui::InputFloat("X1##03", &s_point_xy[0], 1.0f, 10.0f, "%.1f", ImGuiInputTextFlags_CharsNoBlank);

							ImGui::SameLine();

							ImGui::SetNextItemWidth(150);
							ImGui::InputFloat("Y1##04", &s_point_xy[1], 1.0f, 10.0f, "%.1f", ImGuiInputTextFlags_CharsNoBlank);

							ImGui::Text("Second Point");
							static float s_point_zw[2] = {0.0f, 0.0f};
							ImGui::SetNextItemWidth(150);
							ImGui::InputFloat("X2##05", &s_point_zw[0], 1.0f, 10.0f, "%.1f", ImGuiInputTextFlags_CharsNoBlank);

							ImGui::SameLine();

							ImGui::SetNextItemWidth(150);
							ImGui::InputFloat("Y2##06", &s_point_zw[1], 1.0f, 10.0f, "%.1f", ImGuiInputTextFlags_CharsNoBlank);
						}

						if (ImGui::Button("Create"))
						{
							switch (s_type)
							{
								case graphics::Primitives::CIRCLE:
								{
									data.m_fragments = 40;
									data.m_radius    = 10.0f;
								}
								break;

								case graphics::Primitives::ELLIPSE:
								{
									data.m_fragments = 40;
									data.m_radii     = {15.0f, 10.0f};
								}
								break;

								case graphics::Primitives::LINE:
								{
									data.m_start_end = {0.0f, 0.0f, 50.0f, 50.0f};
								}
								break;

								case graphics::Primitives::POINT:
								{
									data.m_pointsize = 3;
								}
								break;

								case graphics::Primitives::POLYLINE:
								{
									data.m_points = std::make_optional<std::vector<glm::vec2>>({});
								}
								break;

								case graphics::Primitives::POLYGON:
								{
									data.m_points = std::make_optional<std::vector<glm::vec2>>({});
								}
								break;
							}
						}

						ImGui::SameLine();

						if (ImGui::Button("Update"))
						{
							gl_operations.push_back([&data, primitive2d]() {
								switch (s_type)
								{
									case graphics::Primitives::CIRCLE:
									{
										primitive2d->create<graphics::Primitives::CIRCLE>(data);
									}
									break;

									case graphics::Primitives::ELLIPSE:
									{
										primitive2d->create<graphics::Primitives::ELLIPSE>(data);
									}
									break;

									case graphics::Primitives::LINE:
									{
										primitive2d->create<graphics::Primitives::LINE>(data);
									}
									break;

									case graphics::Primitives::POINT:
									{
										primitive2d->create<graphics::Primitives::POINT>(data);
									}
									break;

									case graphics::Primitives::POLYLINE:
									{
										primitive2d->create<graphics::Primitives::POLYLINE>(data);
									}
									break;

									case graphics::Primitives::POLYGON:
									{
										primitive2d->create<graphics::Primitives::POLYGON>(data);
									}
									break;
								}
							});
						}

						ImGui::SameLine();

						if (ImGui::Button("Reset"))
						{
							data = {};
						}

						ImGui::EndTabItem();
					}
				}

				if (renderable)
				{
					if (ImGui::BeginTabItem("Renderable"))
					{
						static constexpr const auto s_types = magic_enum::enum_names<graphics::Renderables>();

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

				if (rigidbody)
				{
					if (ImGui::BeginTabItem("Rigid Body"))
					{
						static constexpr const auto s_types = magic_enum::enum_names<physics::BodyType>();

						std::string s_selected = static_cast<std::string>(magic_enum::enum_name(rigidbody->get_type()));
						if (ImGui::BeginCombo("Type", s_selected.c_str()))
						{
							for (const auto& name : s_types)
							{
								const bool selected = (s_selected == name);
								if (ImGui::Selectable(static_cast<std::string>(name).c_str(), selected))
								{
									s_selected = name;
									rigidbody->set_bodytype(magic_enum::enum_cast<physics::BodyType>(s_selected).value());
								}

								if (selected)
								{
									ImGui::SetItemDefaultFocus();
								}
							}

							ImGui::EndCombo();
						}

						ImGui::Text("AABB Stats:");

						const auto& aabb = rigidbody->get_aabb();
						ImGui::Text("Area: %d.", aabb.area());
						ImGui::Text("Min: %f, %f.", aabb.min().x, aabb.min().y);
						ImGui::Text("Max: %f, %f.", aabb.max().x, aabb.max().y);
						ImGui::Text("Size: %f, %f.", aabb.size().x, aabb.size().y);

						ImGui::EndTabItem();
					}
				}

				if (shaderid)
				{
					if (ImGui::BeginTabItem("Shader"))
					{
						ImGui::InputText("Shader ID", &shaderid->m_shader_id, ImGuiInputTextFlags_CharsNoBlank);
						ImGui::EndTabItem();
					}
				}

				if (spotlight)
				{
					if (ImGui::BeginTabItem("Spot Light"))
					{
						ui::im_draw_class(spotlight->m_light.m_ambient_intensity, "Ambient");
						ui::im_draw_class(spotlight->m_light.m_diffuse_intensity, "Diffuse");
						ui::im_draw_class(spotlight->m_light.m_specular_intensity, "Specular");
						ui::im_draw_class(spotlight->m_light.m_pos, "Position");
						ui::im_draw_class(spotlight->m_light.m_dir, "Direction");

						ImGui::InputFloat("Inner Cutoff", &spotlight->m_light.m_inner_cutoff, 0.1f, 1.0f, "%.1f", ImGuiInputTextFlags_AutoSelectAll | ImGuiInputTextFlags_CharsNoBlank);
						ImGui::InputFloat("Outer Cutoff", &spotlight->m_light.m_outer_cutoff, 0.1f, 1.0f, "%.1f", ImGuiInputTextFlags_AutoSelectAll | ImGuiInputTextFlags_CharsNoBlank);

						ImGui::EndTabItem();
					}
				}

				if (sprite)
				{
					if (ImGui::BeginTabItem("Sprite"))
					{
						if (ImGui::Button("Load"))
						{
							const auto file = SL_HANDLE.vfs()->open_with_dialog();
							gl_operations.push_back([sprite, &file]() {
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

						float opacity = sprite->get_opacity();
						if (ImGui::SliderFloat("Opacity", &opacity, 0.0f, 1.0f))
						{
							sprite->set_opacity(opacity);
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

				if (transform2d)
				{
					if (ImGui::BeginTabItem("Transform2D"))
					{
						static float pos[2] = {transform2d->get_pos().x, transform2d->get_pos().y};
						if (ImGui::InputFloat2("Position", pos, "%.1f", ImGuiInputTextFlags_CharsNoBlank | ImGuiInputTextFlags_EnterReturnsTrue))
						{
							transform2d->set_pos(pos[0], pos[1]);
						}

						static float rotation = transform2d->get_rotation();
						if (ImGui::SliderAngle("Rotate", &rotation, 0.0f, 360.0f))
						{
							transform2d->rotate(rotation);
						}

						ImGui::EndTabItem();
					}
				}

				if (transform3d)
				{
					if (ImGui::BeginTabItem("Transform3D"))
					{
						static glm::vec3 origin = transform3d->get_origin();
						ui::im_draw_class(origin, "Origin");
						transform3d->set_origin(origin.x, origin.y, origin.z);

						static glm::vec3 rot_axis = transform3d->get_rotation_axis();
						ui::im_draw_class(rot_axis, "Rotation Axis");
						transform3d->set_rotation_axis(rot_axis.x, rot_axis.y, rot_axis.z);

						static glm::vec3 pos = transform3d->get_pos();
						ui::im_draw_class(pos, "Position");
						transform3d->set_pos(pos.x, pos.y, pos.z);

						static float rot = transform3d->get_rotation();
						if (ImGui::InputFloat("Rotation", &rot, 1.0f, 10.0f, "%.1f", ImGuiInputTextFlags_CharsNoBlank | ImGuiInputTextFlags_EnterReturnsTrue))
						{
							transform3d->rotate(rot);
						}

						static float scale = transform3d->get_scale();
						if (ImGui::InputFloat("Scale", &scale, 0.1f, 1.0f, "%.1f", ImGuiInputTextFlags_CharsNoBlank | ImGuiInputTextFlags_EnterReturnsTrue))
						{
							transform3d->scale(scale);
						}

						if (ImGui::Button("Reset"))
						{
							transform3d->reset();
						}

						ImGui::EndTabItem();
					}
				}

				ImGui::EndTabBar();
			}
		}
	} // namespace panel
} // namespace sc