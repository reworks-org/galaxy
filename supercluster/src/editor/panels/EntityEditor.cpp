///
/// EntityEditor.cpp
/// supercluster
///
/// Refer to LICENSE.txt for more details.
///

#include <entt/entity/registry.hpp>
#include <magic_enum.hpp>
#include <imgui_stdlib.h>

#include <galaxy/components/Animated.hpp>
#include <galaxy/components/DrawShader.hpp>
#include <galaxy/components/Flag.hpp>
#include <galaxy/components/Primitive.hpp>
#include <galaxy/components/Script.hpp>
#include <galaxy/components/Sprite.hpp>
#include <galaxy/components/Tag.hpp>
#include <galaxy/components/Text.hpp>
#include <galaxy/components/Transform.hpp>
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
					});

					draw_component<components::Flag>(selected, "Flag", [](components::Flag* flag) {
					});

					draw_component<components::Primitive>(selected, "Primitive", [](components::Primitive* prim) {
					});

					draw_component<components::Script>(selected, "Script", [](components::Script* script) {
					});

					draw_component<components::Sprite>(selected, "Sprite", [](components::Sprite* sprite) {
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