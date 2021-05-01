///
/// Editor.hpp
/// supercluster
///
/// Refer to LICENSE.txt for more details.
///

#ifndef SUPERCLUSTER_INSTANCES_EDITOR_HPP_
#define SUPERCLUSTER_INSTANCES_EDITOR_HPP_

#include <galaxy/core/Instance.hpp>
#include <galaxy/graphics/texture/Texture.hpp>

#include "editor/panels/AudioPanel.hpp"
#include "editor/panels/EntityEditor.hpp"
#include "editor/panels/JSONEditor.hpp"
#include "editor/panels/LuaConsole.hpp"
#include "editor/panels/ScenePanel.hpp"
#include "editor/panels/ScriptEditor.hpp"
#include "editor/panels/StdConsole.hpp"
#include "editor/panels/GUIPanel.hpp"

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

		void new_project();
		void load(std::string_view path);
		void save();
		void exit();

	private:
		void imgui_render();
		void viewport();

	private:
		std::vector<void*> m_processes;

		std::string m_path          = "";
		std::string m_topscene_type = "3D";
		nlohmann::json m_backup;

		bool m_game_mode        = false;
		bool m_render_demo      = false;
		bool m_viewport_focused = false;
		bool m_viewport_hovered = false;
		bool m_mouse_picked     = false;
		bool m_paused           = false;

		panel::EntityEditor m_entity_panel;
		panel::JSONEditor m_json_panel;
		panel::LuaConsole m_console;
		panel::ScenePanel m_scene_panel;
		panel::ScriptEditor m_script_panel;
		panel::StdConsole m_std_console;
		panel::AudioPanel m_audio_panel;
		panel::GUIPanel m_gui_panel;

		graphics::Texture m_checkerboard;
		graphics::RenderTexture m_framebuffer;
		ImVec2 m_viewport_size = {0.0f, 0.0f};
		OpenGLOperationStack m_gl_operations;

		ImVec2 m_imgui_delta;
		events::MouseMoved m_mousemoved_event;
		events::MouseWheel m_scroll_delta;
		events::KeyDown m_keydown_event;
		events::KeyUp m_keyup_event;
	};

	[[nodiscard]] inline const bool operator==(const ImVec2& a, const ImVec2& b) noexcept
	{
		return ((a.x == b.x) && (a.y == b.y));
	}

	[[nodiscard]] inline const bool operator!=(const ImVec2& a, const ImVec2& b) noexcept
	{
		return !(a == b);
	}
} // namespace sc

#endif