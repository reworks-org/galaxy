///
/// DevTools.hpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#ifndef GALAXY_DEVTOOLS_HPP_
#define GALAXY_DEVTOOLS_HPP_

#include <imgui/addons/TextEditor.h>
#include <qs/core/Window.hpp>

#include "galaxy/core/World.hpp"
#include "galaxy/scripting/LuaConsole.hpp"
#include "galaxy/tools/JsonEditor.hpp"

///
/// Core namespace.
///
namespace galaxy
{
	///
	/// Tools to aid in game development using galaxy engine.
	///
	class DevTools final
	{
	public:
		DevTools();
		~DevTools();

		void create();
		void prepare();
		void draw();

	private:
		void start();
		void end();

		void state_manager_ui();
		void json_ui();
		void script_editor_ui();
		void atlas_editor_ui();
		void entity_ui();
		void component_ui(bool enabled, std::uint32_t entity);
		void gui_builder_ui();

		galaxy::World* m_world;
		qs::Window* m_window;

		bool m_draw_main;
		bool m_draw_demo;
		bool m_draw_state_editor;
		bool m_draw_json_editor;
		bool m_draw_script_editor;
		bool m_draw_atlas_editor;
		bool m_draw_entity_editor;
		bool m_draw_lua_console;
		bool m_draw_gui_builder_ui;

		ImGui::TextEditor m_editor;
		galaxy::LuaConsole m_console;
		JsonEditor m_json_editor;

		short m_atlas_state;

		bool m_show_entity_create;
		std::string m_edn_buffer;
		std::string m_entity_debug_name;
		std::uint32_t m_active_entity;

		float m_batch_region[4]   = {0.0f, 0.0f, 0.0f, 0.0f};
		bool m_sfx_loop           = false;
		std::string m_active_anim = "";
		bool m_add_anim_popup     = false;
		robin_hood::unordered_map<qs::Sprite*, std::string> m_sprites_to_create;
	};
} // namespace galaxy

#endif