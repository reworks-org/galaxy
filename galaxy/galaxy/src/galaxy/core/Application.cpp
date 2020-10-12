///
/// Application.cpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#include <fmt/format.h>
#include <galaxy/scripting/LuaUtils.hpp>
#include <glad/glad.h>
#include <protostar/graphics/Colour.hpp>
#include <protostar/system/Time.hpp>
#include <qs/core/WindowSettings.hpp>
#include <qs/text/FreeType.hpp>
#include <sol/sol.hpp>

#include "galaxy/core/ServiceLocator.hpp"
#include "galaxy/fs/FileSystem.hpp"

#include "Application.hpp"

///
/// Needed for when using std::chrono.
///
using namespace std::chrono_literals;

///
/// Shorthand for high res clock.
///
using std_chrono_hrc = std::chrono::high_resolution_clock;

///
/// Timepoint.
///
using std_chrono_tp = std::chrono::steady_clock::time_point;

///
/// Duration.
///
using std_chrono_duration = std::chrono::duration<long long, std::nano>;

///
/// Core namespace.
///
namespace galaxy
{
	Application::Application(std::unique_ptr<galaxy::Config>& config)
	    : m_openal {}
	{
		m_delta_time.set(0.0);

		// Seed pseudo-random algorithms.
		std::srand(static_cast<unsigned int>(std::time(nullptr)));

		// Set up all of the difference services.
		// The services are configured based off of the config file.

		// Config reader.
		if (!config)
		{
			PL_LOG(PL_FATAL, "Failed to initialize config.");
		}
		else
		{
			m_config           = std::move(config);
			SL_HANDLE.m_config = m_config.get();
		}

		// FS paths.
		galaxy::FileSystem::s_root     = m_config->get<std::string>("root-path");
		galaxy::FileSystem::s_textures = m_config->get<std::string>("textures-path");
		galaxy::FileSystem::s_shaders  = m_config->get<std::string>("shaders-path");
		galaxy::FileSystem::s_scripts  = m_config->get<std::string>("scripts-path");
		galaxy::FileSystem::s_audio    = m_config->get<std::string>("audio-path");
		galaxy::FileSystem::s_json     = m_config->get<std::string>("json-path");
		galaxy::FileSystem::s_fonts    = m_config->get<std::string>("font-path");
		galaxy::FileSystem::s_saves    = m_config->get<std::string>("save-folder");

		// threadpool
		m_threadpool = std::make_unique<pr::ThreadPool<4>>();
		m_threadpool->start();
		SL_HANDLE.m_threadpool = m_threadpool.get();

		// window
		qs::WindowSettings::s_anti_aliasing   = m_config->get<int>("anti-aliasing");    // 2
		qs::WindowSettings::s_ansio_filtering = m_config->get<int>("ansio-filter");     // 2
		qs::WindowSettings::s_vsync           = m_config->get<bool>("vsync");           // false
		qs::WindowSettings::s_srgb            = m_config->get<bool>("srgb");            // false
		qs::WindowSettings::s_aspect_ratio_x  = m_config->get<int>("aspect-ratio-x");   // -1
		qs::WindowSettings::s_aspect_ratio_y  = m_config->get<int>("aspect-ratio-y");   // -1
		qs::WindowSettings::s_raw_mouse_input = m_config->get<bool>("raw-mouse-input"); // true
		qs::WindowSettings::s_texture_format  = GL_RGBA8;

		m_window           = std::make_unique<qs::Window>();
		SL_HANDLE.m_window = m_window.get();

		if (!m_window->create(
			m_config->get<std::string>("window-name"),
			m_config->get<int>("window-width"),
			m_config->get<int>("window-height")))
		{
			PL_LOG(PL_FATAL, "Failed to create window! Aborting...");
		}
		else
		{
			m_window->request_attention();

			bool cursor = m_config->get<bool>("is-cursor-visible");
			m_window->set_cursor_visibility(cursor);
			if (cursor)
			{
				m_window->set_cursor_icon(m_config->get<std::string>("cursor-image"));
			}

			auto icon_path   = fmt::format("{0}{1}{2}", galaxy::FileSystem::s_root, galaxy::FileSystem::s_textures, m_config->get<std::string>("icon-file"));
			auto cursor_icon = fmt::format("{0}{1}{2}", galaxy::FileSystem::s_root, galaxy::FileSystem::s_textures, m_config->get<std::string>("cursor-image"));
			m_window->set_icon(icon_path);
			m_window->set_cursor_icon(cursor_icon);

			// renderer
			m_renderer           = std::make_unique<qs::Renderer>();
			SL_HANDLE.m_renderer = m_renderer.get();

			// Freetype.
			FTLIB.open();

			// Create lua instance and open libraries.
			m_lua = std::make_unique<sol::state>();
			m_lua->open_libraries(sol::lib::base, sol::lib::package, sol::lib::string, sol::lib::os, sol::lib::math, sol::lib::table, sol::lib::io);
			SL_HANDLE.m_lua = m_lua.get();

			m_state           = std::make_unique<pr::StateMachine>();
			SL_HANDLE.m_state = m_state.get();

			// Event dispatcher.
			m_dispatcher           = std::make_unique<sl::Dispatcher>();
			SL_HANDLE.m_dispatcher = m_dispatcher.get();

			// Game "world".
			m_world           = std::make_unique<galaxy::World>();
			SL_HANDLE.m_world = m_world.get();

			// Serializer.
			//m_serializer           = std::make_unique<galaxy::Serializer>(m_config->get<std::string>("save-folder"));
			//SL_HANDLE.m_serializer = m_serializer.get();

			// FontBook
			m_fontbook           = std::make_unique<galaxy::FontBook>(m_config->get<std::string>("fontbook-json"));
			SL_HANDLE.m_fontbook = m_fontbook.get();

			// ShaderBook
			m_shaderbook           = std::make_unique<galaxy::ShaderBook>(m_config->get<std::string>("shaderbook-json"));
			SL_HANDLE.m_shaderbook = m_shaderbook.get();

			// Register all usertypes used by this application for sol3.
			Lua::register_types();
		}
	}

