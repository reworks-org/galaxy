///
/// Editor.cpp
/// supercluster
///
/// Refer to LICENSE.txt for more details.
///

#include <qs/core/Window.hpp>
#include <qs/core/Renderer.hpp>
#include <galaxy/core/ServiceLocator.hpp>

#include "../layers/ECSLayer.hpp"
#include "../layers/ImGuiLayer.hpp"

#include "Editor.hpp"

namespace sc
{
	Editor::Editor()
	{
		m_simple_postrenderer.m_shader.load_path("assets/shaders/render_to_texture.vs", "assets/shaders/render_to_texture.fs");
		SL_HANDLE.renderer()->add_post_effect(&m_simple_postrenderer);
	}

	void Editor::on_push()
	{
		m_layers.add<ECSLayer>();
		m_layers.add<ImGuiLayer>();
	}

	void Editor::on_pop()
	{
		m_layers.clear();
	}

	void Editor::events()
	{
		m_layers.events();
	}

	void Editor::update(const double dt)
	{
		m_layers.update(dt);
	}

	void Editor::render()
	{
		m_layers.render();
	}
} // namespace sc