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
#include "editor/panels/ScriptEditor.hpp"
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
		panel::ScriptEditor m_script_editor;
		panel::JSONEditor m_json_panel;
		panel::EntityEditor m_entity_panel;
		panel::AudioPanel m_audio_panel;
		panel::ScenePanel m_scene_panel;

		std::string m_current_project_path;

		bool m_paused;

		std::vector<void*> m_processes;
		OpenGLOperationStack m_gl_operations;

		graphics::RenderTexture m_framebuffer;
		ImVec2 m_viewport_size = {0.0f, 0.0f};

		/*

		nlohmann::json m_backup;

		bool m_game_mode        = false;
		bool m_viewport_focused = false;
		bool m_viewport_hovered = false;
		bool m_mouse_picked     = false;

		
		ImVec2 m_imgui_delta;
		events::MouseMoved m_mousemoved_event;
		events::MouseWheel m_scroll_delta;
		events::KeyDown m_keydown_event;
		events::KeyUp m_keyup_event;
		*/
	};
} // namespace sc

#endif