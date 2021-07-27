///
/// Sandbox.cpp
/// sandbox_tests
///
/// Refer to LICENSE.txt for more details.
///

#include <iostream>

#include <magic_enum.hpp>
#include <nlohmann/json.hpp>

#include <galaxy/components/ParticleEffect.hpp>
#include <galaxy/core/ServiceLocator.hpp>
#include <galaxy/flags/Enabled.hpp>
#include <galaxy/fs/Config.hpp>
#include <galaxy/fs/FileSystem.hpp>
#include <galaxy/graphics/Renderer2D.hpp>
#include <galaxy/graphics/Shader.hpp>
#include <galaxy/resource/Language.hpp>
#include <galaxy/resource/MusicBook.hpp>
#include <galaxy/resource/SoundBook.hpp>
#include <galaxy/scripting/JSONUtils.hpp>

#include "Sandbox.hpp"

using namespace galaxy;
using namespace std::chrono_literals;

//ui::Progressbar* progressbar;

input::Keys parse_key(const std::string& key)
{
	return magic_enum::enum_cast<input::Keys>(key).value();
}

namespace sb
{
	Sandbox::Sandbox(core::Application* app)
		: Layer {app}
	{
		m_window = SL_HANDLE.window();

		// clang-format off
		m_timer.set_repeating(true);
		m_timer.set([]() {
			std::cout << "Timer Ping" << std::endl;
		}, 1000);
		m_timer.start();
		// clang-format on

		create_sandbox_scene();
		create_physics_scene();
		create_map_scene();

		m_scene_stack.push("SandboxScene");
	}

	Sandbox::~Sandbox()
	{
		m_timer.stop();
		m_window = nullptr;
		//progressbar = nullptr;
	}

	void Sandbox::events()
	{
		if (m_window->key_pressed(input::Keys::ESC))
		{
			m_window->close();
		}

		if (m_window->key_pressed(input::Keys::KEY_1))
		{
			m_scene_stack.pop();
			m_scene_stack.push("SandboxScene");
		}

		if (m_window->key_pressed(input::Keys::KEY_2))
		{
			m_scene_stack.pop();
			m_scene_stack.push("PhysicsScene");
		}

		if (m_window->key_pressed(input::Keys::KEY_3))
		{
			m_scene_stack.pop();
			m_scene_stack.push("MapScene");
		}

		m_scene_stack.events();
	}

	void Sandbox::update()
	{
		m_timer.update();

		/*
		if (m_scene_stack.top()->m_name == "SandboxScene")
		{
			static float progress = 0.0f;
			progress += (0.01 * dt);
			if (progress > 1.0f)
			{
				progress = 0.0f;
			}

			progressbar->set_progress(progress);
		}
		*/

		m_scene_stack.update();
	}

	void Sandbox::pre_render()
	{
		m_scene_stack.pre_render();
	}

	void Sandbox::render()
	{
		m_scene_stack.render();
	}

