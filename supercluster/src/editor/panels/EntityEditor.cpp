///
/// EntityEditor.cpp
/// supercluster
///
/// Refer to LICENSE.txt for more details.
///

#include <entt/entity/registry.hpp>
#include <imgui_stdlib.h>
#include <magic_enum/magic_enum.hpp>

#include <galaxy/components/Animated.hpp>
#include <galaxy/components/MapData.hpp>
#include <galaxy/components/ParticleGenerator.hpp>
#include <galaxy/components/Primitive.hpp>
#include <galaxy/components/RigidBody.hpp>
#include <galaxy/components/Script.hpp>
#include <galaxy/components/Sprite.hpp>
#include <galaxy/components/Tag.hpp>
#include <galaxy/components/Text.hpp>
#include <galaxy/components/Transform.hpp>
#include <galaxy/components/UIScript.hpp>
#include <galaxy/core/ServiceLocator.hpp>
#include <galaxy/core/Window.hpp>
#include <galaxy/flags/DenySerialization.hpp>
#include <galaxy/flags/Disabled.hpp>
#include <galaxy/fs/VirtualFileSystem.hpp>
#include <galaxy/resource/Fonts.hpp>
#include <galaxy/resource/Shaders.hpp>
#include <galaxy/resource/TextureAtlas.hpp>
#include <galaxy/scene/Scene.hpp>
#include <galaxy/ui/ImGuiHelpers.hpp>

#include "EntityEditor.hpp"

using namespace galaxy;

namespace sc
{
	namespace panel
	{
		static constexpr const auto s_types = magic_enum::enum_names<math::Shape>();

