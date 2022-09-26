///
/// EntityEditor.cpp
/// supercluster
///
/// Refer to LICENSE.txt for more details.
///

#include <entt/entity/registry.hpp>
#include <magic_enum.hpp>
#include <imgui_stdlib.h>
#include <imgui_addons/notify/imgui_notify.h>
#include <imgui_addons/ToggleButton.h>

#include <galaxy/core/ServiceLocator.hpp>
#include <galaxy/core/Window.hpp>
#include <galaxy/components/Animated.hpp>
#include <galaxy/components/DrawShader.hpp>
#include <galaxy/components/Flag.hpp>
#include <galaxy/components/Primitive.hpp>
#include <galaxy/components/Sprite.hpp>
#include <galaxy/components/Tag.hpp>
#include <galaxy/components/Text.hpp>
#include <galaxy/components/Transform.hpp>
#include <galaxy/flags/AllowSerialize.hpp>
#include <galaxy/flags/Enabled.hpp>
#include <galaxy/fs/VirtualFileSystem.hpp>
#include <galaxy/resource/Shaders.hpp>
#include <galaxy/ui/ImGuiHelpers.hpp>

#include "EntityEditor.hpp"

using namespace galaxy;

namespace sc
{
	namespace panel
	{
		void EntityEditor::render(Selected& selected, UpdateStack& updates)
		{
			static constexpr const auto numeric_input_flags =
				ImGuiInputTextFlags_CharsDecimal | ImGuiInputTextFlags_CharsNoBlank | ImGuiInputTextFlags_AutoSelectAll;

			if (ImGui::Begin("Entity", nullptr, ImGuiWindowFlags_AlwaysAutoResize))
			{
				if (selected.m_selected != entt::null)
				{
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
						static std::string s_buff = ds->id();
						if (ImGui::InputText("Shader ID", &s_buff, ImGuiInputTextFlags_EnterReturnsTrue))
						{
							if (!s_buff.empty())
							{
								auto& shaders = core::ServiceLocator<resource::Shaders>::ref();
								if (shaders.has(s_buff))
								{
									ds->set_shader(s_buff);
								}
								else
								{
									s_buff = "";
									ImGui_Notify::InsertNotification({ImGuiToastType_Error, 2000, "Shader does not exist."});
								}
							}
						}
					});

					draw_component<components::Flag>(selected, "Flags", [&](components::Flag* flag) {
						static auto s_enabled = flag->is_flag_set<flags::Enabled>();
						if (ImGui::Checkbox("Is Enabled", &s_enabled))
						{
							if (s_enabled)
							{
								if (selected.m_world->is_valid(selected.m_selected))
								{
									flag->set_flag<flags::Enabled>();
								}
								else
								{
									s_enabled = false;
									ImGui_Notify::InsertNotification({ImGuiToastType_Error, 2000, "Entity did not pass validation."});
								}
							}
							else
							{
								flag->unset_flag<flags::Enabled>();
							}
						}

						ImGui::SameLine();

						static auto s_allow_save = flag->is_flag_set<flags::AllowSerialize>();
						if (ImGui::Checkbox("Allow Serialization", &s_allow_save))
						{
							if (s_allow_save)
							{
								flag->set_flag<flags::AllowSerialize>();
							}
							else
							{
								flag->unset_flag<flags::AllowSerialize>();
							}
						}
					});

					draw_component<components::Primitive>(selected, "Primitive", [&](components::Primitive* primitive) {
						ImGui::Text("Width: %.1f", primitive->get_width());
						ImGui::SameLine(0.0f, 5.0f);
						ImGui::Text("Height: %.1f", primitive->get_height());

						static int s_layer = primitive->get_layer();
						if (ImGui::InputInt("Layer", &s_layer, 1, 2, numeric_input_flags))
						{
							primitive->set_layer(s_layer);
						}

						static constexpr const auto s_types = magic_enum::enum_names<graphics::Shape>();

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

						static float colour[4] = {primitive->m_colour.m_red,
							primitive->m_colour.m_green,
							primitive->m_colour.m_blue,
							primitive->m_colour.m_alpha};
						if (ImGui::ColorEdit4("Colour",
								colour,
								ImGuiColorEditFlags_DisplayRGB | ImGuiColorEditFlags_InputRGB | ImGuiColorEditFlags_AlphaBar | ImGuiColorEditFlags_Uint8))
						{
							primitive->m_colour = {static_cast<std::uint8_t>(colour[0]),
								static_cast<std::uint8_t>(colour[1]),
								static_cast<std::uint8_t>(colour[2]),
								static_cast<std::uint8_t>(colour[3])};
						}

						ImGui::Spacing();

						static components::Primitive::PrimitiveData data = primitive->get_data();
						if (s_type == graphics::Shape::CIRCLE || s_type == graphics::Shape::ELLIPSE)
						{
							ImGui::InputFloat("Fragments", &data.m_fragments, 0.1f, 1.0f, "%.1f", ImGuiInputTextFlags_CharsNoBlank);
						}

						if (s_type == graphics::Shape::CIRCLE)
						{
							ImGui::InputFloat("Radius", &data.m_radius, 0.1f, 1.0f, "%.1f", ImGuiInputTextFlags_CharsNoBlank);
						}

						if (s_type == graphics::Shape::ELLIPSE)
						{
							ImGui::Text("Radii");

							ImGui::SetNextItemWidth(150);
							ImGui::InputFloat("X##1", &data.m_radii.x, 1.0f, 10.0f, "%.1f", ImGuiInputTextFlags_CharsNoBlank);

							ImGui::SameLine();

							ImGui::SetNextItemWidth(150);
							ImGui::InputFloat("Y##2", &data.m_radii.y, 1.0f, 10.0f, "%.1f", ImGuiInputTextFlags_CharsNoBlank);
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
								data.m_points = std::move(points);

								switch (s_type)
								{
									case graphics::Shape::CIRCLE:
										primitive->create<graphics::Shape::CIRCLE>(data, primitive->m_colour, primitive->get_layer());
										break;

									case graphics::Shape::ELLIPSE:
										primitive->create<graphics::Shape::ELLIPSE>(data, primitive->m_colour, primitive->get_layer());
										break;

									case graphics::Shape::LINE:
										primitive->create<graphics::Shape::LINE>(data, primitive->m_colour, primitive->get_layer());
										break;

									case graphics::Shape::POINT:
										primitive->create<graphics::Shape::POINT>(data, primitive->m_colour, primitive->get_layer());
										break;

									case graphics::Shape::POLYGON:
										primitive->create<graphics::Shape::POLYGON>(data, primitive->m_colour, primitive->get_layer());
										break;

									case graphics::Shape::POLYLINE:
										primitive->create<graphics::Shape::POLYLINE>(data, primitive->m_colour, primitive->get_layer());
										break;
								}

								data = {};
								points.clear();
							});
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
						ImGui::Text("Width: %.1f", sprite->get_width());
						ImGui::SameLine(0.0f, 5.0f);
						ImGui::Text("Height: %.1f", sprite->get_height());

						static std::string s_buff = sprite->texture_id();
						ImGui::InputText("Texture Id", &s_buff, ImGuiInputTextFlags_AutoSelectAll);

						static float s_opacity = sprite->get_opacity();
						if (ImGui::InputFloat("Opacity", &s_opacity, 0.01f, 0.1f, "%.1f", numeric_input_flags))
						{
							sprite->set_opacity(s_opacity);
						}

						static int s_layer = sprite->get_layer();
						if (ImGui::InputInt("Layer", &s_layer, 1, 2, numeric_input_flags))
						{
							sprite->set_layer(s_layer);
						}

						if (ImGui::Button("Create Sprite"))
						{
							if (!s_buff.empty())
							{
								updates.emplace_back([&]() {
									sprite->create(s_buff, s_layer, s_opacity);
								});
							}
						}

						ImGui::SameLine();

						if (ImGui::Button("Update Texture"))
						{
							if (!s_buff.empty())
							{
								updates.emplace_back([&]() {
									sprite->update(s_buff);
								});
							}
						}
					});