	Application::~Application()
	{
		// We want to destroy everything in a specific order to make sure stuff is freed correctly.
		// And of course the file system being the last to be destroyed.

		m_world.reset();
		m_dispatcher.reset();
		m_state.reset();
		m_lua.reset();
		m_renderer.reset();
		m_window.reset();
		m_threadpool.reset();
		m_config.reset();
	}

	bool Application::run()
	{
		// This is to ensure gameloop is running at 60 UPS, independant of FPS. 1.0 / 60.0.
		std_chrono_tp current;
		std_chrono_duration elapsed;
		std::chrono::nanoseconds lag(0ns);
		constexpr std::chrono::nanoseconds update_ratio(16ms);

		// Fixed timestep gameloop. Pretty easy to understand.
		// Simply loop the game until the window closes, then the mainloop can handle restarting the application if restart = true.
		auto previous = std_chrono_hrc::now();
		while (m_window->is_open())
		{
			current  = std_chrono_hrc::now();
			elapsed  = current - previous;
			previous = current;
			lag += elapsed;

			m_window->poll_events();
			while (lag >= update_ratio)
			{
				m_delta_time.set(static_cast<double>(lag.count()) / static_cast<double>(update_ratio.count()));

				m_state->events();
				m_state->update(m_delta_time.get());

				lag -= update_ratio;
			}

			// Begin render.
			m_window->begin();

			m_state->render();

			m_window->end();
			// End render.
		}

		m_state->clear();
		PL_LOG_FINISH;

		FTLIB.close();

		m_threadpool->end();
		m_window->destroy();

		return SL_HANDLE.m_restart;
	}

	pr::LockedDouble* Application::get_dt() noexcept
	{
		return &m_delta_time;
	}
} // namespace galaxy