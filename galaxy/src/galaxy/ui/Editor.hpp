///
/// Editor.hpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#ifndef GALAXY_UI_EDITOR_HPP_
#define GALAXY_UI_EDITOR_HPP_

#include <SDL3/SDL_events.h>

#include "galaxy/ui/panels/ConfigPanel.hpp"
#include "galaxy/ui/panels/LogPanel.hpp"
#include "galaxy/ui/panels/ScenePanel.hpp"

namespace galaxy
{
	///
	/// Ingame debug editor UI.
	///
	class Editor final : public Panel
	{
	public:
		///
		/// Constructor.
		///
		Editor();

		///
		/// Destructor.
		///
		~Editor();

		///
		/// Process events in the editor.
		///
		/// \param events Event structure.
		///
		void handle_events(const SDL_Event& events) noexcept;

		///
		/// Render the editor to the screen.
		///
		void render();

		///
		/// Draw the ImGui widgets to ImGui buffers.
		///
		void draw() override;

	private:
		///
		/// Start rendering the editor dock.
		///
		void begin_dock();

		///
		/// Finish rendering the editor dock.
		///
		void end_dock();

		///
		/// Draw the editor menubar.
		///
		void draw_menubar();

		///
		/// Handle ImGui widget shortcuts.
		///
		void shortcuts();

	private:
		///
		/// Config panel for manipulating engine settings.
		///
		ConfigPanel m_config_panel;

		///
		/// Scene panel for managing scenes.
		///
		ScenePanel m_scene_panel;

		///
		/// Log console panel.
		///
		LogPanel m_log_panel;

		///
		/// Hotkey info.
		///
		Popup m_hotkeys;
	};
} // namespace galaxy

/*
public:
using Tasks = meta::vector<std::move_only_function<void(void)>>;

private:
void exit();

void new_project();

void load_project(const std::string& path);
void save_project(bool save_as = false);
void export_project();


void draw_game_controls();
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
nlohmann::json m_backup;

EditorState         m_state;

// tools.
platform::Subprocess m_ldtk;
platform::Subprocess m_bfxr;

// popup controls.
bool m_show_new    = false;
bool m_show_exit   = false;
bool m_show_about  = false;
bool m_show_export = false;

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
LuaConsole    m_lua_console;
Viewport      m_viewport;
CodeEditor    m_code_editor;
AssetPanel    m_asset_panel;
ScenePanel    m_scene_panel;
EntityPanel   m_entity_panel;*/

#endif
