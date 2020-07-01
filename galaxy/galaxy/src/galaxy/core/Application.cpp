///
/// Application.cpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#include <glad/glad.h>
#include <sol/sol.hpp>
#include <qs/text/FreeType.hpp>
#include <protostar/system/Time.hpp>
#include <qs/core/WindowSettings.hpp>
#include <protostar/system/Colour.hpp>
#include <galaxy/scripting/LuaUtils.hpp>

#include "galaxy/fs/FileSystem.hpp"
#include "galaxy/core/ServiceLocator.hpp"

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
	{
		m_timeCorrection.set(0);

		// Seed pseudo-random algorithms.
		std::srand(static_cast<unsigned int>(std::time(nullptr)));

		// Supposed to improve performance. Need to run tests and ensure we aren't using C stdio.
		std::ios::sync_with_stdio(false);
		
		// Logging.
		std::string lf = "logs/" + protostar::getFormattedTime() + ".txt";
		PL_LOG_GET.init(lf);
		PL_LOG_GET.setMinimumLevel(PL_INFO);

		// Set up all of the difference services.
		// The services are configured based off of the config file.

		// Config reader.
		m_config = std::move(config);
		SL_HANDLE.m_config = m_config.get();

		// FS paths.
		galaxy::FileSystem::s_root = m_config->get<std::string>("root-path");
		galaxy::FileSystem::s_textures = m_config->get<std::string>("textures-path");
		galaxy::FileSystem::s_shaders = m_config->get<std::string>("shaders-path");
		galaxy::FileSystem::s_scripts = m_config->get<std::string>("scripts-path");
		galaxy::FileSystem::s_music = m_config->get<std::string>("music-path");
		galaxy::FileSystem::s_sfx = m_config->get<std::string>("sfx-path");
		galaxy::FileSystem::s_json = m_config->get<std::string>("json-path");
		galaxy::FileSystem::s_fonts = m_config->get<std::string>("font-path");

		// threadpool
		m_threadPool = std::make_unique<protostar::ThreadPool>();
		m_threadPool->create(m_config->get<int>("threadpool-threadcount"));
		m_threadPool->setActive(true);
		SL_HANDLE.m_threadPool = m_threadPool.get();

		// window
		qs::WindowSettings::s_antiAliasing = m_config->get<int>("anti-aliasing"); // 2
		qs::WindowSettings::s_ansiotropicFiltering = m_config->get<int>("ansio-filter"); // 2
		qs::WindowSettings::s_vsync = m_config->get<bool>("vsync"); // false
		qs::WindowSettings::s_srgb = m_config->get<bool>("srgb"); // false
		qs::WindowSettings::s_aspectRatioX = m_config->get<int>("aspect-ratio-x"); // -1
		qs::WindowSettings::s_aspectRatioY = m_config->get<int>("aspect-ratio-y"); // -1
		qs::WindowSettings::s_rawMouseInput = m_config->get<bool>("raw-mouse-input"); // true
		qs::WindowSettings::s_textureFormat = GL_RGBA8;

		m_window = std::make_unique<qs::Window>();
		SL_HANDLE.m_window = m_window.get();

		if (!m_window->create(
			m_config->get<std::string>("window-name"),
			m_config->get<int>("window-width"),
			m_config->get<int>("window-height")
		))
		{
			PL_LOG(PL_FATAL, "Failed to create window! Aborting...");
		}
		else
		{
			m_window->requestAttention();
			
			bool cursor = m_config->get<bool>("is-cursor-visible");
			m_window->setCursorVisibility(cursor);
			if (cursor)
			{
				m_window->setCursorIcon(m_config->get<std::string>("cursor-image"));
			}

			m_window->setIcon(m_config->get<std::string>("icon-file"));
			//m_window->setKeyRepeatEnabled(m_config->get<bool>("key-repeat"));
			//m_window->setMouseCursorGrabbed(false);

			// renderer
			m_renderer = std::make_unique<qs::Renderer>();
			SL_HANDLE.m_renderer = m_renderer.get();

			// OpenAl.
			m_alContext.initialize();

			// Create lua instance and open libraries.
			m_lua = std::make_unique<sol::state>();
			m_lua->open_libraries(sol::lib::base, sol::lib::package, sol::lib::string, sol::lib::os, sol::lib::math, sol::lib::table, sol::lib::io);
			SL_HANDLE.m_lua = m_lua.get();

			m_state = std::make_unique<protostar::StateMachine>();
			SL_HANDLE.m_state = m_state.get();

			// Event dispatcher.
			m_dispatcher = std::make_unique<starlight::Dispatcher>();
			SL_HANDLE.m_dispatcher = m_dispatcher.get();

			// Game "world".
			m_world = std::make_unique<galaxy::World>();
			SL_HANDLE.m_world = m_world.get();

			// Serializer.
			m_serializer = std::make_unique<galaxy::Serializer>(m_config->get<std::string>("save-folder"));
			SL_HANDLE.m_serializer = m_serializer.get();

			// FontBook
			std::string fbPath = galaxy::FileSystem::s_root + galaxy::FileSystem::s_json + m_config->get<std::string>("fontbook-json");
			m_fontbook = std::make_unique<galaxy::FontBook>(fbPath);
			SL_HANDLE.m_fontbook = m_fontbook.get();

			// ShaderBook
			std::string sbPath = galaxy::FileSystem::s_root + galaxy::FileSystem::s_json + m_config->get<std::string>("shaderbook-json");
			m_shaderbook = std::make_unique<galaxy::ShaderBook>(sbPath);
			SL_HANDLE.m_shaderbook = m_shaderbook.get();

			// END SERVICES

			//m_textureAtlas = std::make_unique<TextureAtlas>(m_world->m_textureFolderPath, m_configReader->lookup<int>(config, "graphics", "atlasPowerOf"));
			//ServiceLocator::textureAtlas = m_textureAtlas.get();

			//m_textureAtlas->m_nullTexture = m_configReader->lookup<std::string>(config, "graphics", "nullTexture");

			//m_musicPlayer = std::make_unique<MusicPlayer>(m_configReader->lookup<std::string>(config, "audio", "musicScript"));
			//ServiceLocator::musicPlayer = m_musicPlayer.get();

			//m_musicPlayer->m_nullMusic = m_configReader->lookup<std::string>(config, "audio", "nullMusic");

			//m_soundPlayer = std::make_unique<SoundPlayer>(m_configReader->lookup<std::string>(config, "audio", "soundScript"));
			//ServiceLocator::soundPlayer = m_soundPlayer.get();

			//m_soundPlayer->m_nullSound = m_configReader->lookup<std::string>(config, "audio", "nullSound");

			//m_box2dHelper = std::make_unique<Box2DHelper>(m_configReader->lookup<float32>(config, "box2d", "gravity"));
			//ServiceLocator::box2dHelper = m_box2dHelper.get();

			//m_box2dHelper->m_b2world->SetContactListener(&m_engineCallbacks);

			// Register all usertypes used by this application for sol3.
			Lua::registerTypes();
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
		m_threadPool.reset();
		m_config.reset();

		PL_LOG_GET.deinit();
	}

	bool Application::run()
	{
		// This is to ensure gameloop is running at 60 UPS, independant of FPS. 1.0 / 60.0.
		std_chrono_tp current;
		std_chrono_duration elapsed;
		std::chrono::nanoseconds lag(0ns);
		constexpr std::chrono::nanoseconds updateRatio(16ms);

		// Fixed timestep gameloop. Pretty easy to understand.
		// Simply loop the game until the window closes, then the mainloop can handle restarting the application if restart = true.
		auto previous = std_chrono_hrc::now();
		while (m_window->isOpen())
		{
			current = std_chrono_hrc::now();
			elapsed = current - previous;
			previous = current;
			lag += elapsed;

			m_window->pollEvents();
			while (lag >= updateRatio)
			{
				m_timeCorrection.set(static_cast<double>(lag.count()) / static_cast<double>(updateRatio.count()));

				m_state->events();
				m_state->update(&m_timeCorrection);

				lag -= updateRatio;
			}
			
			// Begin render.
			m_window->begin({ 0, 0, 0, 255 });
			
			m_state->render();

			m_window->end();
			// End render.
		}

		m_state->clear();
		m_threadPool->setActive(false);
		m_threadPool->destroy();

		FTLIB.close();
		m_window->destroy();

		PL_LOG_GET.deinit();

		return SL_HANDLE.m_restart;
	}
}