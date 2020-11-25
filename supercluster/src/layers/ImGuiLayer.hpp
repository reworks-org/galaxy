///
/// ImGuiLayer.hpp
/// supercluster
///
/// Refer to LICENSE.txt for more details.
///

#ifndef SUPERCLUSTER_IMGUILAYER_HPP_
#define SUPERCLUSTER_IMGUILAYER_HPP_

#include <qs/core/Window.hpp>
#include <galaxy/core/World.hpp>
#include <galaxy/layer/Layer.hpp>
#include <galaxy/scripting/LuaConsole.hpp>
#include <qs/graphics/Camera.hpp>

#include <imgui/addons/TextEditor.h>

#include "widgets/JsonEditor.hpp"

namespace sc
{
	class ImGuiLayer final : public galaxy::Layer
	{
	public:
		ImGuiLayer();
		virtual ~ImGuiLayer();

		void events() override;
		void update(const double dt) override;
		void render() override;

		void start();
		void end();

		void entity_ui();
		void script_editor_ui();

		void json_ui();

	private:
		qs::Camera m_camera;
		galaxy::World* m_world;
		qs::Window* m_window;

		bool m_show_script_editor;
		bool m_show_entity_editor;
		bool m_draw_console;
		bool m_draw_json_editor;

		ImGui::TextEditor m_editor;
		galaxy::LuaConsole m_console;
		JsonEditor m_json_editor;
	};
} // namespace sc

#endif