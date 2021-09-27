///
/// Editor.hpp
/// supercluster
///
/// Refer to LICENSE.txt for more details.
///

#ifndef SUPERCLUSTER_INSTANCES_EDITOR_HPP_
#define SUPERCLUSTER_INSTANCES_EDITOR_HPP_

#include <galaxy/core/Application.hpp>

#include "editor/panels/AudioPanel.hpp"
#include "editor/panels/EntityEditor.hpp"
#include "editor/panels/JSONEditor.hpp"
#include "editor/panels/LuaConsole.hpp"
#include "editor/panels/ScenePanel.hpp"
#include "editor/panels/StdConsole.hpp"

using namespace galaxy;

namespace sc
{
	class Editor final : public core::Layer
	{
	public:
		Editor() noexcept;
		virtual ~Editor() noexcept;

		void events() override;
		void update() override;
		void pre_render() override;
		void render() override;

		void new_project();
		void load_project(std::string_view path);
		void save_project();
		void exit();

	private:
		void viewport();

	private:
		panel::LuaConsole m_lua_console;
		panel::StdConsole m_std_console;
		panel::JSONEditor m_json_panel;
		panel::EntityEditor m_entity_panel;
		panel::AudioPanel m_audio_panel;
		panel::ScenePanel m_scene_panel;

		bool m_paused = false;
		std::string m_current_project_path;

		std::vector<void*> m_processes;
		OpenGLOperationStack m_gl_operations;
		graphics::RenderTexture m_framebuffer;

		glm::vec2 m_clicked_pos;
		glm::vec2 m_cursor_size = {4.0f, 4.0f};
		math::AABB m_cursor_aabb;
		bool m_mouse_picked = false;

		bool m_viewport_focused    = false;
		bool m_viewport_hovered    = false;
		ImVec2 m_viewport_size     = {0.0f, 0.0f};
		ImVec2 m_imgui_mouse_delta = {0.0f, 0.0f};

		/*

		nlohmann::json m_backup;

		bool m_game_mode        = false;

		
		events::MouseMoved m_mousemoved_event;
		events::MouseWheel m_scroll_delta;
		events::KeyDown m_keydown_event;
		events::KeyUp m_keyup_event;
		*/
	};
} // namespace sc

#endif