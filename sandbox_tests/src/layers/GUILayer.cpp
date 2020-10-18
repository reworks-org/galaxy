///
/// GUILayer.cpp
/// sandbox_tests
///
/// Refer to LICENSE.txt for more details.
///

#include <qs/core/Renderer.hpp>

#include <galaxy/core/ServiceLocator.hpp>
#include <galaxy/fs/FileSystem.hpp>
#include <galaxy/res/FontBook.hpp>
#include <galaxy/res/ShaderBook.hpp>

#include <galaxy/ui/widgets/Image.hpp>

#include "GUILayer.hpp"

///
/// Core namespace.
///
namespace sb
{
	GUILayer::GUILayer()
	{
		set_name("gui_layer");

		auto demo_tex   = galaxy::FileSystem::s_root + galaxy::FileSystem::s_textures + "demo_nineslice.png";
		auto demo_font  = galaxy::FileSystem::s_root + galaxy::FileSystem::s_fonts + "public.ttf";
		auto rtt_shader = SL_HANDLE.shaderbook()->get("render_to_texture");

		m_theme.m_fonts    = SL_HANDLE.fontbook();
		m_theme.m_shaders  = SL_HANDLE.shaderbook();
		m_theme.m_renderer = SL_HANDLE.renderer();
		m_theme.m_window   = SL_HANDLE.window();
		m_theme.m_font_col = {255, 255, 255, 255};

		m_theme.m_atlas.add(demo_tex);

		rtt_shader->bind();
		m_theme.m_atlas.create(*SL_HANDLE.renderer(), *rtt_shader);

		m_gui.set_theme(&m_theme);
		m_gui.construct(*SL_HANDLE.pool());

		auto* image = m_gui.create_widget<galaxy::widget::Image>();
		image->create_from_atlas("demo_nineslice");
	}

	GUILayer::~GUILayer()
	{
	}

	void GUILayer::events()
	{
	}

	void GUILayer::update(const double dt)
	{
		m_gui.update();
	}

	void GUILayer::render(qs::Camera& camera)
	{
		m_gui.render(camera);
	}
} // namespace sb