///
/// GUILayer.cpp
/// sandbox_tests
///
/// Refer to LICENSE.txt for more details.
///

#include <galaxy/graphicsRenderer.hpp>

#include <galaxy/core/ServiceLocator.hpp>
#include <galaxy/fs/FileSystem.hpp>
#include <galaxy/res/FontBook.hpp>
#include <galaxy/res/ShaderBook.hpp>
#include <galaxy/graphicsWindow.hpp>

#include <galaxy/ui/widgets/Image.hpp>
#include <galaxy/ui/widgets/Label.hpp>
#include <galaxy/ui/widgets/Button.hpp>
#include <galaxy/ui/widgets/ToggleButton.hpp>
#include <galaxy/ui/widgets/TextInput.hpp>

#include "galaxy/events/MouseMoved.hpp"

#include "GUILayer.hpp"

// clang-format off
#define WHITE {255, 255, 255, 255}
#define BLACK {0, 0, 0, 255}
#define RED {255, 0, 0, 255}
// clang-format on

widget::Progressbar* progressbar;

///
/// Core namespace.
///
namespace sb
{
	GUILayer::GUILayer()
	{
		set_name("gui_layer");
		m_gui_camera.create(0.0f, SL_HANDLE.window()->get_width(), SL_HANDLE.window()->get_height(), 0.0f);

		auto demo_tex = fs::s_root + fs::s_textures + "demo_nineslice.png";

		auto button_default = fs::s_root + fs::s_textures + "button_default.png";
		auto button_pressed = fs::s_root + fs::s_textures + "button_pressed.png";
		auto button_hover   = fs::s_root + fs::s_textures + "button_hover.png";

		auto slider        = fs::s_root + fs::s_textures + "slider.png";
		auto slider_marker = fs::s_root + fs::s_textures + "slider_marker.png";

		auto container = fs::s_root + fs::s_textures + "container.png";
		auto bar       = fs::s_root + fs::s_textures + "bar.png";

		auto tb_on        = fs::s_root + fs::s_textures + "tb_on.png";
		auto tb_off       = fs::s_root + fs::s_textures + "tb_off.png";
		auto tb_on_hover  = fs::s_root + fs::s_textures + "tb_on_hover.png";
		auto tb_off_hover = fs::s_root + fs::s_textures + "tb_off_hover.png";

		auto input_field = fs::s_root + fs::s_textures + "input_field.png";

		auto textbox_box = fs::s_root + fs::s_textures + "textbox.png";
		auto arrow       = fs::s_root + fs::s_textures + "arrow.png";

		auto demo_font  = fs::s_root + fs::s_fonts + "public.ttf";
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

		auto* image = m_gui.create_widget<widget::Image>();
		image->create("demo_nineslice");
		image->set_pos(1000, 100);

		auto* tooltip = m_gui.create_tooltip_for_widget(image);
		tooltip->create(demo_tex, "Demo Test", "public16");

		m_gui.add_event_to_widget<events::MouseMoved>(image);

		auto* label = m_gui.create_widget<widget::Label>();
		label->create("Label Demo", "public16");
		label->set_pos(300, 300);

		auto* button = m_gui.create_widget<widget::Button>();
		button->create("button_default", "button_pressed", "button_hover");
		button->set_pos(250, 250);
		button->set_callback([&]() {
			PL_LOG(PL_INFO, "Button Pressed.");
		});

		m_gui.add_event_to_widget<events::MouseMoved>(button);
		m_gui.add_event_to_widget<events::MousePressed>(button);
		m_gui.add_event_to_widget<events::MouseReleased>(button);

		auto* slider_ptr = m_gui.create_widget<widget::Slider>();
		slider_ptr->create("slider", "slider_marker");
		slider_ptr->set_pos(500, 500);

		m_gui.add_event_to_widget<events::MouseMoved>(slider_ptr);
		m_gui.add_event_to_widget<events::MousePressed>(slider_ptr);
		m_gui.add_event_to_widget<events::MouseReleased>(slider_ptr);

		progressbar = m_gui.create_widget<widget::Progressbar>();
		progressbar->create("container", "bar");
		progressbar->set_pos(500, 600);

		m_gui.add_event_to_widget<events::MouseMoved>(progressbar);

		auto* togglebutton = m_gui.create_widget<widget::ToggleButton>();
		togglebutton->create("tb_on", "tb_off", "tb_on_hover", "tb_off_hover");
		togglebutton->set_pos(150, 150);

		m_gui.add_event_to_widget<events::MouseMoved>(togglebutton);
		m_gui.add_event_to_widget<events::MousePressed>(togglebutton);

		auto* textinput = m_gui.create_widget<widget::TextInput>();
		textinput->create("input_field", "public16", 5.0f);
		textinput->set_pos(650, 650);

		m_gui.add_event_to_widget<events::MouseMoved>(textinput);
		m_gui.add_event_to_widget<events::MousePressed>(textinput);
		m_gui.add_event_to_widget<pr::KeyDownEvent>(textinput);

		auto* textbox = m_gui.create_widget<widget::Textbox>();
		textbox->create("textbox", "arrow", "public16", 5.0f);
		textbox->set_pos(600, 100);

		std::vector<std::string> messages = {"Hello there!", "How are you today?", "I am good,\nthanks!"};
		textbox->set_text(messages);

		m_gui.add_event_to_widget<events::MouseMoved>(textbox);
		m_gui.add_event_to_widget<pr::KeyDownEvent>(textbox);
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
			m_gui.trigger<events::MouseMoved>(pos.x, pos.y);
		}

		if (window->mouse_button_pressed(pr::MouseButton::BUTTON_LEFT))
		{
			m_gui.trigger<events::MousePressed>(pos.x, pos.y, pr::MouseButton::BUTTON_LEFT);
		}

		if (window->mouse_button_released(pr::MouseButton::BUTTON_LEFT))
		{
			m_gui.trigger<events::MouseReleased>(pos.x, pos.y, pr::MouseButton::BUTTON_LEFT);
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

	void GUILayer::pre_render()
	{
	}

	void GUILayer::render()
	{
		m_gui.render(m_gui_camera);
	}
} // namespace sb