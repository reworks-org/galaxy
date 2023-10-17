///
/// Editor.hpp
/// supercluster
///
/// Refer to LICENSE.txt for more details.
///

#ifndef SUPERCLUSTER_LAYERS_EDITOR_HPP_
#define SUPERCLUSTER_LAYERS_EDITOR_HPP_

#include <zip.h>

#include <galaxy/async/Timer.hpp>
#include <galaxy/graphics/Renderer.hpp>
#include <galaxy/graphics/Renderable.hpp>
#include <galaxy/graphics/RenderTexture.hpp>
#include <galaxy/platform/Subprocess.hpp>
#include <galaxy/ui/ImGuiHelpers.hpp>
#include <galaxy/ui/NuklearUI.hpp>

#include "editor/panels/AssetPanel.hpp"
#include "editor/panels/EntityEditor.hpp"
#include "editor/panels/JSONEditor.hpp"
#include "editor/panels/LuaConsole.hpp"
#include "editor/panels/ScenePanel.hpp"
#include "editor/panels/LogConsole.hpp"
#include "editor/panels/noise/NoisePanel.hpp"

#include "editor/Settings.hpp"

using namespace galaxy;

namespace sc
{
	class Editor final : public scene::Scene
	{
	  public:
		Editor(const std::string& name);
		virtual ~Editor();

		void load() override;
		void unload() override;
		void update() override;
		void render() override;

		void new_project();
		void load_project(std::string_view path);
		void save_project(bool save_as = false);
		void export_project();
		void restart();
		void exit();

	  private:
		void draw_editor();
		void viewport();

		void recursively_zip_assets(struct zip_t* zip, const std::filesystem::path& path);
		void do_updates();

	  private:
		async::Timer         m_autosave;
		platform::Subprocess m_tiled_process;
		platform::Subprocess m_bfxr_process;

		panel::LuaConsole   m_lua_console;
		panel::LogConsole   m_log_console;
		panel::JSONEditor   m_json_panel;
		panel::EntityEditor m_entity_panel;
		panel::ScenePanel   m_scene_panel;
		panel::AssetPanel   m_asset_panel;
		panel::NoisePanel   m_noise_graph;

		Settings    m_settings;
		UpdateStack m_update_stack;

		bool m_show_scenes         = true;
		bool m_show_entities       = true;
		bool m_show_viewport       = true;
		bool m_show_assetpanel     = true;
		bool m_show_logpanel       = true;
		bool m_show_luapanel       = true;
		bool m_show_jsonpanel      = true;
		bool m_show_codeeditor     = true;
		bool m_show_settings       = false;
		bool m_show_exportprogress = false;
		bool m_show_about          = false;
		bool m_show_noisegraph     = false;

		bool m_about_control      = true;
		bool m_stopped            = true;
		bool m_viewport_focused   = false;
		bool m_viewport_hovered   = false;
		bool m_game_mode          = false;
		bool m_editor_cam_enabled = true;
		bool m_use_hand           = false;

		ImVec2 m_icon_size       = {24, 24};
		ImVec2 m_icon_size_large = {32, 32};
		ImVec2 m_padding         = {4, 4};
		ImVec2 m_no_padding      = {0, 0};

		graphics::RenderTexture m_framebuffer;

		ImVec2                   m_viewport_size = {0.0f, 0.0f};
		std::array<glm::vec2, 2> m_viewport_bounds;
		ImVec2                   m_imgui_mouse_delta = {0.0f, 0.0f};

		graphics::Texture m_camera_btn;
		graphics::Texture m_editor_cam_btn;
		graphics::Camera  m_editor_camera;

		std::string m_current_project_path;

		nlohmann::json      m_backup;
		scene::SceneManager m_project_sm;

		unsigned int m_mousepick_buffer;
		Selected     m_selected_entity;

		ui::NuklearUI*      m_nui;
		core::Window*       m_window;
		graphics::Renderer* m_renderer;
	};
} // namespace sc

#endif
