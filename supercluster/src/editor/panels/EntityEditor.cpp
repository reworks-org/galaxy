///
/// EntityEditor.cpp
/// supercluster
///
/// Refer to LICENSE.txt for more details.
///

#include <entt/entity/registry.hpp>
#include <magic_enum.hpp>
#include <imgui_stdlib.h>
#include <imgui_addons/imgui_notify.h>

#include <galaxy/core/ServiceLocator.hpp>
#include <galaxy/core/Window.hpp>
#include <galaxy/components/Animated.hpp>
#include <galaxy/components/DrawShader.hpp>
#include <galaxy/components/Flag.hpp>
#include <galaxy/components/Map.hpp>
#include <galaxy/components/Primitive.hpp>
#include <galaxy/components/RigidBody.hpp>
#include <galaxy/components/Script.hpp>
#include <galaxy/components/Sprite.hpp>
#include <galaxy/components/Tag.hpp>
#include <galaxy/components/Text.hpp>
#include <galaxy/components/Transform.hpp>
#include <galaxy/flags/AllowSerialize.hpp>
#include <galaxy/flags/Enabled.hpp>
#include <galaxy/fs/VirtualFileSystem.hpp>
#include <galaxy/resource/Fonts.hpp>
#include <galaxy/resource/Shaders.hpp>
#include <galaxy/resource/TextureAtlas.hpp>
#include <galaxy/ui/ImGuiHelpers.hpp>

#include "EntityEditor.hpp"

using namespace galaxy;

namespace sc
{
	namespace panel
	{
		static constexpr const auto s_types = magic_enum::enum_names<graphics::Shape>();

