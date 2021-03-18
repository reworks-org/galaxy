///
/// Editor.hpp
/// supercluster
///
/// Refer to LICENSE.txt for more details.
///

#ifndef SUPERCLUSTER_INSTANCES_EDITOR_HPP_
#define SUPERCLUSTER_INSTANCES_EDITOR_HPP_

#include <galaxy/core/Instance.hpp>
#include <galaxy/events/WindowResized.hpp>
#include <galaxy/graphics/texture/RenderTexture.hpp>

#include "editor/panels/AudioPanel.hpp"
#include "editor/panels/EntityEditor.hpp"
#include "editor/panels/JSONEditor.hpp"
#include "editor/panels/LuaConsole.hpp"
#include "editor/panels/ScenePanel.hpp"
#include "editor/panels/ScriptEditor.hpp"
#include "editor/panels/StdConsole.hpp"

#include "../Project.hpp"

using namespace galaxy;

namespace sc
{
	class Editor final : public core::Instance
	{
	public:
		Editor();
		virtual ~Editor();

		void events() override;
		void update(const double dt) override;
		void pre_render() override;
		void render() override;

		void on_event(const events::WindowResized& event);

	private:
		void imgui_render();
		void start();
		void end();
		void exit();

		void viewport();

		void save_project();
		void new_project(std::string_view path);

		void first_start();

	private:
		galaxy::core::Window* m_window = nullptr;

		void* m_process = nullptr;

		bool m_first_start      = true;
		bool m_render_demo      = false;
		bool m_viewport_focused = false;
		bool m_viewport_hovered = false;
		bool m_mouse_dragging   = false;
		bool m_mouse_picked     = false;

		panel::EntityEditor m_entity_panel;
		panel::JSONEditor m_json_panel;
		panel::LuaConsole m_console;
		panel::ScenePanel m_scene_panel;
		panel::ScriptEditor m_script_panel;
		panel::StdConsole m_std_console;
		panel::AudioPanel m_audio_panel;

		graphics::RenderTexture m_framebuffer;
		ImVec2 m_viewport_size = {0.0f, 0.0f};
		OpenGLOperationStack m_gl_operations;

		std::unique_ptr<Project> m_project = nullptr;
	};

	[[nodiscard]] inline const bool operator==(const ImVec2& a, const ImVec2& b)
	{
		return ((a.x == b.x) && (a.y == b.y));
	}

	[[nodiscard]] inline const bool operator!=(const ImVec2& a, const ImVec2& b)
	{
		return !(a == b);
	}
} // namespace sc

#endif