		void EntityEditor::render(Selected& selected, UpdateStack& updates)
		{
			static const meta::vector<std::string> b2_body_types = {"b2_dynamicBody", "b2_kinematicBody", "b2_staticBody"};

			static constexpr const auto numeric_input_flags =
				ImGuiInputTextFlags_CharsDecimal | ImGuiInputTextFlags_CharsNoBlank | ImGuiInputTextFlags_AutoSelectAll;
			thread_local constexpr const ImGuiInputTextFlags lua_double_flags = ImGuiInputTextFlags_AutoSelectAll | ImGuiInputTextFlags_CharsNoBlank;
			thread_local constexpr const ImGuiInputTextFlags lua_text_flags   = ImGuiInputTextFlags_EnterReturnsTrue | ImGuiInputTextFlags_AutoSelectAll;

			if (ImGui::Begin(ICON_MDI_DATABASE " Entity", nullptr, ImGuiWindowFlags_AlwaysAutoResize))
			{
				if (selected.m_selected != entt::null && selected.m_scene != nullptr && selected.m_scene->m_registry.valid(selected.m_selected))
				{
					ImGui::PushID(static_cast<std::uint32_t>(selected.m_selected));

					ImGui::Text("EnTT Id: %u", entt::to_integral(selected.m_selected));

					ImGui::SameLine();

					ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.65f, 0.15f, 0.15f, 1.f));
					ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.8f, 0.3f, 0.3f, 1.f));
					ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(1.f, 0.2f, 0.2f, 1.f));
					if (ImGui::Button("Destroy"))
					{
						updates.push_back([&]() {
							selected.m_scene->m_registry.destroy(selected.m_selected);
							selected.m_selected = entt::null;
						});
					}
					ImGui::PopStyleColor(3);

					auto tag = selected.m_scene->m_registry.try_get<components::Tag>(selected.m_selected);
					if (tag)
					{
						ImGui::InputText("##Tag", &tag->m_tag, ImGuiInputTextFlags_AutoSelectAll);
					}

					if (ImGui::Button("Set Flags"))
					{
						ImGui::OpenPopup("EntityFlagsPopup");
					}

					if (ImGui::BeginPopup("EntityFlagsPopup"))
					{
						auto disabled = selected.m_scene->m_registry.all_of<flags::Disabled>(selected.m_selected);
						if (ImGui::Checkbox("Disabled", &disabled))
						{
							if (disabled)
							{
								selected.m_scene->m_registry.emplace_or_replace<flags::Disabled>(selected.m_selected);
							}
							else
							{
								if (selected.m_scene->is_valid(selected.m_selected))
								{
									selected.m_scene->m_registry.remove<flags::DenySerialization>(selected.m_selected);
								}
								else
								{
									ui::imgui_notify_error("Entity did not pass validation.");
								}
							}
						}

						ImGui::SameLine();

						auto deny_save = selected.m_scene->m_registry.all_of<flags::DenySerialization>(selected.m_selected);
						if (ImGui::Checkbox("Deny Serialization", &deny_save))
						{
							if (deny_save)
							{
								selected.m_scene->m_registry.emplace_or_replace<flags::DenySerialization>(selected.m_selected);
							}
							else
							{
								selected.m_scene->m_registry.remove<flags::DenySerialization>(selected.m_selected);
							}
						}

						ImGui::EndPopup();
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
						draw_entry<components::ParticleGenerator>(selected, "ParticleGenerator");
						draw_entry<components::Primitive>(selected, "Primitive");
						draw_entry<components::RigidBody>(selected, "RigidBody");
						draw_entry<components::Script>(selected, "Script");
						draw_entry<components::Sprite>(selected, "Sprite");
						draw_entry<components::Tag>(selected, "Tag");
						draw_entry<components::Text>(selected, "Text");
						draw_entry<components::Transform>(selected, "Transform");
						draw_entry<components::UIScript>(selected, "UIScript");

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

							static meta::vector<graphics::Frame> s_frames = {};
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

										ui::imgui_notify_success("New frame added.");

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

									ui::imgui_notify_success("New animation added.");

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

					draw_component<components::MapData>(selected, "MapData", [](components::MapData* mapdata) {
						ImGui::Text("Texture ID: %u", mapdata->m_texture);
						ImGui::Text("Layer: %i", mapdata->m_layer);
						ImGui::Text("Animation Count: %u", mapdata->m_animations.size());
					});

					draw_component<components::ParticleGenerator>(selected, "ParticleGenerator", [&](components::ParticleGenerator* pg) {
						if (ImGui::Button("Generate"))
						{
							updates.emplace_back([pg]() {
								pg->regenerate();
							});
						}

						ImGui::SameLine();

						if (ImGui::Button("Reset Particles"))
						{
							pg->reset();
						}

						ImGui::InputInt("Count", &pg->m_count);
						ImGui::InputInt("Layer", &pg->m_layer);
						ImGui::Checkbox("Keep Aspect Ratio", &pg->m_keep_scale_aspect_ratio);
						ui::imgui_glm_vec2("Start Pos", pg->m_start_pos);

						ImGui::Spacing();

						m_filter_tex_pg.DrawWithHint("###EntityTextureSearch", ICON_MDI_MAGNIFY "Search...", ImGui::GetContentRegionAvail().x);

						if (ImGui::BeginCombo("Texture", pg->m_texture.c_str()))
						{
							for (const auto& key : core::ServiceLocator<resource::TextureAtlas>::ref().keys())
							{
								if (m_filter_tex_pg.PassFilter(key.c_str()))
								{
									const bool selected = (pg->m_texture == key);
									if (ImGui::Selectable(key.c_str(), selected))
									{
										pg->m_texture = key;
									}

									if (selected)
									{
										ImGui::SetItemDefaultFocus();
									}
								}
							}

							ImGui::EndCombo();
						}

						ImGui::Spacing();

						ImGui::TextUnformatted("Randomize:");

						ImGui::Checkbox("Position", &pg->m_randomize_position);
						ImGui::SameLine();
						ImGui::Checkbox("Init Vel", &pg->m_randomize_initial_vel);
						ImGui::SameLine();
						ImGui::Checkbox("Life", &pg->m_randomize_life);

						ImGui::Checkbox("Scale", &pg->m_randomize_scale);
						ImGui::SameLine();
						ImGui::Checkbox("Colour", &pg->m_randomize_colour);
						ImGui::SameLine();
						ImGui::Checkbox("Alpha", &pg->m_randomize_colour_alpha);

						static auto                       s_selected = std::string(magic_enum::enum_name(pg->m_spread));
						static std::array<std::string, 2> s_spreads  = {"SPREAD_TYPE_RECTANGLE", "SPREAD_TYPE_SPHERE"};

						if (ImGui::BeginCombo("Spread", s_selected.c_str()))
						{
							for (const auto& name : s_spreads)
							{
								const bool selected = (s_selected == name);
								if (ImGui::Selectable(name.c_str(), selected))
								{
									s_selected   = name;
									pg->m_spread = magic_enum::enum_cast<graphics::ParticleSpread>(s_selected).value();
								}

								if (selected)
								{
									ImGui::SetItemDefaultFocus();
								}
							}

							ImGui::EndCombo();
						}

						ImGui::InputFloat("Spread Radius", &pg->m_spread_radius, 0.1f, 1.0f, "%.1f");

						ImGui::Spacing();

						ImGui::TextUnformatted("Rect Spread:");
						ImGui::PushID("rect");
						ui::imgui_glm_vec2("Min", pg->m_min_rect_spread);
						ui::imgui_glm_vec2("Max", pg->m_max_rect_spread);
						ImGui::PopID();

						ImGui::Spacing();

						ImGui::TextUnformatted("Velocity:");
						ImGui::PushID("vel");
						ui::imgui_glm_vec2("Fixed", pg->m_fixed_vel);
						ui::imgui_glm_vec2("Min", pg->m_min_vel);
						ui::imgui_glm_vec2("Max", pg->m_max_vel);
						ImGui::PopID();

						ImGui::Spacing();

						ImGui::TextUnformatted("Life:");
						ImGui::PushID("life");
						ImGui::InputFloat("Fixed", &pg->m_fixed_life);
						ImGui::InputFloat("Min", &pg->m_min_life);
						ImGui::InputFloat("Max", &pg->m_max_life);
						ImGui::PopID();

						ImGui::Spacing();

						ImGui::TextUnformatted("Scale:");
						ImGui::PushID("scale");
						ui::imgui_glm_vec2("Fixed", pg->m_fixed_scale);
						ui::imgui_glm_vec2("Min", pg->m_min_scale);
						ui::imgui_glm_vec2("Max", pg->m_max_scale);
						ImGui::PopID();

						ImGui::Spacing();

						if (ImGui::CollapsingHeader("Tint"))
						{
							ImGui::PushID("tint");

							float fixed_col[3] = {
								pg->m_fixed_colour.x,
								pg->m_fixed_colour.y,
								pg->m_fixed_colour.z,
							};
							ImGui::ColorPicker3("Fixed", fixed_col);

							float min_col[3] = {
								pg->m_fixed_colour.x,
								pg->m_fixed_colour.y,
								pg->m_fixed_colour.z,
							};
							ImGui::ColorPicker3("Min", min_col);

							float max_col[3] = {
								pg->m_max_colour.x,
								pg->m_max_colour.y,
								pg->m_max_colour.z,
							};
							ImGui::ColorPicker3("Max", max_col);

							ImGui::PopID();
						}

						ImGui::Spacing();

						ImGui::TextUnformatted("Alpha:");
						ImGui::PushID("alpha");

						if (ImGui::InputFloat("Fixed", &pg->m_fixed_alpha, 0.01f, 0.1f, "%.1f"))
						{
							pg->m_fixed_alpha = std::clamp(pg->m_fixed_alpha, 0.0f, 1.0f);
						}

						if (ImGui::InputFloat("Min", &pg->m_min_alpha, 0.01f, 0.1f, "%.1f"))
						{
							pg->m_min_alpha = std::clamp(pg->m_min_alpha, 0.0f, 1.0f);
						}

						if (ImGui::InputFloat("Max", &pg->m_max_alpha, 0.01f, 0.1f, "%.1f"))
						{
							pg->m_max_alpha = std::clamp(pg->m_max_alpha, 0.0f, 1.0f);
						}

						ImGui::PopID();
					});

					draw_component<components::Primitive>(selected, "Primitive", [&](components::Primitive* primitive) {
						ImGui::Text("Width: %.0f", primitive->get_width());
						ImGui::SameLine(0.0f, 5.0f);
						ImGui::Text("Height: %.0f", primitive->get_height());

						ImGui::InputInt("Layer", &primitive->m_layer, 1, 2, numeric_input_flags);

						static auto s_selected = std::string(magic_enum::enum_name(primitive->get_shape()));
						static auto s_type     = primitive->get_shape();
						if (ImGui::BeginCombo("Type", s_selected.c_str()))
						{
							for (const auto& name : s_types)
							{
								const bool selected = (s_selected == name);
								if (ImGui::Selectable(std::string(name).c_str(), selected))
								{
									s_selected = name;
									s_type     = magic_enum::enum_cast<math::Shape>(s_selected).value();
								}

								if (selected)
								{
									ImGui::SetItemDefaultFocus();
								}
							}

							ImGui::EndCombo();
						}

						auto colour = primitive->m_colour.to_array();
						if (ImGui::ColorEdit4("Colour", &colour[0]))
						{
							primitive->m_colour.set_from_normalized(colour[0], colour[1], colour[2], colour[3]);
						}

						ImGui::Spacing();

						static components::Primitive::PrimitiveData data = primitive->get_data();
						if (s_type == math::Shape::CIRCLE || s_type == math::Shape::ELLIPSE)
						{
							ImGui::InputFloat("Fragments", &data.fragments, 0.1f, 1.0f, "%.1f", ImGuiInputTextFlags_CharsNoBlank);
						}

						if (s_type == math::Shape::CIRCLE)
						{
							ImGui::InputFloat("Radius", &data.radius, 0.1f, 1.0f, "%.1f", ImGuiInputTextFlags_CharsNoBlank);
						}

						if (s_type == math::Shape::ELLIPSE)
						{
							ImGui::Text("Radii");

							ImGui::SetNextItemWidth(150);
							ImGui::InputFloat("X##1", &data.radii.x, 1.0f, 10.0f, "%.1f", ImGuiInputTextFlags_CharsNoBlank);

							ImGui::SameLine();

							ImGui::SetNextItemWidth(150);
							ImGui::InputFloat("Y##2", &data.radii.y, 1.0f, 10.0f, "%.1f", ImGuiInputTextFlags_CharsNoBlank);
						}

						if (s_type == math::Shape::LINE)
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

						static meta::vector<glm::vec2> points = {};
						if (s_type == math::Shape::POLYLINE || s_type == math::Shape::POLYGON)
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
								ui::imgui_glm_vec2("Point", s_point);

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
									case math::Shape::CIRCLE:
										primitive->create<math::Shape::CIRCLE>(data, primitive->m_colour, primitive->m_layer);
										break;

									case math::Shape::ELLIPSE:
										primitive->create<math::Shape::ELLIPSE>(data, primitive->m_colour, primitive->m_layer);
										break;

									case math::Shape::LINE:
										primitive->create<math::Shape::LINE>(data, primitive->m_colour, primitive->m_layer);
										break;

									case math::Shape::POINT:
										primitive->create<math::Shape::POINT>(data, primitive->m_colour, primitive->m_layer);
										break;

									case math::Shape::POLYGON:
										primitive->create<math::Shape::POLYGON>(data, primitive->m_colour, primitive->m_layer);
										break;

									case math::Shape::POLYLINE:
										primitive->create<math::Shape::POLYLINE>(data, primitive->m_colour, primitive->m_layer);
										break;
								}

								data = {};
								points.clear();
							});

							ui::imgui_notify_success("Successfully created primitive.");
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
							const auto path = core::ServiceLocator<fs::VirtualFileSystem>::ref().open_file_dialog({"*.lua"});
							if (!path.empty())
							{
								auto str = "{\"file\":\"" + path + "\"}";
								strutils::replace_all(str, "\\", "/");
								selected.m_scene->m_registry.emplace_or_replace<components::Script>(selected.m_selected, nlohmann::json::parse(str));
							}
						}

						ImGui::SameLine();

						if (ImGui::Button("Reload"))
						{
							if (!script->file().empty())
							{
								auto str = "{\"file\":\"" + script->file() + "\"}";
								strutils::replace_all(str, "\\", "/");
								selected.m_scene->m_registry.emplace_or_replace<components::Script>(selected.m_selected, nlohmann::json::parse(str));
							}
						}

						ImGui::SameLine();

						if (ImGui::Button("Copy Path to Clipboard"))
						{
							core::ServiceLocator<core::Window>::ref().get_input<input::Clipboard>().set(script->file().c_str());
						}

						if (ImGui::CollapsingHeader("Data", ImGuiTreeNodeFlags_SpanAvailWidth))
						{
							ImGui::TextUnformatted("Show:");
							ImGui::Checkbox("Functions", &script->m_show_functions);
							ImGui::SameLine();
							ImGui::Checkbox("Userdata", &script->m_show_userdata);
							ImGui::SameLine();
							ImGui::Checkbox("Other", &script->m_show_unknown);

							ImGui::Spacing();
							ImGui::Spacing();

							if (script->m_self.valid())
							{
								for (auto& [id, value] : script->m_self)
								{
									const auto name = id.as<std::string>();
									switch (value.get_type())
									{
										case sol::type::number:
											{
												auto num = value.as<double>();
												if (ImGui::InputDouble(name.c_str(), &num, 0.1, 1.0, "%.1f", lua_double_flags))
												{
													script->m_self[name] = num;
												}
											}
											break;

										case sol::type::boolean:
											{
												auto var = value.as<bool>();
												if (ImGui::Checkbox(name.c_str(), &var))
												{
													script->m_self[name] = var;
												}
											}
											break;

										case sol::type::string:
											{
												auto str = value.as<std::string>();
												if (ImGui::InputText(name.c_str(), &str, lua_text_flags))
												{
													script->m_self[name] = str;
												}
											}
											break;

										case sol::type::function:
											if (script->m_show_functions)
											{
												ImGui::Text("Function: %s", name.c_str());
											}
											break;

										case sol::type::userdata:
										case sol::type::lightuserdata:
											if (script->m_show_userdata)
											{
												ImGui::Text("Userdata: %s", name.c_str());
											}
											break;

										default:
											if (script->m_show_unknown)
											{
												const auto type = std::string {magic_enum::enum_name(value.get_type())};
												ImGui::Text("%s: %s", type.c_str(), name.c_str());
											}
											break;
									}
								}
							}
						}
					});

					draw_component<components::Sprite>(selected, "Sprite", [&](components::Sprite* sprite) {
						ImGui::Text("Width: %.0f", sprite->get_width());
						ImGui::SameLine(0.0f, 5.0f);
						ImGui::Text("Height: %.0f", sprite->get_height());

						m_filter_textures.DrawWithHint("###EntityTextureSearch", ICON_MDI_MAGNIFY "Search...", ImGui::GetContentRegionAvail().x);

						if (ImGui::BeginCombo("Texture", sprite->m_tex_name.c_str()))
						{
							for (const auto& key : core::ServiceLocator<resource::TextureAtlas>::ref().keys())
							{
								if (m_filter_textures.PassFilter(key.c_str()))
								{
									const bool selected = (sprite->m_tex_name == key);
									if (ImGui::Selectable(key.c_str(), selected))
									{
										sprite->m_tex_name = key;
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
							if (!sprite->m_tex_name.empty())
							{
								updates.emplace_back([sprite]() {
									sprite->create(sprite->m_tex_name, sprite->m_layer, sprite->m_opacity);
								});

								ui::imgui_notify_success("New sprite created.");
							}
							else
							{
								ui::imgui_notify_error("Specify texture buffer.");
							}
						}

						ImGui::SameLine();

						if (ImGui::Button("Update Texture"))
						{
							if (!sprite->m_tex_name.empty())
							{
								updates.emplace_back([sprite]() {
									sprite->update(sprite->m_tex_name);
								});

								ui::imgui_notify_success("Sprite updated.");
							}
							else
							{
								ui::imgui_notify_error("Specify texture buffer.");
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

						static auto s_selected = std::string(magic_enum::enum_name(text->get_alignment()));
						static auto s_type     = text->get_alignment();
						if (ImGui::BeginCombo("Alignment", s_selected.c_str()))
						{
							for (const auto& name : s_types)
							{
								const bool selected = (s_selected == name);
								if (ImGui::Selectable(std::string(name).c_str(), selected))
								{
									s_selected = name;
									s_type     = magic_enum::enum_cast<components::Text::Alignment>(s_selected).value();
								}

								if (selected)
								{
									ImGui::SetItemDefaultFocus();
								}
							}

							ImGui::EndCombo();
						}

						auto colour = text->m_colour.to_array();
						if (ImGui::ColorEdit4("Colour", &colour[0]))
						{
							text->m_colour.set_from_normalized(colour[0], colour[1], colour[2], colour[3]);
						}

						m_filter_fonts.DrawWithHint("###EntityFontSearch", ICON_MDI_MAGNIFY "Search...", ImGui::GetContentRegionAvail().x);

						if (ImGui::BeginCombo("Font", text->m_font_name.c_str()))
						{
							for (const auto& key : core::ServiceLocator<resource::Fonts>::ref().keys())
							{
								if (m_filter_fonts.PassFilter(key.c_str()))
								{
									const bool selected = (text->m_font_name == key);
									if (ImGui::Selectable(key.c_str(), selected))
									{
										text->m_font_name = key;
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
								text->create(text->m_text, text->m_size, text->m_font_name, text->m_colour, text->m_layer, text->m_alignment);
							});

							ui::imgui_notify_success("Text created.");
						}

						ImGui::SameLine();

						if (ImGui::Button("Update"))
						{
							updates.emplace_back([text]() {
								text->update(text->m_text, text->m_size, text->m_colour, text->m_alignment);
							});

							ui::imgui_notify_success("Text updated.");
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

					draw_component<components::UIScript>(selected, "UIScript", [&](components::UIScript* ui) {
						if (ImGui::Button("Load"))
						{
							const auto path = core::ServiceLocator<fs::VirtualFileSystem>::ref().open_file_dialog({"*.lua"});
							if (!path.empty())
							{
								auto str = "{\"file\":\"" + path + "\"}";
								strutils::replace_all(str, "\\", "/");
								selected.m_scene->m_registry.emplace_or_replace<components::UIScript>(selected.m_selected, nlohmann::json::parse(str));
							}
						}

						ImGui::SameLine();

						if (ImGui::Button("Reload"))
						{
							if (!ui->file().empty())
							{
								auto str = "{\"file\":\"" + ui->file() + "\"}";
								strutils::replace_all(str, "\\", "/");
								selected.m_scene->m_registry.emplace_or_replace<components::UIScript>(selected.m_selected, nlohmann::json::parse(str));
							}
						}

						ImGui::SameLine();

						if (ImGui::Button("Copy Path to Clipboard"))
						{
							core::ServiceLocator<core::Window>::ref().get_input<input::Clipboard>().set(ui->file().c_str());
						}

						ImGui::Text("Loaded file: %s", ui->file().c_str());
					});

					ImGui::PopID();
				}
			}

			ImGui::End();
		}
	} // namespace panel
} // namespace sc
