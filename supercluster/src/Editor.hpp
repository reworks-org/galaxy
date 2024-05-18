///
/// Editor.hpp
/// supercluster
///
/// Refer to LICENSE.txt for more details.
///

#ifndef SUPERCLUSTER_EDITOR_HPP_
#define SUPERCLUSTER_EDITOR_HPP_

#include <nlohmann/json.hpp>
#include <zip.h>

#include <galaxy/async/Timer.hpp>
#include <galaxy/platform/Subprocess.hpp>
#include <galaxy/scene/SceneManager.hpp>

#include "panels/AssetPanel.hpp"
#include "panels/CodeEditor.hpp"
#include "panels/EntityPanel.hpp"
#include "panels/LogConsole.hpp"
#include "panels/LuaConsole.hpp"
#include "panels/ScenePanel.hpp"
#include "panels/SettingsPanel.hpp"
#include "panels/Viewport.hpp"

#include "Selected.hpp"

using namespace galaxy;

namespace sc
{
	class Editor final : public scene::Scene
	{
	  public:
		using Tasks = meta::vector<std::move_only_function<void(void)>>;

		Editor(const std::string& name);
		virtual ~Editor();

	  private:
		void load() override;
		void unload() override;
		void update() override;
		void render() override;

		void exit();

		void new_project();

		void load_project(const std::string& path);
		void save_project(bool save_as = false);
		void export_project();

		void draw_editor();
		void begin_dock();
		void end_dock();
		void draw_menubar();
		void draw_game_controls();
		void short_cuts();
		void show_loadproject();
		void render_exporter() const;

		void recursively_zip_assets(struct zip_t* zip, const std::filesystem::path& path);

		void set_state_game();
		void set_state_editor();
		void set_state_game_and_editor();

		void set_input_game();
		void set_input_editor();

	  private:
		// game data independant from editor.
		nlohmann::json      m_backup;
		scene::SceneManager m_project;
		EditorState         m_state;

		// tools.
		platform::Subprocess m_ldtk;
		platform::Subprocess m_bfxr;

		// popup controls.
		bool m_show_new     = false;
		bool m_show_exit    = false;
		bool m_show_restart = false;
		bool m_show_about   = false;
		bool m_show_export  = false;

		// random editor data.
		ImVec2                m_padding;
		Tasks                 m_tasks;
		std::filesystem::path m_current_project_path;
		async::Timer<false>   m_autosave;
		graphics::Camera      m_editor_camera;
		bool                  m_use_hand_cursor   = false;
		ImVec2                m_imgui_mouse_delta = {0.0f, 0.0f};
		Selected              m_selected;

		// panels
		LogConsole    m_log_console;
		LuaConsole    m_lua_console;
		Viewport      m_viewport;
		CodeEditor    m_code_editor;
		AssetPanel    m_asset_panel;
		SettingsPanel m_settings_panel;
		ScenePanel    m_scene_panel;
		EntityPanel   m_entity_panel;
	};
} // namespace sc

#endif