	void Sandbox::create_sandbox_scene()
	{
		auto sandbox = m_scene_stack.create("SandboxScene");

		sandbox->m_world.create_from_json("bg.json");
		sandbox->m_world.create_from_json("point.json");
		sandbox->m_world.create_from_json("line.json");
		sandbox->m_world.create_from_json("circle.json");
		sandbox->m_world.create_from_json("batch_a.json");
		sandbox->m_world.create_from_json("batch_b.json");
		sandbox->m_world.create_from_json("text.json");
		sandbox->m_world.create_from_json("big_text.json");
		sandbox->m_world.create_from_json("animated.json");
		sandbox->m_world.create_from_json("sprite.json");

		const auto opt = sandbox->m_world.create_from_json("particles.json");
		if (opt != std::nullopt)
		{
			m_particles = opt.value();
		}

		/*
		sandbox->m_gui_theme.m_font_col = {255, 0, 0, 255};

		auto* image = sandbox->m_gui.create_widget<galaxy::ui::Image>();
		image->create("image");
		image->set_pos(1000, 100);

		auto* tooltip = image->create_tooltip();
		tooltip->create("Demo Test", "public16");

		auto* label = sandbox->m_gui.create_widget<galaxy::ui::Label>();
		label->create("Label Demo", "public16");
		label->set_pos(300, 300);

		auto* button = sandbox->m_gui.create_widget<galaxy::ui::Button>();
		button->create("button_default", "button_pressed", "button_hover");
		button->set_pos(250, 250);
		button->set_onclick("on_click");

		auto* slider_ptr = sandbox->m_gui.create_widget<galaxy::ui::Slider>();
		slider_ptr->create("slider", "slider_marker");
		slider_ptr->set_pos(500, 500);

		progressbar = sandbox->m_gui.create_widget<galaxy::ui::Progressbar>();
		progressbar->create("container", "bar");
		progressbar->set_pos(500, 600);

		auto* togglebutton = sandbox->m_gui.create_widget<galaxy::ui::ToggleButton>();
		togglebutton->create("tb_on", "tb_off", "tb_on_hover", "tb_off_hover");
		togglebutton->set_pos(150, 150);

		auto* textinput = sandbox->m_gui.create_widget<galaxy::ui::TextInput>();
		textinput->create("input_field", "public16", 5.0f);
		textinput->set_pos(650, 650);

		auto* textbox = sandbox->m_gui.create_widget<galaxy::ui::Textbox>();
		textbox->create("textbox", "arrow", "public16", 5.0f);
		textbox->set_pos(600, 100);

		// Get messages from language file.
		std::vector<ui::Textbox::Message> messages;
		messages.emplace_back(SL_HANDLE.lang()->translate("demo_textbox_greet"));
		messages.emplace_back(SL_HANDLE.lang()->translate("demo_textbox_question"), "speaker_bob.png");
		messages.emplace_back(SL_HANDLE.lang()->translate("demo_textbox_reply"));
		textbox->set_text(messages);

		sandbox->m_gui.enable_input();
		*/

		sandbox->m_dispatcher.subscribe_callback<events::MouseReleased>(
			[sandbox, this](const events::MouseReleased& mre)
			{
				if (mre.m_button == input::MouseButtons::BUTTON_RIGHT)
				{
					sandbox->m_world.get<components::ParticleEffect>(m_particles)->regen(std::make_optional<glm::vec2>(mre.m_x, mre.m_y));
					sandbox->m_world.enable(m_particles);
				}
			});

		sandbox->m_dispatcher.subscribe_callback<events::KeyDown>(
			[&](const events::KeyDown& kde)
			{
				switch (kde.m_keycode)
				{
					case input::Keys::Z:
					{
						nlohmann::json json = serialize();
						json::save_to_disk("assets/saves/test.json", json);
					}
					break;

					case input::Keys::X:
					{
						const auto json = json::parse_from_disk("assets/saves/test.json");
						if (json != std::nullopt)
						{
							deserialize(json.value());
						}
					}
					break;

					case input::Keys::M:
						SL_HANDLE.musicbook()->get("PleasingGuns")->play();
						break;

					case input::Keys::P:
						SL_HANDLE.musicbook()->get("PleasingGuns")->pause();
						break;

					case input::Keys::N:
						SL_HANDLE.musicbook()->get("PleasingGuns")->stop();
						break;

					case input::Keys::B:
						SL_HANDLE.soundbook()->get("button")->play();
						break;
				}
			});

		sandbox->m_camera.m_forward_key = parse_key(SL_HANDLE.config()->get<std::string>("key-forward"));
		sandbox->m_camera.m_back_key    = parse_key(SL_HANDLE.config()->get<std::string>("key-back"));
		sandbox->m_camera.m_left_key    = parse_key(SL_HANDLE.config()->get<std::string>("key-left"));
		sandbox->m_camera.m_right_key   = parse_key(SL_HANDLE.config()->get<std::string>("key-right"));
	}

	void Sandbox::create_physics_scene()
	{
		auto physics = m_scene_stack.create("PhysicsScene");

		physics->m_world.create_from_json("bg.json");
		physics->m_world.create_from_json("floor.json");
		m_cube = physics->m_world.create_from_json("cube.json").value();

		physics->m_dispatcher.subscribe_callback<events::KeyRepeat>(
			[physics, this](const events::KeyRepeat& kde)
			{
				switch (kde.m_keycode)
				{
					case input::Keys::UP:
						physics->m_world.get<components::Transform2D>(m_cube)->move(0.0f, -10.0f);
						break;

					case input::Keys::DOWN:
						physics->m_world.get<components::Transform2D>(m_cube)->move(0.0f, 10.0f);
						break;

					case input::Keys::LEFT:
						physics->m_world.get<components::Transform2D>(m_cube)->move(-10.0f, 0.0f);
						break;

					case input::Keys::RIGHT:
						physics->m_world.get<components::Transform2D>(m_cube)->move(10.0f, 0.0f);
						break;

					case input::Keys::Q:
						physics->m_world.get<components::Transform2D>(m_cube)->rotate(-2.0f);
						break;

					case input::Keys::E:
						physics->m_world.get<components::Transform2D>(m_cube)->rotate(2.0f);
						break;
				}
			});

		physics->m_camera.m_forward_key = parse_key(SL_HANDLE.config()->get<std::string>("key-forward"));
		physics->m_camera.m_back_key    = parse_key(SL_HANDLE.config()->get<std::string>("key-back"));
		physics->m_camera.m_left_key    = parse_key(SL_HANDLE.config()->get<std::string>("key-left"));
		physics->m_camera.m_right_key   = parse_key(SL_HANDLE.config()->get<std::string>("key-right"));
	}

	void Sandbox::create_map_scene()
	{
		auto map = m_scene_stack.create("MapScene");

		map->m_world.create_from_json("bg.json");

		map->create_maps("assets/maps/maps.world");
		map->set_active_map("desert");

		map->m_dispatcher.subscribe_callback<events::KeyDown>(
			[map, this](const events::KeyDown& kde)
			{
				switch (kde.m_keycode)
				{
					case input::Keys::Z:
						map->get_active_map()->enable_objects(map->m_world);
						break;

					case input::Keys::X:
						map->get_active_map()->disable_objects(map->m_world);
						break;
				}
			});

		map->m_camera.m_forward_key = parse_key(SL_HANDLE.config()->get<std::string>("key-forward"));
		map->m_camera.m_back_key    = parse_key(SL_HANDLE.config()->get<std::string>("key-back"));
		map->m_camera.m_left_key    = parse_key(SL_HANDLE.config()->get<std::string>("key-left"));
		map->m_camera.m_right_key   = parse_key(SL_HANDLE.config()->get<std::string>("key-right"));
	}
} // namespace sb