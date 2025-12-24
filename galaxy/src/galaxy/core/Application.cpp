///
/// Application.cpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#include <format>

#include <BS_thread_pool.hpp>
#include <entt/signal/dispatcher.hpp>
#include <mimalloc.h>
#include <SDL3/SDL.h>
#include <sol/sol.hpp>

#include "galaxy/core/Config.hpp"
#include "galaxy/core/Window.hpp"
#include "galaxy/core/Settings.hpp"
#include "galaxy/fs/VirtualFileSystem.hpp"
#include "galaxy/logging/ConsoleSink.hpp"
#include "galaxy/logging/FileSink.hpp"
#include "galaxy/logging/Log.hpp"
#include "galaxy/lua/Lua.hpp"
#include "galaxy/meta/SystemFactory.hpp"
#include "galaxy/platform/Platform.hpp"
#include "galaxy/scene/World.hpp"
#include "galaxy/time/Time.hpp"

#include "Application.hpp"

using namespace std::chrono_literals;

namespace galaxy
{
	App::App(const std::string& config_file)
	{
		SDL_SetMemoryFunctions(&mi_malloc, &mi_calloc, &mi_realloc, &mi_free);

		setup_logging();
		setup_async();
		setup_config(config_file);
		setup_platform();
		setup_fs();
		setup_window();
		setup_events();
		// setup_nuklear();
		// setup_loader();
		setup_meta();
		setup_services();
		setup_scripting();

		// Load game assets.
		// core::entt::locator<core::Loader>::ref().load_all();

		m_update = [&](entt::dispatcher& dispatcher, Window& window, World& world) {
			// nui.begin_input();
			window.process_events(dispatcher);
			// nui.end_input();
			world.update();
		};

		m_render = [&](entt::dispatcher& dispatcher, Window& window, World& world) {
			// graphics::renderer::begin();
			world.render();
			// graphics::renderer::end();
		};
	}

	App::~App()
	{
		entt::locator<VirtualFileSystem>::reset();
		entt::locator<Config>::reset();
		entt::locator<BS::light_thread_pool>::reset();

		GALAXY_LOG(GALAXY_INFO, "Application closed.");
		entt::locator<Log>::reset();

		SDL_Quit();
	}

	/*void App::load()
	{
		const auto path = Settings::root_dir() / Settings::asset_pack();

		auto& sm = entt::locator<scene::SceneManager>::value();
		sm.load_app(path.string());
	}*/

	void App::run()
	{
		// https://stackoverflow.com/a/59446610
		// We dont need 't' or 'alpha/render' sections.

		auto& dispatcher = entt::locator<entt::dispatcher>::value();
		auto& window     = entt::locator<Window>::value();
		auto& world      = entt::locator<World>::value();

		// The expression dt/1s simply converts the double-based chrono seconds
		// into a double so it can participate in the physics computation.
		constexpr const auto dt = std::chrono::duration<long long, std::ratio<1, 60>> {1};
		time::dt(dt / 1.0s);

		using clock      = std::chrono::steady_clock;
		using duration   = decltype(clock::duration {} + dt);
		using time_point = std::chrono::time_point<clock, duration>;

		duration   accum = 0s;
		time_point prev  = clock::now();
		time_point now   = clock::now();

		auto     updates = 0u;
		auto     frames  = 0u;
		duration perf    = 0s;

		while (window.is_open())
		{
			now          = clock::now();
			auto elapsed = now - prev;

			// 250ms is the limit put in place on the frame time to cope with the spiral of death.
			// It doesn't have to be 250ms exactly but it should be sufficiently high enough to deal with spikes in load.
			if (elapsed > 250ms)
			{
				elapsed = 250ms;
			}

			prev   = now;
			accum += elapsed;

			while (accum >= dt)
			{
				perf  += dt;
				accum -= dt;

				m_update(dispatcher, window, world);
				updates++;
			}

			m_render(dispatcher, window, world);
			frames++;

			if (perf >= 1s)
			{
				window.append_title(std::format(" | UPS: {0}, FPS: {1}", updates, frames));

				frames  = 0;
				updates = 0;
				perf    = 0s;
			}
		}
	}

	void App::set_update_func(LoopFunc&& update)
	{
		m_update = std::move(update);
	}

	void App::set_render_func(LoopFunc&& render)
	{
		m_render = std::move(render);
	}

	void App::setup_logging()
	{
		platform::configure_terminal();
		if (!std::filesystem::exists(Settings::log_dir()))
		{
			std::filesystem::create_directory(Settings::log_dir());
		}
		entt::locator<Log>::emplace();

		const auto path = std::format("{0}{1}{2}", Settings::log_dir(), std::format("{0:%d-%m-%Y-[%H-%M]}", time::now()), ".log");
		GALAXY_ADD_SINK(FileSink, path);

		GALAXY_ADD_SINK(ConsoleSink);
		GALAXY_LOG(GALAXY_INFO, "App started.");
	}

