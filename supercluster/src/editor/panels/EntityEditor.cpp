///
/// EntityEditor.cpp
/// supercluster
///
/// Refer to LICENSE.txt for more details.
///

#include <entt/entity/registry.hpp>
#include <magic_enum.hpp>
#include <imgui_stdlib.h>
#include <imgui_addons/ToggleButton.h>

#include <galaxy/core/ServiceLocator.hpp>
#include <galaxy/core/Window.hpp>
#include <galaxy/components/Animated.hpp>
#include <galaxy/components/DrawShader.hpp>
#include <galaxy/components/Flag.hpp>
#include <galaxy/components/Primitive.hpp>
#include <galaxy/components/Script.hpp>
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
		EntityEditor::EntityEditor() noexcept
		{
		}

		EntityEditor::~EntityEditor() noexcept
		{
		}

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
									ui::imgui_open_alert("ShaderNotExisting");
								}
							}
						}
					});

					ui::imgui_alert("ShaderNotExisting", "Shader does not exist.");

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
									ui::imgui_open_alert("EnabledNotValidWarning");
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

					ui::imgui_alert("EnabledNotValidWarning", "Entity did not pass validation.");

					draw_component<components::Primitive>(selected, "Primitive", [](components::Primitive* prim) {
					});

					draw_component<components::Script>(selected, "Script", [](components::Script* script) {
						if (ImGui::Button("Copy Path to Clipboard"))
						{
							core::ServiceLocator<core::Window>::ref().get_input<input::Clipboard>().set(script->file().c_str());
						}

						ImGui::SameLine();

						if (ImGui::Button("Load"))
						{
							auto file = core::ServiceLocator<fs::VirtualFileSystem>::ref().show_open_dialog("*.lua");
							if (file.has_value())
							{
								script->load(file.value());
							}
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
									sprite->create(s_buff, s_opacity, s_layer);
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
					});

					draw_component<components::Transform>(selected, "Transform", [](components::Transform* tf) {
					});
				}
			}

			ImGui::End();
		}
	} // namespace panel
} // namespace sc

/*
if (ImGui::IsMouseDown(0) && ImGui::IsWindowHovered())
{
	selected.m_selected = entt::null;
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
				ImGui::InputText("ID", &s_id, ImGuiInputTextFlags_AutoSelectAll | ImGuiInputTextFlags_CharsNoBlank | ImGuiInputTextFlags_EnterReturnsTrue);
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
*/