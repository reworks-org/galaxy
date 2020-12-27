///
/// GUILayer.cpp
/// sandbox_tests
///
/// Refer to LICENSE.txt for more details.
///

#include <galaxy/core/ServiceLocator.hpp>
#include <galaxy/events/KeyDown.hpp>
#include <galaxy/events/MouseMoved.hpp>
#include <galaxy/fs/FileSystem.hpp>
#include <galaxy/res/FontBook.hpp>
#include <galaxy/res/ShaderBook.hpp>
#include <galaxy/graphics/Window.hpp>
#include <galaxy/graphics/Renderer.hpp>
#include <galaxy/ui/widgets/Image.hpp>
#include <galaxy/ui/widgets/Label.hpp>
#include <galaxy/ui/widgets/Button.hpp>
#include <galaxy/ui/widgets/ToggleButton.hpp>
#include <galaxy/ui/widgets/TextInput.hpp>

#include "GUILayer.hpp"

// clang-format off
#define WHITE {255, 255, 255, 255}
#define BLACK {0, 0, 0, 255}
#define RED {255, 0, 0, 255}
// clang-format on

galaxy::ui::Progressbar* progressbar;

namespace sb
{
	GUILayer::GUILayer()
	{
		set_name("gui_layer");
		m_gui_camera.create(0.0f, SL_HANDLE.window()->get_width(), SL_HANDLE.window()->get_height(), 0.0f);

		auto demo_tex = galaxy::fs::s_root + galaxy::fs::s_textures + "demo_nineslice.png";

		auto button_default = galaxy::fs::s_root + galaxy::fs::s_textures + "button_default.png";
		auto button_pressed = galaxy::fs::s_root + galaxy::fs::s_textures + "button_pressed.png";
		auto button_hover   = galaxy::fs::s_root + galaxy::fs::s_textures + "button_hover.png";

		auto slider        = galaxy::fs::s_root + galaxy::fs::s_textures + "slider.png";
		auto slider_marker = galaxy::fs::s_root + galaxy::fs::s_textures + "slider_marker.png";

		auto container = galaxy::fs::s_root + galaxy::fs::s_textures + "container.png";
		auto bar       = galaxy::fs::s_root + galaxy::fs::s_textures + "bar.png";

		auto tb_on        = galaxy::fs::s_root + galaxy::fs::s_textures + "tb_on.png";
		auto tb_off       = galaxy::fs::s_root + galaxy::fs::s_textures + "tb_off.png";
		auto tb_on_hover  = galaxy::fs::s_root + galaxy::fs::s_textures + "tb_on_hover.png";
		auto tb_off_hover = galaxy::fs::s_root + galaxy::fs::s_textures + "tb_off_hover.png";

		auto input_field = galaxy::fs::s_root + galaxy::fs::s_textures + "input_field.png";

		auto textbox_box = galaxy::fs::s_root + galaxy::fs::s_textures + "textbox.png";
		auto arrow       = galaxy::fs::s_root + galaxy::fs::s_textures + "arrow.png";

		auto demo_font  = galaxy::fs::s_root + galaxy::fs::s_fonts + "public.ttf";
		auto rtt_shader = SL_HANDLE.shaderbook()->get("render_to_texture");

		m_theme.m_fonts    = SL_HANDLE.fontbook();
		m_theme.m_shaders  = SL_HANDLE.shaderbook();
		m_theme.m_renderer = SL_HANDLE.renderer();
		m_theme.m_window   = SL_HANDLE.window();
		m_theme.m_font_col = RED;

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
		m_theme.m_atlas.add(textbox_box);
		m_theme.m_atlas.add(arrow);

		rtt_shader->bind();
		m_theme.m_atlas.create(*SL_HANDLE.renderer(), *rtt_shader);

		m_gui.set_theme(&m_theme);

		auto* image = m_gui.create_widget<galaxy::ui::Image>();
		image->create("demo_nineslice");
		image->set_pos(1000, 100);

		auto* tooltip = m_gui.create_tooltip_for_widget(image);
		tooltip->create(demo_tex, "Demo Test", "public16");

		m_gui.add_event_to_widget<galaxy::events::MouseMoved>(image);

		auto* label = m_gui.create_widget<galaxy::ui::Label>();
		label->create("Label Demo", "public16");
		label->set_pos(300, 300);

		auto* button = m_gui.create_widget<galaxy::ui::Button>();
		button->create("button_default", "button_pressed", "button_hover");
		button->set_pos(250, 250);
		button->set_callback([&]() {
			GALAXY_LOG(GALAXY_INFO, "Button Pressed.");
		});

		m_gui.add_event_to_widget<galaxy::events::MouseMoved>(button);
		m_gui.add_event_to_widget<galaxy::events::MousePressed>(button);
		m_gui.add_event_to_widget<galaxy::events::MouseReleased>(button);

		auto* slider_ptr = m_gui.create_widget<galaxy::ui::Slider>();
		slider_ptr->create("slider", "slider_marker");
		slider_ptr->set_pos(500, 500);

		m_gui.add_event_to_widget<galaxy::events::MouseMoved>(slider_ptr);
		m_gui.add_event_to_widget<galaxy::events::MousePressed>(slider_ptr);
		m_gui.add_event_to_widget<galaxy::events::MouseReleased>(slider_ptr);

		progressbar = m_gui.create_widget<galaxy::ui::Progressbar>();
		progressbar->create("container", "bar");
		progressbar->set_pos(500, 600);

		m_gui.add_event_to_widget<galaxy::events::MouseMoved>(progressbar);

		auto* togglebutton = m_gui.create_widget<galaxy::ui::ToggleButton>();
		togglebutton->create("tb_on", "tb_off", "tb_on_hover", "tb_off_hover");
		togglebutton->set_pos(150, 150);

		m_gui.add_event_to_widget<galaxy::events::MouseMoved>(togglebutton);
		m_gui.add_event_to_widget<galaxy::events::MousePressed>(togglebutton);

		auto* textinput = m_gui.create_widget<galaxy::ui::TextInput>();
		textinput->create("input_field", "public16", 5.0f);
		textinput->set_pos(650, 650);

		m_gui.add_event_to_widget<galaxy::events::MouseMoved>(textinput);
		m_gui.add_event_to_widget<galaxy::events::MousePressed>(textinput);
		m_gui.add_event_to_widget<galaxy::events::KeyDown>(textinput);

		auto* textbox = m_gui.create_widget<galaxy::ui::Textbox>();
		textbox->create("textbox", "arrow", "public16", 5.0f);
		textbox->set_pos(600, 100);

		std::vector<std::string> messages = {"Hello there!", "How are you today?", "I am good,\nthanks!"};
		textbox->set_text(messages);

		m_gui.add_event_to_widget<galaxy::events::MouseMoved>(textbox);
		m_gui.add_event_to_widget<galaxy::events::KeyDown>(textbox);
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
			m_gui.trigger<galaxy::events::MouseMoved>(pos.x, pos.y);
		}

		if (window->mouse_button_pressed(galaxy::input::MouseButton::BUTTON_LEFT))
		{
			m_gui.trigger<galaxy::events::MousePressed>(pos.x, pos.y, galaxy::input::MouseButton::BUTTON_LEFT);
		}

		if (window->mouse_button_released(galaxy::input::MouseButton::BUTTON_LEFT))
		{
			m_gui.trigger<galaxy::events::MouseReleased>(pos.x, pos.y, galaxy::input::MouseButton::BUTTON_LEFT);
		}

		if (window->key_pressed(galaxy::input::Keys::ENTER))
		{
			m_gui.trigger<galaxy::events::KeyDown>(galaxy::input::Keys::ENTER);
		}

		if (window->key_pressed(galaxy::input::Keys::BACKSPACE))
		{
			m_gui.trigger<galaxy::events::KeyDown>(galaxy::input::Keys::BACKSPACE);
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

	void GUILayer::pre_render()
	{
	}

	void GUILayer::render()
	{
		m_gui.render(m_gui_camera);
	}
} // namespace sb