		void EntityEditor::render(Selected& selected, UpdateStack& updates)
		{
			static constexpr const auto numeric_input_flags =
				ImGuiInputTextFlags_CharsDecimal | ImGuiInputTextFlags_CharsNoBlank | ImGuiInputTextFlags_AutoSelectAll;
			static const std::vector<std::string> b2_body_types = {"b2_dynamicBody", "b2_kinematicBody", "b2_staticBody"};

			if (ImGui::Begin("Entity", nullptr, ImGuiWindowFlags_AlwaysAutoResize))
			{
				if (selected.m_selected != entt::null && selected.m_world != nullptr && selected.m_world->m_registry.valid(selected.m_selected))
				{
					ImGui::PushID(static_cast<std::uint32_t>(selected.m_selected));

					ImGui::Text("EnTT Id: %u", entt::to_integral(selected.m_selected));

					ImGui::SameLine();

					ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.65f, 0.15f, 0.15f, 1.f));
					ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.8f, 0.3f, 0.3f, 1.f));
					ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(1.f, 0.2f, 0.2f, 1.f));
					if (ImGui::Button("Destroy"))
					{
						selected.m_world->m_registry.destroy(selected.m_selected);
						selected.m_selected = entt::null;

						// Update stack for early exit.
						ImGui::PopStyleColor(3);
						ImGui::End();

						return;
					}
					ImGui::PopStyleColor(3);

					auto tag = selected.m_world->m_registry.try_get<components::Tag>(selected.m_selected);
					if (tag)
					{
						ImGui::InputText("##Tag", &tag->m_tag, ImGuiInputTextFlags_AutoSelectAll);
					}

					ImGui::SameLine();
					ImGui::PushItemWidth(-1);

					if (ImGui::Button("Add Component"))
					{
						ImGui::OpenPopup("AddNewComponent");
					}

					if (ImGui::BeginPopup("AddNewComponent"))
					{
						draw_entry<components::Animated>(selected, "Animated");
						draw_entry<components::DrawShader>(selected, "Draw Shader");
						draw_entry<components::Flag>(selected, "Flag");
						draw_entry<components::Primitive>(selected, "Primitive");
						draw_entry<components::RigidBody>(selected, "RigidBody");
						draw_entry<components::Script>(selected, "Script");
						draw_entry<components::Sprite>(selected, "Sprite");
						draw_entry<components::Tag>(selected, "Tag");
						draw_entry<components::Text>(selected, "Text");
						draw_entry<components::Transform>(selected, "Transform");

						ImGui::EndPopup();
					}

					ImGui::PopItemWidth();

					draw_component<components::Animated>(selected, "Animated", [](components::Animated* animated) {
						if (animated->is_paused())
						{
							ImGui::Text("State: Paused.");
						}
						else
						{
							ImGui::Text("State: Playing");
						}

						ImGui::Text("Time spent on frame: %.2f", animated->m_time_spent_on_frame);

						ImGui::Spacing();
						ImGui::Separator();
						ImGui::Spacing();

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

						if (ImGui::Button("Add Animation"))
						{
							ImGui::OpenPopup("AddAnimationPopup");
						}

						if (ImGui::BeginPopup("AddAnimationPopup", ImGuiWindowFlags_AlwaysAutoResize))
						{
							static std::string s_name = "";
							ImGui::InputText("Name", &s_name, ImGuiInputTextFlags_AutoSelectAll);

							static bool s_loop = false;
							ImGui::Checkbox("Is Looping?", &s_loop);

							static float s_speed = 1.0f;
							ImGui::SliderFloat("Speed", &s_speed, 0.1f, 10.0f, "%.1f", ImGuiSliderFlags_AlwaysClamp);

							if (ImGui::Button("Add Frame"))
							{
								ImGui::OpenPopup("AddFramePopup");
							}

							static std::vector<graphics::Frame> s_frames = {};
							if (ImGui::BeginPopup("AddFramePopup", ImGuiWindowFlags_AlwaysAutoResize))
							{
								static graphics::Frame s_frame;

								static std::string s_tr_id = "";
								ImGui::InputText("Texture Region Id", &s_tr_id, ImGuiInputTextFlags_AutoSelectAll);

								static float s_tpf = 0.1f;
								ImGui::SliderFloat("Time Per Frame", &s_tpf, 0.1f, 10.0f, "%.1f", ImGuiSliderFlags_AlwaysClamp);

								if (!s_tr_id.empty())
								{
									if (ImGui::Button("Create##Frame"))
									{
										s_frame.m_texture_id     = s_tr_id;
										s_frame.m_time_per_frame = s_tpf;
										s_frames.push_back(s_frame);

										s_frame = {};
										s_tr_id = "";
										s_tpf   = 0.1f;

										ImGui_Notify::InsertNotification({ImGuiToastType_Success, 2000, "New frame added."});

										ImGui::CloseCurrentPopup();
									}
								}

								ImGui::EndPopup();
							}

							for (const auto& frame : s_frames)
							{
								ImGui::BulletText("{Texture: %s, Time: %.1f}", frame.m_texture_id, frame.m_time_per_frame);
							}

							if (!s_name.empty())
							{
								if (ImGui::Button("Create##Animation"))
								{
									animated->add(s_name, s_loop, static_cast<double>(s_speed), s_frames);

									s_name  = "";
									s_loop  = false;
									s_speed = 1.0f;
									s_frames.clear();

									ImGui_Notify::InsertNotification({ImGuiToastType_Success, 2000, "New animation added."});

									ImGui::CloseCurrentPopup();
								}
							}

							ImGui::EndPopup();
						}

						static std::string s_selected = "None";
						if (animated->active())
						{
							s_selected = animated->active()->m_name;
						}

						if (ImGui::BeginCombo("Animation", s_selected.c_str()))
						{
							for (const auto& [name, anim] : animated->m_animations)
							{
								const bool selected = (s_selected == name);
								if (ImGui::Selectable(name.c_str(), selected))
								{
									s_selected = name;
									animated->set(s_selected);
								}

								if (selected)
								{
									ImGui::SetItemDefaultFocus();
								}
							}

							ImGui::EndCombo();
						}
					});

					draw_component<components::DrawShader>(selected, "Draw Shader", [](components::DrawShader* ds) {
						static std::string s_search = "";
						ImGui::InputTextWithHint("##EntityShaderSearch", "Search...", &s_search, ImGuiInputTextFlags_AutoSelectAll);

						if (ImGui::BeginCombo("Shader", ds->m_id.c_str()))
						{
							for (const auto& key : core::ServiceLocator<resource::Shaders>::ref().keys())
							{
								if (key.find(s_search) != std::string::npos)
								{
									const bool selected = (ds->m_id == key);
									if (ImGui::Selectable(key.c_str(), selected))
									{
										ds->set_shader(key);
									}

									if (selected)
									{
										ImGui::SetItemDefaultFocus();
									}
								}
							}

							ImGui::EndCombo();
						}
					});

					draw_component<components::Flag>(selected, "Flags", [&](components::Flag* flag) {
						auto enabled = flag->is_flag_set<flags::Enabled>();
						if (ImGui::Checkbox("Is Enabled", &enabled))
						{
							if (enabled)
							{
								if (selected.m_world->is_valid(selected.m_selected))
								{
									flag->set_flag<flags::Enabled>();
								}
								else
								{
									ImGui_Notify::InsertNotification({ImGuiToastType_Error, 2000, "Entity did not pass validation."});
								}
							}
							else
							{
								flag->unset_flag<flags::Enabled>();
							}
						}

						ImGui::SameLine();

						auto allow_save = flag->is_flag_set<flags::AllowSerialize>();
						if (ImGui::Checkbox("Allow Serialization", &allow_save))
						{
							if (allow_save)
							{
								flag->set_flag<flags::AllowSerialize>();
							}
							else
							{
								flag->unset_flag<flags::AllowSerialize>();
							}
						}
					});

					draw_component<components::Map>(selected, "Map", [&](components::Map* map) {
						ImGui::Text("Reference component, not directly editable.");
						ImGui::Spacing();
						ImGui::Text("Animated Tile Count: %zu", map->m_animations.size());
						ImGui::Text("Render Layer: %i", map->get_layer());
					});

					draw_component<components::Primitive>(selected, "Primitive", [&](components::Primitive* primitive) {
						ImGui::Text("Width: %.0f", primitive->get_width());
						ImGui::SameLine(0.0f, 5.0f);
						ImGui::Text("Height: %.0f", primitive->get_height());

						ImGui::InputInt("Layer", &primitive->m_layer, 1, 2, numeric_input_flags);

						static auto s_selected = static_cast<std::string>(magic_enum::enum_name(primitive->get_shape()));
						static auto s_type     = primitive->get_shape();
						if (ImGui::BeginCombo("Type", s_selected.c_str()))
						{
							for (const auto& name : s_types)
							{
								const bool selected = (s_selected == name);
								if (ImGui::Selectable(static_cast<std::string>(name).c_str(), selected))
								{
									s_selected = name;
									s_type     = magic_enum::enum_cast<graphics::Shape>(s_selected).value();
								}

								if (selected)
								{
									ImGui::SetItemDefaultFocus();
								}
							}

							ImGui::EndCombo();
						}

						auto colour = primitive->m_colour.normalized();
						if (ImGui::ColorEdit4("Colour", &colour[0]))
						{
							primitive->m_colour.set_from_normalized(colour[0], colour[1], colour[2], colour[3]);
						}

						ImGui::Spacing();

						static components::Primitive::PrimitiveData data = primitive->get_data();
						if (s_type == graphics::Shape::CIRCLE || s_type == graphics::Shape::ELLIPSE)
						{
							ImGui::InputFloat("Fragments", &data.fragments, 0.1f, 1.0f, "%.1f", ImGuiInputTextFlags_CharsNoBlank);
						}

						if (s_type == graphics::Shape::CIRCLE)
						{
							ImGui::InputFloat("Radius", &data.radius, 0.1f, 1.0f, "%.1f", ImGuiInputTextFlags_CharsNoBlank);
						}

						if (s_type == graphics::Shape::ELLIPSE)
						{
							ImGui::Text("Radii");

							ImGui::SetNextItemWidth(150);
							ImGui::InputFloat("X##1", &data.radii.x, 1.0f, 10.0f, "%.1f", ImGuiInputTextFlags_CharsNoBlank);

							ImGui::SameLine();

							ImGui::SetNextItemWidth(150);
							ImGui::InputFloat("Y##2", &data.radii.y, 1.0f, 10.0f, "%.1f", ImGuiInputTextFlags_CharsNoBlank);
						}

						if (s_type == graphics::Shape::LINE)
						{
							ImGui::Text("First Point");
							static float s_point_xy[2] = {0.0f, 0.0f};
							ImGui::SetNextItemWidth(150);
							ImGui::InputFloat("X1##3", &s_point_xy[0], 1.0f, 10.0f, "%.1f", ImGuiInputTextFlags_CharsNoBlank);

							ImGui::SameLine();

							ImGui::SetNextItemWidth(150);
							ImGui::InputFloat("Y1##4", &s_point_xy[1], 1.0f, 10.0f, "%.1f", ImGuiInputTextFlags_CharsNoBlank);

							ImGui::Text("Second Point");
							static float s_point_zw[2] = {0.0f, 0.0f};
							ImGui::SetNextItemWidth(150);
							ImGui::InputFloat("X2##5", &s_point_zw[0], 1.0f, 10.0f, "%.1f", ImGuiInputTextFlags_CharsNoBlank);

							ImGui::SameLine();

							ImGui::SetNextItemWidth(150);
							ImGui::InputFloat("Y2##6", &s_point_zw[1], 1.0f, 10.0f, "%.1f", ImGuiInputTextFlags_CharsNoBlank);
						}

						static std::vector<glm::vec2> points = {};
						if (s_type == graphics::Shape::POLYLINE || s_type == graphics::Shape::POLYGON)
						{
							if (ImGui::Button("Add Point"))
							{
								ImGui::OpenPopup("PolyPointPopup");
							}

							ImGui::SameLine();

							if (ImGui::Button("Clear Points"))
							{
								points.clear();
							}

							ImGui::Text("Points");
							for (const auto& point : points)
							{
								ImGui::BulletText("{X: %.1f, Y: %.1f}", point.x, point.y);
							}

							if (ImGui::BeginPopup("PolyPointPopup"))
							{
								static glm::vec2 s_point {0.0f, 0.0f};
								ImGui::SetNextItemWidth(150);
								ImGui::InputFloat("X##7", &s_point.x, 1.0f, 10.0f, "%.1f", ImGuiInputTextFlags_CharsNoBlank);

								ImGui::SameLine();

								ImGui::SetNextItemWidth(150);
								ImGui::InputFloat("Y##8", &s_point.y, 1.0f, 10.0f, "%.1f", ImGuiInputTextFlags_CharsNoBlank);

								if (ImGui::Button("Push"))
								{
									points.emplace_back(s_point);

									s_point.x = 0.0f;
									s_point.y = 0.0f;

									ImGui::CloseCurrentPopup();
								}
							}
						}

						if (ImGui::Button("Create"))
						{
							updates.emplace_back([&]() {
								data.points = std::move(points);

								switch (s_type)
								{
									case graphics::Shape::CIRCLE:
										primitive->create<graphics::Shape::CIRCLE>(data, primitive->m_colour, primitive->m_layer);
										break;

									case graphics::Shape::ELLIPSE:
										primitive->create<graphics::Shape::ELLIPSE>(data, primitive->m_colour, primitive->m_layer);
										break;

									case graphics::Shape::LINE:
										primitive->create<graphics::Shape::LINE>(data, primitive->m_colour, primitive->m_layer);
										break;

									case graphics::Shape::POINT:
										primitive->create<graphics::Shape::POINT>(data, primitive->m_colour, primitive->m_layer);
										break;

									case graphics::Shape::POLYGON:
										primitive->create<graphics::Shape::POLYGON>(data, primitive->m_colour, primitive->m_layer);
										break;

									case graphics::Shape::POLYLINE:
										primitive->create<graphics::Shape::POLYLINE>(data, primitive->m_colour, primitive->m_layer);
										break;
								}

								data = {};
								points.clear();
							});

							ImGui_Notify::InsertNotification({ImGuiToastType_Success, 2000, "Successfully created primitive."});
						}
					});

					draw_component<components::RigidBody>(selected, "RigidBody", [&](components::RigidBody* body) {
						float shape[2] = {body->get_shape().x, body->get_shape().y};
						if (ImGui::InputFloat2("Shape", shape, "%.1f", ImGuiInputTextFlags_CharsNoBlank | ImGuiInputTextFlags_AutoSelectAll))
						{
							body->set_shape(shape[0], shape[1]);
						}

						const auto type_selected = std::string(magic_enum::enum_name(body->get_type()));
						if (ImGui::BeginCombo("Body Type", type_selected.c_str()))
						{
							for (const auto& type : b2_body_types)
							{
								const bool selected = (type_selected == type);
								if (ImGui::Selectable(type.c_str(), selected))
								{
									body->set_type(magic_enum::enum_cast<b2BodyType>(type).value());
								}

								if (selected)
								{
									ImGui::SetItemDefaultFocus();
								}
							}

							ImGui::EndCombo();
						}

						ImGui::Spacing();

						auto density = body->get_density();
						if (ImGui::InputFloat("Density", &density, 0.1f, 1.0f, "%.1f", ImGuiInputTextFlags_CharsNoBlank | ImGuiInputTextFlags_AutoSelectAll))
						{
							body->set_density(density);
						}

						ImGui::SameLine();

						auto friction = body->get_friction();
						if (ImGui::InputFloat("Friction", &friction, 0.1f, 1.0f, "%.1f", ImGuiInputTextFlags_CharsNoBlank | ImGuiInputTextFlags_AutoSelectAll))
						{
							body->set_friction(friction);
						}

						auto restitution = body->get_restitution();
						if (ImGui::InputFloat("Restitution",
								&restitution,
								0.1f,
								1.0f,
								"%.1f",
								ImGuiInputTextFlags_CharsNoBlank | ImGuiInputTextFlags_AutoSelectAll))
						{
							body->set_restitution(restitution);
						}

						auto rt = body->get_restitution_threshold();
						if (ImGui::InputFloat("Restitution Threshold",
								&rt,
								0.1f,
								1.0f,
								"%.1f",
								ImGuiInputTextFlags_CharsNoBlank | ImGuiInputTextFlags_AutoSelectAll))
						{
							body->set_restitution_threshold(rt);
						}

						ImGui::Spacing();

						auto bullet = body->is_bullet();
						if (ImGui::Checkbox("Is Bullet", &bullet))
						{
							body->set_bullet(bullet);
						}

						ImGui::SameLine();

						auto rf = body->is_rotation_fixed();
						if (ImGui::Checkbox("Rotation Fixed", &rf))
						{
							body->set_fixed_rotation(rf);
						}
					});

					draw_component<components::Script>(selected, "Script", [&](components::Script* script) {
						if (ImGui::Button("Load"))
						{
							auto file = core::ServiceLocator<fs::VirtualFileSystem>::ref().show_open_dialog("*.lua");
							if (file.has_value())
							{
								auto& script = selected.m_world->m_registry.emplace_or_replace<components::Script>(selected.m_selected);
								script.load(file.value());
							}
						}

						ImGui::SameLine();

						if (ImGui::Button("Copy Path to Clipboard"))
						{
							core::ServiceLocator<core::Window>::ref().get_input<input::Clipboard>().set(script->file().c_str());
						}
					});

					draw_component<components::Sprite>(selected, "Sprite", [&](components::Sprite* sprite) {
						ImGui::Text("Width: %.0f", sprite->get_width());
						ImGui::SameLine(0.0f, 5.0f);
						ImGui::Text("Height: %.0f", sprite->get_height());

						static std::string s_search = "";
						ImGui::InputTextWithHint("##EntityTextureSearch", "Search...", &s_search, ImGuiInputTextFlags_AutoSelectAll);

						if (ImGui::BeginCombo("Texture", sprite->m_texture.c_str()))
						{
							for (const auto& key : core::ServiceLocator<resource::TextureAtlas>::ref().keys())
							{
								if (key.find(s_search) != std::string::npos)
								{
									const bool selected = (sprite->m_texture == key);
									if (ImGui::Selectable(key.c_str(), selected))
									{
										sprite->m_texture = key;
									}

									if (selected)
									{
										ImGui::SetItemDefaultFocus();
									}
								}
							}

							ImGui::EndCombo();
						}

						ImGui::InputFloat("Opacity", &sprite->m_opacity, 0.01f, 0.1f, "%.1f", numeric_input_flags);
						ImGui::InputInt("Layer", &sprite->m_layer, 1, 2, numeric_input_flags);

						if (ImGui::Button("Create Sprite"))
						{
							if (!sprite->m_texture.empty())
							{
								updates.emplace_back([sprite]() {
									sprite->create(sprite->m_texture, sprite->m_layer, sprite->m_opacity);
								});

								ImGui_Notify::InsertNotification({ImGuiToastType_Success, 2000, "New sprite created."});
							}
							else
							{
								ImGui_Notify::InsertNotification({ImGuiToastType_Error, 2000, "Specify texture buffer."});
							}
						}

						ImGui::SameLine();

						if (ImGui::Button("Update Texture"))
						{
							if (!sprite->m_texture.empty())
							{
								updates.emplace_back([sprite]() {
									sprite->update(sprite->m_texture);
								});

								ImGui_Notify::InsertNotification({ImGuiToastType_Success, 2000, "Sprite updated."});
							}
							else
							{
								ImGui_Notify::InsertNotification({ImGuiToastType_Error, 2000, "Specify texture buffer."});
							}
						}
					});

					draw_component<components::Tag>(selected, "Tag", [](components::Tag* tag) {
						ImGui::InputText("##ComponentTagEditor", &tag->m_tag, ImGuiInputTextFlags_AutoSelectAll);
					});

					draw_component<components::Text>(selected, "Text", [&](components::Text* text) {
						ImGui::Text("Width: %.0f", text->get_width());
						ImGui::SameLine(0.0f, 5.0f);
						ImGui::Text("Height: %.0f", text->get_height());

						ImGui::InputInt("Layer", &text->m_layer, 1, 2, numeric_input_flags);
						ImGui::InputTextMultiline("Text", &text->m_text);
						ImGui::InputFloat("Size", &text->m_size, 1.0f, 5.0f, "%.0f", ImGuiInputTextFlags_CharsNoBlank);

						auto colour = text->m_colour.normalized();
						if (ImGui::ColorEdit4("Colour", &colour[0]))
						{
							text->m_colour.set_from_normalized(colour[0], colour[1], colour[2], colour[3]);
						}

						static std::string s_search = "";
						ImGui::InputTextWithHint("##EntityFontSearch", "Search...", &s_search, ImGuiInputTextFlags_AutoSelectAll);

						if (ImGui::BeginCombo("Font", text->m_font_id.c_str()))
						{
							for (const auto& key : core::ServiceLocator<resource::Fonts>::ref().keys())
							{
								if (key.find(s_search) != std::string::npos)
								{
									const bool selected = (text->m_font_id == key);
									if (ImGui::Selectable(key.c_str(), selected))
									{
										text->m_font_id = key;
									}

									if (selected)
									{
										ImGui::SetItemDefaultFocus();
									}
								}
							}

							ImGui::EndCombo();
						}

						if (ImGui::Button("Create"))
						{
							updates.emplace_back([text]() {
								text->create(text->m_text, text->m_size, text->m_font_id, text->m_colour, text->m_layer);
							});

							ImGui_Notify::InsertNotification({ImGuiToastType_Success, 2000, "Text created."});
						}

						ImGui::SameLine();

						if (ImGui::Button("Update"))
						{
							updates.emplace_back([text]() {
								text->update(text->m_text, text->m_size);
							});

							ImGui_Notify::InsertNotification({ImGuiToastType_Success, 2000, "Text updated."});
						}
					});

					draw_component<components::Transform>(selected, "Transform", [](components::Transform* tf) {
						float pos[2] = {tf->get_pos().x, tf->get_pos().y};
						if (ImGui::InputFloat2("Position", pos, "%.1f", ImGuiInputTextFlags_CharsNoBlank))
						{
							tf->set_pos(pos[0], pos[1]);
						}

						auto rotate = tf->get_rotation();
						if (ImGui::SliderAngle("Rotate", &rotate, 0.0f, 1080.0f))
						{
							tf->set_rotation(rotate);
						}

						float scale[2] = {tf->get_scale().x, tf->get_scale().y};
						if (ImGui::InputFloat2("Scale", scale, "%.1f", ImGuiInputTextFlags_CharsNoBlank))
						{
							tf->set_scale_horizontal(scale[0]);
							tf->set_scale_vertical(scale[1]);
						}
					});

					ImGui::PopID();
				}
			}

			ImGui::End();
		}
	} // namespace panel
} // namespace sc