	void App::setup_async()
	{
		// Configure threadpool.
		// Use half of available cores minus 2 for audio and main thread.
		const auto system_cores = std::thread::hardware_concurrency();
		if (system_cores < 4)
		{
			GALAXY_LOG(GALAXY_WARN, "Total cores are less than 4, this is not optimal.");
		}

		const auto cores = static_cast<int>(std::floor(system_cores / 2.0) - 2);
		entt::locator<BS::light_thread_pool>::emplace(cores);
	}

	void App::setup_config(std::string_view config_file)
	{
		auto& config = entt::locator<Config>::emplace(config_file);
		Settings::set_config_to_default();
		Settings::set_settings_from_config();
	}

	void App::setup_platform()
	{
		platform::seed_random();

		platform::set_metadata(SDL_PROP_APP_METADATA_NAME_STRING, Settings::title().c_str());
		platform::set_metadata(SDL_PROP_APP_METADATA_VERSION_STRING, Settings::version().c_str());
		platform::set_metadata(SDL_PROP_APP_METADATA_IDENTIFIER_STRING, Settings::identifier().c_str());
		platform::set_metadata(SDL_PROP_APP_METADATA_CREATOR_STRING, Settings::creator().c_str());
		platform::set_metadata(SDL_PROP_APP_METADATA_COPYRIGHT_STRING, Settings::copyright().c_str());
		platform::set_metadata(SDL_PROP_APP_METADATA_URL_STRING, Settings::website().c_str());
		platform::set_metadata(SDL_PROP_APP_METADATA_TYPE_STRING, "game");

		platform::set_hint(SDL_HINT_ALLOW_ALT_TAB_WHILE_GRABBED, "1");
		platform::set_hint(SDL_HINT_ASSERT, "break");
		platform::set_hint(SDL_HINT_AUDIO_CATEGORY, "playback");
		platform::set_hint(SDL_HINT_AUDIO_CHANNELS, "2");
		platform::set_hint(SDL_HINT_AUDIO_INCLUDE_MONITORS, "0");
		platform::set_hint(SDL_HINT_AUTO_UPDATE_JOYSTICKS, "1");
		platform::set_hint(SDL_HINT_AUTO_UPDATE_SENSORS, "1");
		// platform::set_hint(SDL_HINT_DEBUG_LOGGING, "1");
		platform::set_hint(SDL_HINT_ENABLE_SCREEN_KEYBOARD, "0");
		platform::set_hint(SDL_HINT_EVENT_LOGGING, "0");
		platform::set_hint(SDL_HINT_FRAMEBUFFER_ACCELERATION, "opengl");
		platform::set_hint(SDL_HINT_GAMECONTROLLER_SENSOR_FUSION, "1");
		platform::set_hint(SDL_HINT_GPU_DRIVER, "vulkan");
		platform::set_hint(SDL_HINT_HIDAPI_ENUMERATE_ONLY_CONTROLLERS, "1");
		platform::set_hint(SDL_HINT_JOYSTICK_ALLOW_BACKGROUND_EVENTS, "0");
		platform::set_hint(SDL_HINT_JOYSTICK_DIRECTINPUT, "1");
		platform::set_hint(SDL_HINT_TV_REMOTE_AS_JOYSTICK, "1");
		// platform::set_hint(SDL_HINT_LOG_BACKENDS, "1");
		platform::set_hint(SDL_HINT_MOUSE_AUTO_CAPTURE, "0");
		platform::set_hint(SDL_HINT_MOUSE_FOCUS_CLICKTHROUGH, "1");
		platform::set_hint(SDL_HINT_MOUSE_RELATIVE_MODE_CENTER, "0");
		platform::set_hint(SDL_HINT_MOUSE_TOUCH_EVENTS, "1");
		platform::set_hint(SDL_HINT_PEN_MOUSE_EVENTS, "1");
		platform::set_hint(SDL_HINT_PEN_TOUCH_EVENTS, "1");
		platform::set_hint(SDL_HINT_QUIT_ON_LAST_WINDOW_CLOSE, "1");
		platform::set_hint(SDL_HINT_RENDER_DRIVER, "opengl");
		platform::set_hint(SDL_HINT_RENDER_GPU_DEBUG, "0");
		platform::set_hint(SDL_HINT_RENDER_GPU_LOW_POWER, "0");
		platform::set_hint(SDL_HINT_TOUCH_MOUSE_EVENTS, "1");
		platform::set_hint(SDL_HINT_TRACKPAD_IS_TOUCH_ONLY, "0");
		platform::set_hint(SDL_HINT_THREAD_FORCE_REALTIME_TIME_CRITICAL, "0");
		platform::set_hint(SDL_HINT_XINPUT_ENABLED, "1");
		platform::set_hint(SDL_HINT_VIDEO_ALLOW_SCREENSAVER, "0");
		platform::set_hint(SDL_HINT_VIDEO_DUMMY_SAVE_FRAMES, "0");
		platform::set_hint(SDL_HINT_VIDEO_MATCH_EXCLUSIVE_MODE_ON_MOVE, "1");
		platform::set_hint(SDL_HINT_VIDEO_MINIMIZE_ON_FOCUS_LOSS, "0");
		platform::set_hint(SDL_HINT_VIDEO_OFFSCREEN_SAVE_FRAMES, "0");
		platform::set_hint(SDL_HINT_VIDEO_SYNC_WINDOW_OPERATIONS, "0");
		platform::set_hint(SDL_HINT_WINDOW_FRAME_USABLE_WHILE_CURSOR_HIDDEN, "1");
		platform::set_hint(SDL_HINT_WINDOWS_CLOSE_ON_ALT_F4, "1");
		platform::set_hint(SDL_HINT_WINDOWS_ENABLE_MENU_MNEMONICS, "0");
		platform::set_hint(SDL_HINT_WINDOWS_GAMEINPUT, "1");
		platform::set_hint(SDL_HINT_WINDOWS_RAW_KEYBOARD, "0");
		// platform::set_hint(SDL_HINT_IME_IMPLEMENTED_UI, "0");

		const auto vsync = Settings::vsync() ? "1" : "0";
		platform::set_hint(SDL_HINT_RENDER_VSYNC, vsync);

		const auto audio_freq = std::to_string(Settings::audio_freq());
		platform::set_hint(SDL_HINT_AUDIO_FREQUENCY, audio_freq.c_str());

		if (!SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO | SDL_INIT_JOYSTICK | SDL_INIT_GAMEPAD | SDL_INIT_EVENTS | SDL_INIT_HAPTIC | SDL_INIT_SENSOR))
		{
			GALAXY_LOG(GALAXY_FATAL, SDL_GetError());
		}
	}

	void App::setup_fs()
	{
		entt::locator<VirtualFileSystem>::emplace();
	}

	void App::setup_window()
	{
		auto& window = entt::locator<Window>::emplace();

		window.set_icon(Settings::window_icon());
		window.show();
	}

	void App::setup_events()
	{
		entt::locator<entt::dispatcher>::emplace();
	}

	// void App::setup_nuklear()
	//{
	// auto& nui = ServiceLocator<ui::NuklearUI>::make();
	// }

	// void App::setup_loader()
	//{
	//	// entt::locator<Loader>::make();
	// }

	void App::setup_meta()
	{
		auto& sf = entt::locator<SystemFactory>::emplace();
		// auto& ef = entt::locator<meta::EntityFactory>::emplace();

		/*ef.register_component<components::Tag>("Tag");

		em.register_component<components::Animated>("Animated");
		em.register_component<components::Circle>("Circle");
		em.register_component<components::Ellipse>("Ellipse");
		em.register_component<components::Point>("Point");
		em.register_component<components::Polygon>("Polygon");
		em.register_component<components::Polyline>("Polyline");
		em.register_component<components::RigidBody>("RigidBody");
		em.register_component<components::Script>("Script");
		em.register_component<components::Sprite>("Sprite");
		em.register_component<components::Text>("Text");
		em.register_component<components::TileMap>("TileMap");
		em.register_component<components::Transform>("Transform");
		em.register_component<flags::DenySerialization>("DenySerialization");
		em.register_component<flags::Disabled>("Disabled");

		em.register_dependencies<components::Animated, components::Sprite>();
		em.register_dependencies<components::Circle, components::Transform>();
		em.register_dependencies<components::Ellipse, components::Transform>();
		em.register_dependencies<components::Point, components::Transform>();
		em.register_dependencies<components::Polygon, components::Transform>();
		em.register_dependencies<components::Polyline, components::Transform>();
		em.register_dependencies<components::RigidBody, components::Transform>();
		em.register_dependencies<components::Sprite, components::Transform>();
		em.register_dependencies<components::Text, components::Transform>();
		*/
	}

	void App::setup_services()
	{
		// entt::locator<media::SoundEngine>::make(listener_count);
		// entt::locator<media::MusicEngine>::make(listener_count);
		// entt::locator<media::VoiceEngine>::make(listener_count);
		// entt::locator<resource::SoundCache>::make();
		// entt::locator<resource::MusicCache>::make();
		// entt::locator<resource::VoiceCache>::make();
		// entt::locator<resource::VideoCache>::make();
		// entt::locator<resource::Animations>::make();
		// entt::locator<resource::Shaders>::make();
		// entt::locator<resource::Fonts>::make();
		// entt::locator<resource::Textures>::make();
		// entt::locator<resource::Prefabs>::make();
		//	  entt::locator<resource::Scripts>::make();
		entt::locator<World>::emplace();
	}

	void App::setup_scripting()
	{
		auto& lua = entt::locator<sol::state>::emplace();
		lua.open_libraries(sol::lib::base, sol::lib::package, sol::lib::coroutine, sol::lib::string, sol::lib::os, sol::lib::math, sol::lib::table, sol::lib::io, sol::lib::utf8);

		//
		// Add external libraries to Lua.
		// Inject all configured galaxy into Lua.
		// Add engine services to lua.
		//
		Lua::inject();
	}
} // namespace galaxy