					draw_component<components::Tag>(selected, "Tag", [](components::Tag* tag) {
						ImGui::InputText("##ComponentTagEditor", &tag->m_tag, ImGuiInputTextFlags_AutoSelectAll);
					});

					draw_component<components::Text>(selected, "Text", [](components::Text* text) {
						/*
						static std::string s_text_buff = text->get_text();
						ImGui::InputText("Text", &s_text_buff);

						static std::string s_font_buff = text->get_font_id();
						if (ImGui::InputText("Font ID", &s_font_buff, ImGuiInputTextFlags_EnterReturnsTrue))
						{
							text->set_font(s_font_buff);
						}

						static float colour[4] = {static_cast<float>(text->get_colour().m_red),
							static_cast<float>(text->get_colour().m_green),
							static_cast<float>(text->get_colour().m_blue),
							static_cast<float>(text->get_colour().m_alpha)};
						if (ImGui::ColorEdit4("Colour",
								colour,
								ImGuiColorEditFlags_DisplayRGB | ImGuiColorEditFlags_InputRGB | ImGuiColorEditFlags_AlphaBar | ImGuiColorEditFlags_Uint8))
						{
							text->change_colour({static_cast<std::uint8_t>(colour[0]),
								static_cast<std::uint8_t>(colour[1]),
								static_cast<std::uint8_t>(colour[2]),
								static_cast<std::uint8_t>(colour[3])});
						}

						if (ImGui::Button("Update"))
						{
							gl_operations.emplace_back([text]() -> void {
								text->create(s_text_buff);
							});
						}
						*/
					});

					draw_component<components::Transform>(selected, "Transform", [](components::Transform* tf) {
						static float pos[2] = {tf->get_pos().x, tf->get_pos().y};
						if (ImGui::InputFloat2("Position", pos, "%.1f", ImGuiInputTextFlags_CharsNoBlank | ImGuiInputTextFlags_EnterReturnsTrue))
						{
							tf->set_pos(pos[0], pos[1]);
						}

						static auto s_rotate = tf->get_rotation();
						if (ImGui::SliderAngle("Rotate", &s_rotate, 0.0f, 360.0f))
						{
							tf->set_rotation(s_rotate);
						}

						static auto s_scale = tf->get_scale();
						if (ImGui::InputFloat("Scale", &s_scale, 0.1f, 1.0f, "%.1f", ImGuiInputTextFlags_CharsNoBlank))
						{
							tf->scale(s_scale);
						}
					});
				}
			}

			ImGui::End();
		}
	} // namespace panel
} // namespace sc