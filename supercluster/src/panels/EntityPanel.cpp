///
/// EntityPanel.cpp
/// supercluster
///
/// Refer to LICENSE.txt for more details.
///

#include <galaxy/components/Animated.hpp>
#include <galaxy/components/Circle.hpp>
#include <galaxy/components/Ellipse.hpp>
#include <galaxy/components/GUI.hpp>
#include <galaxy/components/Point.hpp>
#include <galaxy/components/Polygon.hpp>
#include <galaxy/components/Polyline.hpp>
#include <galaxy/components/RenderCommand.hpp>
#include <galaxy/components/RigidBody.hpp>
#include <galaxy/components/Script.hpp>
#include <galaxy/components/Sprite.hpp>
#include <galaxy/components/Tag.hpp>
#include <galaxy/components/Text.hpp>
#include <galaxy/components/TileMap.hpp>
#include <galaxy/components/Transform.hpp>
#include <galaxy/flags/DenySerialization.hpp>
#include <galaxy/flags/Disabled.hpp>
#include <galaxy/resource/Fonts.hpp>
#include <galaxy/resource/Textures.hpp>

#include "EntityPanel.hpp"

namespace sc
{
	EntityPanel::EntityPanel()
	{
		for (const auto& key : core::ServiceLocator<resource::Textures>::ref().keys())
		{
			m_texture_data.items.push_back(key);
		}

		for (const auto& key : core::ServiceLocator<resource::Fonts>::ref().keys())
		{
			m_font_data.items.push_back(key);
		}

		m_alignment_data.items.emplace_back("LEFT");
		m_alignment_data.items.emplace_back("CENTER");
		m_alignment_data.items.emplace_back("RIGHT");
	}

	EntityPanel::~EntityPanel()
	{
	}

