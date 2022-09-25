///
/// Editor.hpp
/// supercluster
///
/// Refer to LICENSE.txt for more details.
///

#ifndef SUPERCLUSTER_LAYERS_EDITOR_HPP_
#define SUPERCLUSTER_LAYERS_EDITOR_HPP_

#include <galaxy/async/Timer.hpp>
#include <galaxy/platform/Subprocess.hpp>
#include <galaxy/state/Layer.hpp>
#include <galaxy/ui/ImGuiHelpers.hpp>

#include "editor/panels/AssetPanel.hpp"
#include "editor/panels/EntityEditor.hpp"
#include "editor/panels/JSONEditor.hpp"
#include "editor/panels/LuaConsole.hpp"
#include "editor/panels/ScenePanel.hpp"
#include "editor/panels/LogConsole.hpp"

#include "editor/Settings.hpp"

using namespace galaxy;

namespace sc
{
	class Editor final : public state::Layer
	{
	public:
		Editor(std::string_view name, state::Scene* scene) noexcept;
		virtual ~Editor() noexcept;

		void on_push() override;
		void on_pop() override;

		void events() override;
		void update() override;
		void render() override;

		void new_project();
		void load_project(std::string_view path);
		void save_project(bool save_as = false);
		void restart();
		void exit();

	private:
		Editor() = delete;

		void viewport();

		[[nodiscard]] nlohmann::json serialize() override;
		void deserialize(const nlohmann::json& json) override;

	private:
		/*
		platform::Subprocess m_tiled_process;
		platform::Subprocess m_notepad_process;

		glm::vec2 m_clicked_pos;
		glm::vec2 m_cursor_size = {4.0f, 4.0f};
		math::AABB m_cursor_aabb;

		nlohmann::json m_backup;

		bool m_game_mode        = false;

		events::MouseMoved m_mousemoved_event;
		events::MouseWheel m_scroll_delta;
		events::KeyDown m_keydown_event;
		events::KeyUp m_keyup_event;
		*/

		Settings m_settings;

		panel::LuaConsole m_lua_console;
		panel::LogConsole m_log_console;
		panel::JSONEditor m_json_panel;
		panel::EntityEditor m_entity_panel;
		panel::ScenePanel m_scene_panel;
		panel::AssetPanel m_asset_panel;

		UpdateStack m_update_stack;

		bool m_paused           = false;
		bool m_viewport_focused = false;
		bool m_viewport_hovered = false;
		bool m_mouse_picked     = false;
		bool m_use_mouse_hand   = false;
		bool m_show_settings    = false;

		ImVec2 m_imgui_mouse_delta = {0.0f, 0.0f};
		ImVec2 m_viewport_size     = {0.0f, 0.0f};
		ImVec2 m_icon_size         = {20, 20};

		Selected m_selected_entity;

		std::string m_current_project_path;
		state::SceneManager m_project_scenes;
		graphics::RenderTexture m_framebuffer;

		graphics::Texture m_resume_play;
		graphics::Texture m_play;
		graphics::Texture m_stop;
		graphics::Texture m_help;
		graphics::Texture m_cog;

		async::Timer m_autosave;
	};
} // namespace sc

#endif