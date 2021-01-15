///
/// CoreLayer.hpp
/// supercluster
///
/// Refer to LICENSE.txt for more details.
///

#ifndef SUPERCLUSTER_CORELAYER_HPP_
#define SUPERCLUSTER_CORELAYER_HPP_

#include <galaxy/core/World.hpp>
#include <galaxy/core/Layer.hpp>
#include <galaxy/graphics/Camera.hpp>
#include <galaxy/graphics/post/Simple.hpp>
#include <galaxy/graphics/Renderer.hpp>
#include <galaxy/graphics/Window.hpp>
#include <galaxy/scripting/LuaConsole.hpp>
#include <imgui/addons/TextEditor.h>

#include "../editor/JSON.hpp"

using namespace galaxy;

namespace sc
{
	class CoreLayer final : public core::Layer
	{
	public:
		CoreLayer();
		virtual ~CoreLayer();

		void events() override;
		void update(const double dt) override;
		void pre_render() override;
		void render() override;

	private:
		void start();
		void end();

		void state_manager_ui();
		void json_ui();
		void script_editor_ui();
		void atlas_editor_ui();
		void entity_ui();
		void component_ui(bool enabled, std::uint64_t entity);
		void gui_builder_ui();

		core::World* m_world;
		graphics::Window* m_window;
		graphics::Renderer* m_renderer;
		graphics::Camera m_camera;

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
		lua::Console m_console;
		editor::JSON m_json_editor;

		short m_atlas_state;
		bool m_visible_tools;

		posteffect::Simple m_default_post;

		bool m_show_entity_create;
		std::string m_edn_buffer;
		std::string m_entity_debug_name;
		std::uint64_t m_active_entity;

		float m_batch_region[4]   = {0.0f, 0.0f, 0.0f, 0.0f};
		bool m_sfx_loop           = false;
		std::string m_active_anim = "";
		std::string m_tiled_path;
		bool m_add_anim_popup = false;
		robin_hood::unordered_map<graphics::Sprite*, std::string> m_sprites_to_create;
	};
} // namespace sc

#endif