	void EntityPanel::render(meta::vector<std::function<void(void)>>& tasks, Selected& selected)
	{
		if (m_show)
		{
			ImGui::Begin(ICON_MDI_DATABASE " Entity", nullptr);

			if (selected.entity != entt::null && selected.scene != nullptr && selected.scene->m_registry.m_entt.valid(selected.entity))
			{
				ImGui::PushID(static_cast<std::uint32_t>(selected.entity));

				ImGui::Text("ID: %u", entt::to_integral(selected.entity));

				ImGui::SameLine();

				ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.65f, 0.15f, 0.15f, 1.f));
				ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.8f, 0.3f, 0.3f, 1.f));
				ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(1.f, 0.2f, 0.2f, 1.f));
				if (ImGui::Button("Destroy"))
				{
					tasks.push_back([&]() {
						selected.scene->m_registry.m_entt.destroy(selected.entity);

						selected.entity = entt::null;
						selected.scene  = nullptr;
					});
				}
				ImGui::PopStyleColor(3);

				ImGui::Spacing();
				ImGui::Separator();
				ImGui::Spacing();

				auto tag = selected.scene->m_registry.m_entt.try_get<components::Tag>(selected.entity);
				if (tag)
				{
					ImGui::InputText("##Tag", &tag->m_tag, ImGuiInputTextFlags_AutoSelectAll);
				}

				if (ImGui::Button("Set Flags"))
				{
					m_open_flags = true;
				}

				ui::imgui_popup("EntityFlagsPopup", m_open_flags, [&]() {
					auto disabled = selected.scene->m_registry.m_entt.all_of<flags::Disabled>(selected.entity);
					if (ImGui::Checkbox("Disabled", &disabled))
					{
						if (disabled)
						{
							selected.scene->m_registry.m_entt.emplace_or_replace<flags::Disabled>(selected.entity);
						}
						else
						{
							selected.scene->m_registry.m_entt.remove<flags::DenySerialization>(selected.entity);
						}
					}

					ImGui::SetTooltip("Stops an entities components from being updated.");
					ImGui::SameLine();

					auto deny_save = selected.scene->m_registry.m_entt.all_of<flags::DenySerialization>(selected.entity);
					if (ImGui::Checkbox("Disable Save", &deny_save))
					{
						if (deny_save)
						{
							selected.scene->m_registry.m_entt.emplace_or_replace<flags::DenySerialization>(selected.entity);
						}
						else
						{
							selected.scene->m_registry.m_entt.remove<flags::DenySerialization>(selected.entity);
						}
					}

					ImGui::SetTooltip("Disable the ability to serialize this entity.");
				});

				ImGui::SameLine();
				ImGui::PushItemWidth(-1);

				if (ImGui::Button("Add Component"))
				{
					m_open_components = true;
				}

				ui::imgui_popup("AddNewComponent", m_open_components, [&]() {
					draw_entry<components::Animated>(selected, "Animated");
					draw_entry<components::Circle>(selected, "Circle");
					draw_entry<components::Ellipse>(selected, "Ellipse");
					draw_entry<components::GUI>(selected, "GUI");
					draw_entry<components::Point>(selected, "Point");
					draw_entry<components::Polygon>(selected, "Polygon");
					draw_entry<components::Polyline>(selected, "Polyline");
					draw_entry<components::RenderCommand>(selected, "RenderCommand");
					draw_entry<components::RigidBody>(selected, "RigidBody");
					draw_entry<components::Script>(selected, "Script");
					draw_entry<components::Sprite>(selected, "Sprite");
					draw_entry<components::Tag>(selected, "Tag");
					draw_entry<components::Text>(selected, "Text");
					draw_entry<components::Transform>(selected, "Transform");

					// system added components
					// draw_entry<components::TileMap>(selected, "TileMap");
				});

				ImGui::PopItemWidth();

				draw_component<components::Animated>(selected, "Animated", [&](components::Animated* animated) {
					/*
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
					*/
				});

				draw_component<components::Circle>(selected, "Circle", [&](components::Circle* circle) {
					auto frag = circle->m_shape.fragments();
					if (ImGui::InputFloat("Fragments", &frag, 0.1f, 1.0f, "%.1f"))
					{
						tasks.push_back([&]() {
							circle->m_shape.create(frag, circle->m_shape.radius());
						});
					}

					auto rad = circle->m_shape.radius();
					if (ImGui::InputFloat("Radius", &rad, 0.1f, 1.0f, "%.1f"))
					{
						tasks.push_back([&]() {
							circle->m_shape.create(circle->m_shape.fragments(), rad);
						});
					}

					auto& tint = circle->m_shape.m_colour.vec4();
					if (ImGui::ColorEdit4("Colour", &tint.x))
					{
						circle->m_shape.m_colour.set_rgba(tint);
					}
				});

				draw_component<components::Ellipse>(selected, "Ellipse", [&](components::Ellipse* ellipse) {
					auto frag = ellipse->m_shape.fragments();
					if (ImGui::InputFloat("Fragments", &frag, 0.1f, 1.0f, "%.1f"))
					{
						tasks.push_back([&]() {
							ellipse->m_shape.create(frag, ellipse->m_shape.radii());
						});
					}

					glm::vec2 radii = ellipse->m_shape.radii();
					if (ui::imgui_glm_vec2("Radii", radii))
					{
						ellipse->m_shape.create(ellipse->m_shape.fragments(), radii);
					}

					auto& tint = ellipse->m_shape.m_colour.vec4();
					if (ImGui::ColorEdit4("Colour", &tint.x))
					{
						ellipse->m_shape.m_colour.set_rgba(tint);
					}
				});

				draw_component<components::GUI>(selected, "GUI", [&](components::GUI* gui) {
					if (ImGui::Button("Load"))
					{
						const auto path = core::ServiceLocator<fs::VirtualFileSystem>::ref().open_file_dialog({"*.lua"});
						if (!path.empty())
						{
							auto str = "{\"file\":\"" + path + "\"}";
							strutils::replace_all(str, "\\", "/");
							selected.scene->m_registry.m_entt.emplace_or_replace<components::GUI>(selected.entity, nlohmann::json::parse(str));
						}
					}

					ImGui::SameLine();

					if (ImGui::Button("Reload"))
					{
						if (!gui->file().empty())
						{
							auto str = "{\"file\":\"" + gui->file() + "\"}";
							strutils::replace_all(str, "\\", "/");
							selected.scene->m_registry.m_entt.emplace_or_replace<components::GUI>(selected.entity, nlohmann::json::parse(str));
						}
					}

					if (ImGui::CollapsingHeader("Data", ImGuiTreeNodeFlags_SpanAvailWidth))
					{
						if (gui->m_self.valid())
						{
							for (auto& [id, value] : gui->m_self)
							{
								const auto name = id.as<std::string>();
								switch (value.get_type())
								{
									case sol::type::number:
										{
											auto num = value.as<double>();
											if (ImGui::InputDouble(name.c_str(),
													&num,
													0.1,
													1.0,
													"%.1f",
													ImGuiInputTextFlags_AutoSelectAll | ImGuiInputTextFlags_CharsNoBlank))
											{
												gui->m_self[name] = num;
											}
										}
										break;

									case sol::type::boolean:
										{
											auto var = value.as<bool>();
											if (ImGui::Checkbox(name.c_str(), &var))
											{
												gui->m_self[name] = var;
											}
										}
										break;

									case sol::type::string:
										{
											auto str = value.as<std::string>();
											if (ImGui::InputText(name.c_str(), &str, ImGuiInputTextFlags_EnterReturnsTrue | ImGuiInputTextFlags_AutoSelectAll))
											{
												gui->m_self[name] = str;
											}
										}
										break;

									case sol::type::function:
										ImGui::Text("Function: %s", name.c_str());
										break;

									case sol::type::userdata:
									case sol::type::lightuserdata:
										ImGui::Text("Userdata: %s", name.c_str());
										break;

									default:
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

				draw_component<components::Point>(selected, "Point", [&](components::Point* point) {
					glm::vec2 pos = point->m_shape.pos();
					if (ui::imgui_glm_vec2("Pos", pos))
					{
						point->m_shape.create(pos);
					}

					auto& tint = point->m_shape.m_colour.vec4();
					if (ImGui::ColorEdit4("Colour", &tint.x))
					{
						point->m_shape.m_colour.set_rgba(tint);
					}
				});

				draw_component<components::Polygon>(selected, "Polygon", [&](components::Polygon* polygon) {
					static meta::vector<glm::vec2> points = polygon->m_shape.points();

					static glm::vec2 s_point {0.0f, 0.0f};
					ui::imgui_glm_vec2("Add Point", s_point);

					if (ImGui::Button("Push Point"))
					{
						points.emplace_back(s_point);

						s_point.x = 0.0f;
						s_point.y = 0.0f;
					}

					ImGui::SameLine();

					if (ImGui::Button("Clear Points"))
					{
						points.clear();
					}

					auto& tint = polygon->m_shape.m_colour.vec4();
					if (ImGui::ColorEdit4("Colour", &tint.x))
					{
						polygon->m_shape.m_colour.set_rgba(tint);
					}

					ImGui::Text("Points");
					for (const auto& point : points)
					{
						ImGui::BulletText("{X: %.1f, Y: %.1f}", point.x, point.y);
					}

					if (ImGui::Button("Create"))
					{
						polygon->m_shape.create(points);
					}
				});

				draw_component<components::Polyline>(selected, "Polyline", [&](components::Polyline* polyline) {
					static meta::vector<glm::vec2> points = polyline->m_shape.points();

					static glm::vec2 s_point {0.0f, 0.0f};
					ui::imgui_glm_vec2("Add Point", s_point);

					if (ImGui::Button("Push Point"))
					{
						points.emplace_back(s_point);

						s_point.x = 0.0f;
						s_point.y = 0.0f;
					}

					ImGui::SameLine();

					if (ImGui::Button("Clear Points"))
					{
						points.clear();
					}

					auto& tint = polyline->m_shape.m_colour.vec4();
					if (ImGui::ColorEdit4("Colour", &tint.x))
					{
						polyline->m_shape.m_colour.set_rgba(tint);
					}

					ImGui::Text("Points");
					for (const auto& point : points)
					{
						ImGui::BulletText("{X: %.1f, Y: %.1f}", point.x, point.y);
					}

					if (ImGui::Button("Create"))
					{
						polyline->m_shape.create(points);
					}
				});

				draw_component<components::RenderCommand>(selected, "RenderCommand", [&](components::RenderCommand* cmd) {
					ImGui::InputInt("Instances", &cmd->m_command.instances, 1, 10);
					ImGui::InputInt("Layer", &cmd->m_command.layer, 1, 10);
					ImGui::Checkbox("Textured", &cmd->m_command.uniforms.textured);
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
						for (const char* type : {"b2_dynamicBody", "b2_kinematicBody", "b2_staticBody"})
						{
							const bool selected = (type_selected == type);
							if (ImGui::Selectable(type, selected))
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
							selected.scene->m_registry.m_entt.emplace_or_replace<components::Script>(selected.entity, nlohmann::json::parse(str));
						}
					}

					ImGui::SameLine();

					if (ImGui::Button("Reload"))
					{
						if (!script->file().empty())
						{
							auto str = "{\"file\":\"" + script->file() + "\"}";
							strutils::replace_all(str, "\\", "/");
							selected.scene->m_registry.m_entt.emplace_or_replace<components::Script>(selected.entity, nlohmann::json::parse(str));
						}
					}

					if (ImGui::CollapsingHeader("Data", ImGuiTreeNodeFlags_SpanAvailWidth))
					{
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
											if (ImGui::InputDouble(name.c_str(),
													&num,
													0.1,
													1.0,
													"%.1f",
													ImGuiInputTextFlags_AutoSelectAll | ImGuiInputTextFlags_CharsNoBlank))
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
											if (ImGui::InputText(name.c_str(), &str, ImGuiInputTextFlags_EnterReturnsTrue | ImGuiInputTextFlags_AutoSelectAll))
											{
												script->m_self[name] = str;
											}
										}
										break;

									case sol::type::function:
										ImGui::Text("Function: %s", name.c_str());
										break;

									case sol::type::userdata:
									case sol::type::lightuserdata:
										ImGui::Text("Userdata: %s", name.c_str());
										break;

									default:
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
					ImGui::Text("Texture: %s", sprite->get_texture_name().c_str());

					if (ImGui::ComboAutoSelect("Select Texture", m_texture_data))
					{
						tasks.push_back([&]() {
							sprite->set_texture(m_texture_data.input);
						});
					}

					math::fRect clip = sprite->get_clip();
					if (ui::imgui_frect("Clip", clip))
					{
						tasks.push_back([&]() {
							sprite->set_clip(clip);
						});
					}

					auto& tint = sprite->m_tint.vec4();
					if (ImGui::ColorEdit4("Colour", &tint.x))
					{
						sprite->m_tint.set_rgba(tint);
					}
				});

				draw_component<components::Tag>(selected, "Tag", [&](components::Tag* tag) {
					ImGui::InputText("##ComponentTagEditor", &tag->m_tag, ImGuiInputTextFlags_AutoSelectAll);
				});

				draw_component<components::Text>(selected, "Text", [&](components::Text* text) {
					ImGui::Text("Font: %s", text->m_text.get_font().c_str());
					if (ImGui::ComboAutoSelect("##FontSelector", m_font_data))
					{
						text->m_text.set_font(m_font_data.input);
					}

					if (!text->m_text.get_font().empty())
					{
						std::string tstr = text->m_text.get_text();
						if (ImGui::InputTextMultiline("Text", &tstr))
						{
							tasks.push_back([&]() {
								text->m_text.update(tstr);
							});
						}

						auto size = text->m_text.get_size();
						if (ImGui::InputFloat("Size", &size, 1.0f, 2.0f, "%.0f"))
						{
							tasks.push_back([&]() {
								text->m_text.update(size);
							});
						}

						ImGui::Text("Alignment: %s", magic_enum::enum_name<graphics::Text::Alignment>(text->m_text.get_alignment()));
						if (ImGui::ComboAutoSelect("##AlignmentSelector", m_alignment_data))
						{
							tasks.push_back([&]() {
								auto align = magic_enum::enum_cast<graphics::Text::Alignment>(m_alignment_data.input).value();
								text->m_text.update(align);
							});
						}

						auto& tint = text->m_text.m_colour.vec4();
						if (ImGui::ColorEdit4("Colour", &tint.x))
						{
							text->m_text.m_colour.set_rgba(tint);
						}
					}
				});

				draw_component<components::Transform>(selected, "Transform", [&](components::Transform* tf) {
					float pos[2] = {tf->m_tf.get_pos().x, tf->m_tf.get_pos().y};
					if (ImGui::InputFloat2("Position", pos, "%.1f", ImGuiInputTextFlags_CharsNoBlank))
					{
						tf->m_tf.set_pos(pos[0], pos[1]);
					}

					float origin[2] = {tf->m_tf.get_origin().x, tf->m_tf.get_origin().y};
					if (ImGui::InputFloat2("Origin", origin, "%.1f", ImGuiInputTextFlags_CharsNoBlank))
					{
						tf->m_tf.set_origin(origin[0], origin[1]);
					}

					auto rotate = tf->m_tf.get_rotation();
					if (ImGui::SliderAngle("Rotate", &rotate, 0.0f, 1080.0f))
					{
						tf->m_tf.set_rotation(rotate);
					}

					float scale[2] = {tf->m_tf.get_scale().x, tf->m_tf.get_scale().y};
					if (ImGui::InputFloat2("Scale", scale, "%.1f", ImGuiInputTextFlags_CharsNoBlank))
					{
						tf->m_tf.set_scale_horizontal(scale[0]);
						tf->m_tf.set_scale_vertical(scale[1]);
					}

					if (ImGui::Button("Reset"))
					{
						tf->m_tf.reset();
					}
				});

				ImGui::PopID();
			}

			ImGui::End();
		}
	}
} // namespace sc
