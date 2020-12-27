///
/// DevTools.hpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#ifndef GALAXY_TOOLS_DEVTOOLS_HPP_
#define GALAXY_TOOLS_DEVTOOLS_HPP_

#include <imgui/addons/TextEditor.h>

#include "galaxy/core/World.hpp"
#include "galaxy/graphics/Window.hpp"
#include "galaxy/scripting/LuaConsole.hpp"
#include "galaxy/tools/JsonEditor.hpp"

namespace galaxy
{
	namespace tools
	{
		///
		/// Tools to aid in game development using galaxy engine.
		///
		class DevTools final
		{
		public:
			///
			/// Constructor.
			///
			DevTools();

			///
			/// Destructor.
			///
			~DevTools();

			///
			/// Create devtools ui.
			///
			void create();

			///
			/// Prepares imgui rendering data for drawing.
			///
			void prepare();

			///
			/// Draws imgui data to screen.
			///
			void draw();

		private:
			///
			/// Called before all imgui functions.
			///
			void start();

			///
			/// Called when finished with imgui.
			///
			void end();

			///
			/// Internal function to render state manager widgets.
			///
			void state_manager_ui();

			///
			/// Internal function to render json widgets.
			///
			void json_ui();

			///
			/// Internal function to render script editor widgets.
			///
			void script_editor_ui();

			///
			/// Internal function to render atlas editor widgets.
			///
			void atlas_editor_ui();

			///
			/// Internal function to render entity manager widgets.
			///
			void entity_ui();

			///
			/// Internal function to render component manager widgets.
			///
			/// \param enabled Is this entity currently enabled.
			/// \param entity Entity whos components are to be displayed with imgui.
			///
			void component_ui(bool enabled, std::uint64_t entity);

			///
			/// Internal function to render gui building widgets.
			///
			void gui_builder_ui();

			///
			/// Pointer to game world.
			///
			core::World* m_world;

			///
			/// Pointer to applicadtion window.
			///
			graphics::Window* m_window;

			///
			/// ImGui draw window flag. Controls all widgets.
			///
			bool m_draw_main;

			///
			/// ImGui draw window flag. Controls demo window.
			///
			bool m_draw_demo;

			///
			/// ImGui draw window flag. State editor.
			///
			bool m_draw_state_editor;

			///
			/// ImGui draw window flag. Json widgets.
			///
			bool m_draw_json_editor;

			///
			/// ImGui draw window flag. Script editor.
			///
			bool m_draw_script_editor;

			///
			/// ImGui draw window flag. Atlas editor.
			///
			bool m_draw_atlas_editor;

			///
			/// ImGui draw window flag. Entity editor.
			///
			bool m_draw_entity_editor;

			///
			/// ImGui draw window flag. Lua console.
			///
			bool m_draw_lua_console;

			///
			/// ImGui draw window flag. Gui builder.
			///
			bool m_draw_gui_builder_ui;

			///
			/// Text editor widget.
			///
			ImGui::TextEditor m_editor;

			///
			/// Lua console widget.
			///
			lua::Console m_console;

			///
			/// Json widget.
			///
			JsonEditor m_json_editor;

			///
			/// Keeps track of atlas build state.
			///
			short m_atlas_state;

			///
			/// Flag to show entity creation widgets.
			///
			bool m_show_entity_create;

			///
			/// Entity debug name buffer.
			///
			std::string m_edn_buffer;

			///
			/// Current entity debug name.
			///
			std::string m_entity_debug_name;

			///
			/// Currently active entity.
			///
			std::uint64_t m_active_entity;

			///
			/// Spritebatch current region.
			///
			float m_batch_region[4] = {0.0f, 0.0f, 0.0f, 0.0f};

			///
			/// Flag for controlling sound effect looping.
			///
			bool m_sfx_loop = false;

			///
			/// Currently active animation.
			///
			std::string m_active_anim = "";

			///
			/// Flag to display adding animation popups.
			///
			bool m_add_anim_popup = false;

			///
			/// Keeps track of sprites that need to be created outside of start(), end() and draw().
			///
			robin_hood::unordered_map<graphics::Sprite*, std::string> m_sprites_to_create;
		};
	} // namespace tools
} // namespace galaxy

#endif