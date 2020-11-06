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
#include <qs/core/Window.hpp>

#include <galaxy/ui/widgets/Image.hpp>
#include <galaxy/ui/widgets/Label.hpp>
#include <galaxy/ui/widgets/Button.hpp>
#include <galaxy/ui/widgets/ToggleButton.hpp>
#include <galaxy/ui/widgets/TextInput.hpp>

#include <protostar/events/MouseMovedEvent.hpp>

#include "GUILayer.hpp"

// clang-format off
#define WHITE {255, 255, 255, 255}
#define BLACK {0, 0, 0, 255}
#define RED {255, 0, 0, 255}
// clang-format on

galaxy::widget::Progressbar* progressbar;

///
/// Core namespace.
///
namespace sb
{
	GUILayer::GUILayer()
	{
		set_name("gui_layer");

		auto demo_tex = galaxy::FileSystem::s_root + galaxy::FileSystem::s_textures + "demo_nineslice.png";

		auto button_default = galaxy::FileSystem::s_root + galaxy::FileSystem::s_textures + "button_default.png";
		auto button_pressed = galaxy::FileSystem::s_root + galaxy::FileSystem::s_textures + "button_pressed.png";
		auto button_hover   = galaxy::FileSystem::s_root + galaxy::FileSystem::s_textures + "button_hover.png";

		auto slider        = galaxy::FileSystem::s_root + galaxy::FileSystem::s_textures + "slider.png";
		auto slider_marker = galaxy::FileSystem::s_root + galaxy::FileSystem::s_textures + "slider_marker.png";

		auto container = galaxy::FileSystem::s_root + galaxy::FileSystem::s_textures + "container.png";
		auto bar       = galaxy::FileSystem::s_root + galaxy::FileSystem::s_textures + "bar.png";

		auto tb_on        = galaxy::FileSystem::s_root + galaxy::FileSystem::s_textures + "tb_on.png";
		auto tb_off       = galaxy::FileSystem::s_root + galaxy::FileSystem::s_textures + "tb_off.png";
		auto tb_on_hover  = galaxy::FileSystem::s_root + galaxy::FileSystem::s_textures + "tb_on_hover.png";
		auto tb_off_hover = galaxy::FileSystem::s_root + galaxy::FileSystem::s_textures + "tb_off_hover.png";

		auto input_field = galaxy::FileSystem::s_root + galaxy::FileSystem::s_textures + "input_field.png";

		auto demo_font  = galaxy::FileSystem::s_root + galaxy::FileSystem::s_fonts + "public.ttf";
		auto rtt_shader = SL_HANDLE.shaderbook()->get("render_to_texture");

		m_theme.m_fonts    = SL_HANDLE.fontbook();
		m_theme.m_shaders  = SL_HANDLE.shaderbook();
		m_theme.m_renderer = SL_HANDLE.renderer();
		m_theme.m_window   = SL_HANDLE.window();
		m_theme.m_font_col = RED;

		m_theme.m_fonts->create("public16", demo_font, 16);
		m_theme.m_atlas.add(demo_tex);
		m_theme.m_atlas.add(button_default);
		m_theme.m_atlas.add(button_pressed);
		m_theme.m_atlas.add(button_hover);
		m_theme.m_atlas.add(slider);
		m_theme.m_atlas.add(slider_marker);
		m_theme.m_atlas.add(container);
		m_theme.m_atlas.add(bar);
		m_theme.m_atlas.add(tb_on);
		m_theme.m_atlas.add(tb_off);
		m_theme.m_atlas.add(tb_on_hover);
		m_theme.m_atlas.add(tb_off_hover);
		m_theme.m_atlas.add(input_field);

		rtt_shader->bind();
		m_theme.m_atlas.create(*SL_HANDLE.renderer(), *rtt_shader);

		m_gui.set_theme(&m_theme);

		auto* image = m_gui.create_widget<galaxy::widget::Image>();
		image->create_from_atlas("demo_nineslice");

		auto* tooltip = m_gui.create_tooltip_for_widget(image);
		tooltip->create(demo_tex, "Demo Test", "public16");

		m_gui.add_event_to_widget<pr::MouseMovedEvent>(image);

		auto* label = m_gui.create_widget<galaxy::widget::Label>();
		label->create("Label Demo", "public16");
		label->set_pos(300, 300);

		auto* button = m_gui.create_widget<galaxy::widget::Button>();
		button->create_from_atlas("button_default", "button_pressed", "button_hover");
		button->set_pos(250, 250);
		button->set_callback([&]() {
			PL_LOG(PL_INFO, "Button Pressed.");
		});

		m_gui.add_event_to_widget<pr::MouseMovedEvent>(button);
		m_gui.add_event_to_widget<pr::MousePressedEvent>(button);
		m_gui.add_event_to_widget<pr::MouseReleasedEvent>(button);

		auto* slider_ptr = m_gui.create_widget<galaxy::widget::Slider>();
		slider_ptr->create("slider", "slider_marker");
		slider_ptr->set_pos(500, 500);

		m_gui.add_event_to_widget<pr::MouseMovedEvent>(slider_ptr);
		m_gui.add_event_to_widget<pr::MousePressedEvent>(slider_ptr);
		m_gui.add_event_to_widget<pr::MouseReleasedEvent>(slider_ptr);

		progressbar = m_gui.create_widget<galaxy::widget::Progressbar>();
		progressbar->create("container", "bar");
		progressbar->set_pos(500, 600);

		m_gui.add_event_to_widget<pr::MouseMovedEvent>(progressbar);

		auto* togglebutton = m_gui.create_widget<galaxy::widget::ToggleButton>();
		togglebutton->create_from_atlas("tb_on", "tb_off", "tb_on_hover", "tb_off_hover");
		togglebutton->set_pos(150, 150);

		m_gui.add_event_to_widget<pr::MouseMovedEvent>(togglebutton);
		m_gui.add_event_to_widget<pr::MousePressedEvent>(togglebutton);

		auto* textinput = m_gui.create_widget<galaxy::widget::TextInput>();
		textinput->create_from_atlas("input_field", "public16", 5.0f);
		textinput->set_pos(650, 650);

		m_gui.add_event_to_widget<pr::MouseMovedEvent>(textinput);
		m_gui.add_event_to_widget<pr::MousePressedEvent>(textinput);
		m_gui.add_event_to_widget<pr::KeyDownEvent>(textinput);
	}

