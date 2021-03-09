///
/// SandboxScene.cpp
/// sandbox_tests
///
/// Refer to LICENSE.txt for more details.
///

#include <galaxy/core/ServiceLocator.hpp>
#include <galaxy/core/Window.hpp>

#include <galaxy/fs/Serializer.hpp>

#include <galaxy/graphics/Renderer2D.hpp>

#include <galaxy/res/MusicBook.hpp>
#include <galaxy/res/SoundBook.hpp>
#include <galaxy/res/TextureAtlas.hpp>

#include <galaxy/systems/AnimationSystem.hpp>
#include <galaxy/systems/RenderSystem.hpp>
#include <galaxy/systems/TransformSystem.hpp>

#include <galaxy/ui/widgets/Button.hpp>
#include <galaxy/ui/widgets/Image.hpp>
#include <galaxy/ui/Widgets/Label.hpp>
#include <galaxy/ui/widgets/ProgressBar.hpp>
#include <galaxy/ui/widgets/Slider.hpp>
#include <galaxy/ui/widgets/Textbox.hpp>
#include <galaxy/ui/widgets/TextInput.hpp>
#include <galaxy/ui/widgets/ToggleButton.hpp>

#include "SandboxScene.hpp"

using namespace galaxy;
using namespace std::chrono_literals;

ui::Progressbar* progressbar;

namespace sb
{
	SandboxScene::SandboxScene()
	    : Scene {"SandboxScene"}
	{
		m_camera.create(0.0f, SL_HANDLE.window()->get_width(), SL_HANDLE.window()->get_height(), 0.0f);
		m_camera.set_speed(100.0f);

		SL_HANDLE.window()->register_on_window_resize(m_camera);

		m_world.create_from_json("point.json");
		m_world.create_from_json("line.json");
		m_world.create_from_json("circle.json");
		m_world.create_from_json("batch_a.json");
		m_world.create_from_json("batch_b.json");
		m_world.create_from_json("text.json");
		m_world.create_from_json("animated.json");
		m_world.create_from_json("sprite.json");

		m_world.create_system<systems::TransformSystem>();
		m_world.create_system<systems::AnimationSystem>();
		m_world.create_system<systems::RenderSystem>();

		// clang-format off
		m_timer.set_repeating(true);
		m_timer.set([]() {
			std::cout << "Timer Ping" << std::endl;
		}, 1000);
		m_timer.start();
		// clang-format on

		m_theme.m_font_col = {255, 0, 0, 255};
		m_theme.m_camera.create(0.0f, (float)SL_HANDLE.window()->get_width(), (float)SL_HANDLE.window()->get_height(), 0.0f);
		m_gui.set_theme(&m_theme);

		auto* image = m_gui.create_widget<galaxy::ui::Image>();
		image->create("image");
		image->set_pos(1000, 100);

		auto* tooltip = m_gui.create_tooltip_for_widget(image);
		tooltip->create("Demo Test", "public16");

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

		m_particle_gen.create_from_json("demo_particle_gen.json");
	}

	SandboxScene::~SandboxScene()
	{
		m_timer.stop();
		progressbar = nullptr;
	}

	void SandboxScene::events()
	{
		if (SL_HANDLE.window()->key_pressed(input::Keys::Z))
		{
			fs::Serializer::serialize(this, "assets/saves/");
		}

		if (SL_HANDLE.window()->key_pressed(input::Keys::X))
		{
			fs::Serializer::deserialize(this, "assets/saves/");
		}

		if (SL_HANDLE.window()->key_pressed(input::Keys::M))
		{
			SL_HANDLE.musicbook()->get("PleasingGuns")->play();
		}

		if (SL_HANDLE.window()->key_pressed(input::Keys::P))
		{
			SL_HANDLE.musicbook()->get("PleasingGuns")->pause();
		}

		if (SL_HANDLE.window()->key_pressed(input::Keys::N))
		{
			SL_HANDLE.musicbook()->get("PleasingGuns")->stop();
		}

		if (SL_HANDLE.window()->key_pressed(input::Keys::B))
		{
			SL_HANDLE.soundbook()->get("button")->play();
		}

		if (SL_HANDLE.window()->key_down(input::Keys::W))
		{
			m_camera.on_key_down({input::Keys::W});
		}
		else
		{
			m_camera.on_key_up({input::Keys::W});
		}

		if (SL_HANDLE.window()->key_down(input::Keys::S))
		{
			m_camera.on_key_down({input::Keys::S});
		}
		else
		{
			m_camera.on_key_up({input::Keys::S});
		}

		if (SL_HANDLE.window()->key_down(input::Keys::A))
		{
			m_camera.on_key_down({input::Keys::A});
		}
		else
		{
			m_camera.on_key_up({input::Keys::A});
		}

		if (SL_HANDLE.window()->key_down(input::Keys::D))
		{
			m_camera.on_key_down({input::Keys::D});
		}
		else
		{
			m_camera.on_key_up({input::Keys::D});
		}

		auto [changed, pos] = SL_HANDLE.window()->get_cursor_pos();
		if (changed)
		{
			m_gui.trigger<galaxy::events::MouseMoved>(pos.x, pos.y);
		}

		if (SL_HANDLE.window()->mouse_button_pressed(galaxy::input::MouseButton::BUTTON_LEFT))
		{
			m_gui.trigger<galaxy::events::MousePressed>(pos.x, pos.y, galaxy::input::MouseButton::BUTTON_LEFT);
		}

		if (SL_HANDLE.window()->mouse_button_pressed(galaxy::input::MouseButton::BUTTON_RIGHT))
		{
			m_particle_gen.update_emitter(pos.x, pos.y);
			m_particle_gen.gen_circular("default", 100, 100.0f, 10.0f, 10.0f);
		}

		if (SL_HANDLE.window()->mouse_button_released(galaxy::input::MouseButton::BUTTON_LEFT))
		{
			m_gui.trigger<galaxy::events::MouseReleased>(pos.x, pos.y, galaxy::input::MouseButton::BUTTON_LEFT);
		}

		if (SL_HANDLE.window()->key_pressed(galaxy::input::Keys::ENTER))
		{
			m_gui.trigger<galaxy::events::KeyDown>(galaxy::input::Keys::ENTER);
		}

		if (SL_HANDLE.window()->key_pressed(galaxy::input::Keys::BACKSPACE))
		{
			m_gui.trigger<galaxy::events::KeyDown>(galaxy::input::Keys::BACKSPACE);
		}
	}

	void SandboxScene::update(const double dt)
	{
		m_timer.update(dt);
		m_particle_gen.update(dt);
		m_camera.update(dt);
		m_world.update(dt);

		static float progress = 0.0f;
		progress += (0.01 * dt);
		if (progress > 1.0f)
		{
			progress = 0.0f;
		}

		progressbar->set_progress(progress);
		m_gui.update(dt);
	}

	void SandboxScene::pre_render()
	{
	}

	void SandboxScene::render()
	{
		m_world.get_system<systems::RenderSystem>()->render(m_world, m_camera);
		graphics::Renderer2D::draw_particles(&m_particle_gen, m_camera);
		m_gui.render();
	}
} // namespace sb