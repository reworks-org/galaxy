///
/// Application.cpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#define BS_THREAD_POOL_NATIVE_EXTENSIONS

#include <format>
#include <filesystem>

#include <BS_thread_pool.hpp>
#include <entt/locator/locator.hpp>
#include <entt/signal/dispatcher.hpp>
#include <mimalloc.h>
#include <Raylib.hpp>
#include <sol/sol.hpp>

#include "galaxy/core/Config.hpp"
#include "galaxy/core/Settings.hpp"
#include "galaxy/fs/VirtualFileSystem.hpp"
#include "galaxy/logging/ConsoleSink.hpp"
#include "galaxy/logging/FileSink.hpp"
#include "galaxy/logging/Log.hpp"
#include "galaxy/lua/Lua.hpp"
#include "galaxy/platform/Platform.hpp"
#include "galaxy/time/Time.hpp"

#include "Application.hpp"

using namespace std::chrono_literals;

namespace galaxy
{
	App::App(const std::string& config_file)
	{
		setup_async();
		setup_logging();
		setup_config(config_file);
		setup_platform();
		setup_fs();
		setup_rendering();
		// setup_input();
		//  setup_nuklear();
		//  setup_loader();
		// setup_meta();
		// setup_services();
		setup_scripting();

		// Load game assets.
		// core::entt::locator<core::Loader>::ref().load_all();
	}

	App::~App()
	{
		// entt::locator<VirtualFileSystem>::reset();
		// entt::locator<Config>::reset();

		// GALAXY_LOG(GALAXY_INFO, "Application closed.");
		// entt::locator<BS::priority_thread_pool>::value().wait();

		// entt::locator<Log>::reset();
		// entt::locator<BS::priority_thread_pool>::reset();

		ray::CloseWindow();
	}

	void App::load()
	{
		// const auto path = Settings::root_dir() / Settings::asset_pack();

		// auto& sm = entt::locator<scene::SceneManager>::value();
		// sm.load_app(path.string());
	}

	void App::run()
	{
		// https://stackoverflow.com/a/59446610
		// We dont need 't' or 'alpha/render' sections.

		// auto& window = entt::locator<Window>::value();
		//  auto& scenes = entt::locator<SceneManager>::value();

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

		if (!m_update)
		{
			m_update = [&](App* app) {
				ray::PollInputEvents();
				// scenes.update();
			};
		}

		if (!m_render)
		{
			m_render = [&](App* app) {
				ray::BeginDrawing();
				ray::ClearBackground(RAY_WHITE);

				// scenes.render();
				// window.swap();
				ray::EndDrawing();
				ray::SwapScreenBuffer();
			};
		}

		while (!ray::WindowShouldClose())
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

				m_update(this);

				updates++;
			}

			m_render(this);

			frames++;

			if (perf >= 1s)
			{
				ray::SetWindowTitle(std::format(" | UPS: {0}, FPS: {1}", updates, frames).c_str());

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

	void App::setup_async()
	{
		// Configure threadpool.

		// Calc threads.
		// We optimize for 6: 1 for audio, 1 for main, 4 for tasks.
		auto system_cores = std::thread::hardware_concurrency();
		if (system_cores < 6)
		{
			system_cores = std::thread::hardware_concurrency();
		}

		// Check for highest available priority.
		BS::set_os_process_priority(BS::os_process_priority::high);
		entt::locator<BS::priority_thread_pool>::emplace(system_cores, [](const std::size_t idx) {
			BS::this_thread::set_os_thread_priority(BS::os_thread_priority::highest);
		});
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

	void App::setup_config(std::string_view config_file)
	{
		auto& config = entt::locator<Config>::emplace(config_file);
		Settings::set_config_to_default();
		Settings::set_settings_from_config();
	}

	void App::setup_platform()
	{
		platform::seed_random();
	}

	void App::setup_fs()
	{
		entt::locator<VirtualFileSystem>::emplace();
	}

	void App::setup_rendering()
	{
		if (Settings::vsync())
		{
			ray::SetConfigFlags(ray::FLAG_VSYNC_HINT);
		}

		if (Settings::window_resizable())
		{
			ray::SetConfigFlags(ray::FLAG_WINDOW_RESIZABLE);
		}

		if (Settings::fullscreen())
		{
			ray::SetConfigFlags(ray::FLAG_FULLSCREEN_MODE);
		}

		if (Settings::borderless_fullscreen())
		{
			ray::SetConfigFlags(ray::FLAG_BORDERLESS_WINDOWED_MODE);
		}

		if (!Settings::decoration())
		{
			ray::SetConfigFlags(ray::FLAG_WINDOW_UNDECORATED);
		}

		if (Settings::ontop())
		{
			ray::SetConfigFlags(ray::FLAG_WINDOW_TOPMOST);
		}

		ray::SetConfigFlags(ray::FLAG_WINDOW_HIGHDPI);
		ray::SetConfigFlags(ray::FLAG_WINDOW_HIDDEN);
		ray::SetConfigFlags(ray::FLAG_WINDOW_TRANSPARENT);
		ray::InitWindow(Settings::window_width(), Settings::window_height(), Settings::title().c_str());
		ray::SetExitKey(ray::KEY_NULL);

		if (Settings::maximized())
		{
			ray::SetWindowState(ray::FLAG_WINDOW_MAXIMIZED);
		}

		if (Settings::minimized())
		{
			ray::SetWindowState(ray::FLAG_WINDOW_MINIMIZED);
		}

		if (!Settings::window_icon().empty())
		{
			auto& fs = entt::locator<VirtualFileSystem>::value();

			const auto image = fs.load_ray_image(Settings::window_icon());
			if (image.has_value())
			{
				ray::SetWindowIcon(image.value());
				ray::UnloadImage(image.value());
			}
		}

		ray::ClearWindowState(ray::FLAG_WINDOW_HIDDEN);
		ray::SetWindowFocused();

		//	FLAG_WINDOW_UNFOCUSED         = 0x00000800, // Set to window non focused
		//	FLAG_WINDOW_ALWAYS_RUN        = 0x00000100, // Set to allow windows running while minimized
		//	FLAG_WINDOW_MOUSE_PASSTHROUGH = 0x00004000, // Set to support mouse passthrough, only supported when FLAG_WINDOW_UNDECORATED
		//	FLAG_MSAA_4X_HINT             = 0x00000020, // Set to try enabling MSAA 4X
		//	FLAG_INTERLACED_HINT          = 0x00010000  // Set to try enabling interlaced video format (for V3D)
	}

	void App::setup_input()
	{
		if (Settings::cursor_locked())
		{
			ray::DisableCursor();
		}
		else
		{
			ray::EnableCursor();
		}

		if (Settings::cursor_show())
		{
			ray::ShowCursor();
		}
		else
		{
			ray::HideCursor();
		}
	}

	void App::setup_nuklear()
	{
		// auto& nui = ServiceLocator<ui::NuklearUI>::make();
	}

	void App::setup_loader()
	{
		// entt::locator<Loader>::make();
	}

	void App::setup_meta()
	{
		// auto& sf = entt::locator<SystemFactory>::emplace();
		//  auto& ef = entt::locator<meta::EntityFactory>::emplace();

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
		// entt::locator<SceneManager>::emplace();
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