	GUILayer::~GUILayer()
	{
		progressbar = nullptr;
	}

	void GUILayer::events()
	{
		auto* window        = SL_HANDLE.window();
		auto [changed, pos] = window->get_cursor_pos();

		if (changed)
		{
			m_gui.trigger<pr::MouseMovedEvent>(pos.x, pos.y);
		}

		if (window->mouse_button_pressed(GLFW_MOUSE_BUTTON_LEFT))
		{
			m_gui.trigger<pr::MousePressedEvent>(pos.x, pos.y, GLFW_MOUSE_BUTTON_LEFT);
		}

		if (window->mouse_button_released(GLFW_MOUSE_BUTTON_LEFT))
		{
			m_gui.trigger<pr::MouseReleasedEvent>(pos.x, pos.y, GLFW_MOUSE_BUTTON_LEFT);
		}

		if (window->key_pressed(pr::Keys::ENTER))
		{
			m_gui.trigger<pr::KeyDownEvent>(pr::Keys::ENTER);
		}

		if (window->key_pressed(pr::Keys::BACKSPACE))
		{
			m_gui.trigger<pr::KeyDownEvent>(pr::Keys::BACKSPACE);
		}
	}

	void GUILayer::update(const double dt)
	{
		static float progress = 0.0f;
		progress += (0.01 * dt);
		if (progress > 1.0f)
		{
			progress = 0.0f;
		}

		progressbar->set_progress(progress);
		m_gui.update(dt);
	}

	void GUILayer::render(qs::Camera& camera)
	{
		m_gui.render(camera);
	}
} // namespace sb