///
/// EditorLayer.hpp
/// supercluster
///
/// Refer to LICENSE.txt for more details.
///

#ifndef SUPERCLUSTER_LAYERS_EDITORLAYER_HPP_
#define SUPERCLUSTER_LAYERS_EDITORLAYER_HPP_

#include <galaxy/core/Layer.hpp>
#include <galaxy/core/Window.hpp>

#include "editor/panels/EntityEditor.hpp"
#include "editor/panels/JSONEditor.hpp"
#include "editor/panels/LuaConsole.hpp"
#include "editor/panels/ScriptEditor.hpp"

#include "scenes/EditorScene.hpp"

using namespace galaxy;

namespace sc
{
	class EditorLayer final : public core::Layer
	{
	public:
		EditorLayer();
		virtual ~EditorLayer();

		void on_push() override;
		void on_pop() override;

		void events() override;
		void update(const double dt) override;
		void pre_render() override;
		void render() override;

	private:
		void start();
		void end();

	private:
		galaxy::core::Window* m_window;
		std::unique_ptr<EditorScene> m_editor_scene;

		void* m_process;

		bool m_draw_demo;

		//panel::EntityEditor m_entity_panel;
		panel::JSONEditor m_json_panel;
		panel::LuaConsole m_console;
		panel::ScriptEditor m_script_panel;
	};
} // namespace sc

#endif