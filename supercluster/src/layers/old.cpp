///
/// Editor.cpp
/// supercluster
///
/// Refer to LICENSE.txt for more details.
///

#include <iostream>

#include <portable-file-dialogs.h>

#include <galaxy/components/Renderable.hpp>
#include <galaxy/core/ServiceLocator.hpp>
#include <galaxy/core/Window.hpp>
#include <galaxy/graphics/Renderer2D.hpp>
#include <galaxy/math/ZLib.hpp>
#include <galaxy/platform/Platform.hpp>
#include <galaxy/scripting/JSONUtils.hpp>
#include <galaxy/ui/ImGuiHelpers.hpp>
#include <galaxy/ui/ImGuiTheme.hpp>

#include "Editor.hpp"

namespace sc
{
	void Editor::events()
	{
		/*
		if (!m_game_mode)
		{
			//	ImGui::ImplGlfw::g_BlockInput = false;

			if (m_viewport_focused && m_viewport_hovered)
			{




				m_scroll_delta.m_x_offset = 0.0;
				m_scroll_delta.m_y_offset = SL_HANDLE.window()->get_scroll_delta();
				if (m_scroll_delta.m_y_offset != 0.0)
				{
					//camera.on_event(m_scroll_delta);
				}



				if (SL_HANDLE.window()->key_down(input::Keys::W))
				{
					m_keydown_event.m_keycode = input::Keys::W;
					//camera.on_event(m_keydown_event);
				}
				else
				{
					m_keyup_event.m_keycode = input::Keys::W;
					//camera.on_event(m_keyup_event);
				}

				if (SL_HANDLE.window()->key_down(input::Keys::S))
				{
					m_keydown_event.m_keycode = input::Keys::S;
					//camera.on_event(m_keydown_event);
				}
				else
				{
					m_keyup_event.m_keycode = input::Keys::S;
					//camera.on_event(m_keyup_event);
				}

				if (SL_HANDLE.window()->key_down(input::Keys::A))
				{
					m_keydown_event.m_keycode = input::Keys::A;
					//camera.on_event(m_keydown_event);
				}
				else
				{
					m_keyup_event.m_keycode = input::Keys::A;
					//camera.on_event(m_keyup_event);
				}

				if (SL_HANDLE.window()->key_down(input::Keys::D))
				{
					m_keydown_event.m_keycode = input::Keys::D;
					//camera.on_event(m_keydown_event);
				}
				else
				{
					m_keyup_event.m_keycode = input::Keys::D;
					//camera.on_event(m_keyup_event);
				}

				if (SL_HANDLE.window()->key_down(input::Keys::Q))
				{
					m_keydown_event.m_keycode = input::Keys::Q;
					//camera.on_event(m_keydown_event);
				}
				else
				{
					m_keyup_event.m_keycode = input::Keys::Q;
					//camera.on_event(m_keyup_event);
				}

				if (SL_HANDLE.window()->key_down(input::Keys::E))
				{
					m_keydown_event.m_keycode = input::Keys::E;
					//camera.on_event(m_keydown_event);
				}
				else
				{
					m_keyup_event.m_keycode = input::Keys::E;
					//camera.on_event(m_keyup_event);
				}
			}
		}
		else
		{
			//ImGui::ImplGlfw::g_BlockInput = true;
			m_scene_stack.top()->on_push();

			if (SL_HANDLE.window()->key_pressed(input::Keys::ESC))
			{
				m_game_mode = false;
				deserialize(m_backup);
				SL_HANDLE.window()->set_cursor_visibility(true);
			}

			m_scene_stack.events();
		}
		*/
	}
} // namespace sc

/*
if (ImGui::BeginPopupContextItem("RightClickCreateEntityPopup"))
{
if (ImGui::BeginMenu("  Create"))
{
if (ImGui::BeginMenu("  Entity"))
{
if (m_topscene_type == "2D")
{
if (ImGui::MenuItem("  Sprite"))
{
auto& world       = m_scene_stack.top()->m_world;
const auto entity = world.create();

world.create_component<components::BatchSprite>(entity);
world.create_component<components::Tag>(entity);
world.create_component<components::Transform2D>(entity);
auto* r   = world.create_component<components::Renderable>(entity);
r->m_type = graphics::Renderables::BATCHED;
}

if (ImGui::MenuItem("  Animated Sprite"))
{
auto& world       = m_scene_stack.top()->m_world;
const auto entity = world.create();

world.create_component<components::Animated>(entity);
world.create_component<components::BatchSprite>(entity);
world.create_component<components::Tag>(entity);
world.create_component<components::Transform2D>(entity);
auto* r   = world.create_component<components::Renderable>(entity);
r->m_type = graphics::Renderables::BATCHED;
}

if (ImGui::MenuItem("  Primitive2D"))
{
auto& world       = m_scene_stack.top()->m_world;
const auto entity = world.create();

world.create_component<components::Primitive2D>(entity);
world.create_component<components::Tag>(entity);
world.create_component<components::Transform2D>(entity);
auto* r   = world.create_component<components::Renderable>(entity);
r->m_type = graphics::Renderables::LINE_LOOP;
}

if (ImGui::MenuItem("  Rigid Body"))
{
auto& world       = m_scene_stack.top()->m_world;
const auto entity = world.create();

world.create_component<components::BatchSprite>(entity);
world.create_component<components::OnCollision>(entity);
world.create_component<components::RigidBody>(entity);
world.create_component<components::Tag>(entity);
world.create_component<components::Transform2D>(entity);
auto* r   = world.create_component<components::Renderable>(entity);
r->m_type = graphics::Renderables::BATCHED;
}

if (ImGui::MenuItem("  Animated Body"))
{
auto& world       = m_scene_stack.top()->m_world;
const auto entity = world.create();

world.create_component<components::Animated>(entity);
world.create_component<components::BatchSprite>(entity);
world.create_component<components::OnCollision>(entity);
world.create_component<components::RigidBody>(entity);
world.create_component<components::Tag>(entity);
world.create_component<components::Transform2D>(entity);
auto* r   = world.create_component<components::Renderable>(entity);
r->m_type = graphics::Renderables::BATCHED;
}

if (ImGui::MenuItem("  Text"))
{
auto& world       = m_scene_stack.top()->m_world;
const auto entity = world.create();

world.create_component<components::Tag>(entity);
world.create_component<components::Text>(entity);
world.create_component<components::Transform2D>(entity);
auto* r   = world.create_component<components::Renderable>(entity);
r->m_type = graphics::Renderables::TEXT;
}
}

ImGui::EndMenu();
}

ImGui::EndMenu();
}

ImGui::EndPopup();
}*/