///
/// Application.cpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#define SOL_NO_LUA_HPP

#include <sol/sol.hpp>
#include <nlohmann/json.hpp>

#include "galaxy/core/ServiceLocator.hpp"

#include "Application.hpp"

///
/// Core namespace.
///
namespace galaxy
{
	Application::Application(const std::string& config)
	:m_restart(false)
	{
		// Seed pseudo-random algorithms.
		//std::srand(std::time(nullptr));

		// Supposed to improve performance. Need to run tests and ensure we aren't using C stdio.
		// std::ios::sync_with_stdio(false);

		// Set up logging and set loguru to throw an exception on fatal errors.
		//std::string lname = "logs/" + Time::getFormattedTime() + ".log";
		//loguru::g_flush_interval_ms = 0;
		//loguru::add_file(lname.c_str(), loguru::Append, loguru::Verbosity_MAX);
		//loguru::set_fatal_handler([](const loguru::Message& message)
			//{
				//std::string msg = std::string(message.preamble) + std::string(message.indentation) + std::string(message.prefix) + std::string(message.message);

			//ALLEGRO_DISPLAY* display = ServiceLocator::window->getDisplay();
			//if (display)
				//	{
				//		al_show_native_message_box(display, "FATAL", "Exception has occured!", msg.c_str(), nullptr, ALLEGRO_MESSAGEBOX_ERROR);
				//	}

			//	throw std::runtime_error(msg);
			//});

		// Set allegro to use loguru.
		//al_register_trace_handler([](const char* message) -> void
			//	{
				//	LOG_S(INFO) << "ALLEGRO TRACE: " << message;
			//});

		// Set allegro to throw loguru errors when an assert is tirggered.
		//al_register_assert_handler([](const char* expr, const char* file, int line, const char* func) -> void
			//{
				//LOG_S(FATAL) << "ALLEGRO ASSERT: EXPR: " << expr << " FILE: " << file << " LINE: " << line << " FUNC: " << func;
			//});

		// Initialize all of allegros systems.
		// We dont use al_init() macro because manual control
		// over when allegro shuts down is needed.
		//al_install_system(ALLEGRO_VERSION_INT, nullptr);
		//al_install_keyboard();
		//al_install_mouse();
		//al_install_audio();

		//al_init_font_addon();
		//al_init_ttf_addon();
		//al_init_image_addon();
		//al_init_video_addon();
		//al_init_acodec_addon();
		//al_init_primitives_addon();
		//al_init_native_dialog_addon();

		// Create queue for game to recieve events.
		//m_queue = al_create_event_queue();

		// Set up all of the difference services.
		// The services are configured based off of the config file.

		// Create lua instance and open libraries.
		m_lua = std::make_unique<sol::state>();
		m_lua->open_libraries(sol::lib::base, sol::lib::math, sol::lib::os, sol::lib::package, sol::lib::string, sol::lib::table, sol::lib::utf8);
		

		// Config reader.
		//m_configReader = std::make_unique<ConfigReader>(config, newConfig);
		//ServiceLocator::configReader = m_configReader.get();


		//m_virtualFS = std::make_unique<VirtualFS>(m_configReader->getSection(config, "archives"));
		//ServiceLocator::virtualFS = m_virtualFS.get();
		//m_virtualFS->setWriteDir(m_configReader->lookup<std::string>(config, "fs", "writeDir"));

		m_window = std::make_unique<sf::RenderWindow>();

		//m_window = std::make_unique<Window>(m_configReader->lookup<std::string>(config, "graphics", "title"), m_configReader->lookup<std::string>(config, "graphics", "icon"), m_configReader->lookup<int>(config, "graphics", "width"), m_configReader->lookup<int>(config, "graphics", "height"),
		//m_configReader->lookup<bool>(config, "graphics", "fullscreen"), m_configReader->lookup<bool>(config, "graphics", "msaa"), m_configReader->lookup<int>(config, "graphics", "msaaValue"), m_configReader->lookup<bool>(config, "graphics", "grabMouse"));
		//ServiceLocator::window = m_window.get();

		//m_world = std::make_unique<World>();
		//ServiceLocator::world = m_world.get();

		//m_world->m_scriptFolderPath = m_configReader->lookup<std::string>(config, "fs", "scriptFolderPath");
		//m_world->m_textureFolderPath = m_configReader->lookup<std::string>(config, "fs", "textureFolderPath");
		//m_world->m_musicFolderPath = m_configReader->lookup<std::string>(config, "fs", "musicFolderPath");
		//m_world->m_soundFolderPath = m_configReader->lookup<std::string>(config, "fs", "soundFolderPath");
		//m_world->m_fontFolderPath = m_configReader->lookup<std::string>(config, "fs", "fontFolderPath");

		//m_debugInterface = std::make_unique<DebugInterface>(m_world->m_scriptFolderPath, m_window->getDisplay());
		//ServiceLocator::debugInterface = m_debugInterface.get();
		

		//m_stateMachine = std::make_unique<StateMachine>();
		//ServiceLocator::stateMachine = m_stateMachine.get();

		//m_textureAtlas = std::make_unique<TextureAtlas>(m_world->m_textureFolderPath, m_configReader->lookup<int>(config, "graphics", "atlasPowerOf"));
		//ServiceLocator::textureAtlas = m_textureAtlas.get();

		//m_textureAtlas->m_nullTexture = m_configReader->lookup<std::string>(config, "graphics", "nullTexture");

		//m_fontBook = std::make_unique<FontBook>(m_configReader->lookup<std::string>(config, "font", "fontScript"));
		//ServiceLocator::fontBook = m_fontBook.get();

		//m_shaderLibrary = std::make_unique<ShaderLibrary>(m_configReader->lookup<std::string>(config, "graphics", "shaderScript"));
		//ServiceLocator::shaderLibrary = m_shaderLibrary.get();

		//m_musicPlayer = std::make_unique<MusicPlayer>(m_configReader->lookup<std::string>(config, "audio", "musicScript"));
		//ServiceLocator::musicPlayer = m_musicPlayer.get();

		//m_musicPlayer->m_nullMusic = m_configReader->lookup<std::string>(config, "audio", "nullMusic");

		//m_soundPlayer = std::make_unique<SoundPlayer>(m_configReader->lookup<std::string>(config, "audio", "soundScript"));
		//ServiceLocator::soundPlayer = m_soundPlayer.get();

		//m_soundPlayer->m_nullSound = m_configReader->lookup<std::string>(config, "audio", "nullSound");

		//m_box2dHelper = std::make_unique<Box2DHelper>(m_configReader->lookup<float32>(config, "box2d", "gravity"));
		//ServiceLocator::box2dHelper = m_box2dHelper.get();

		//m_dispatcher = std::make_unique<entt::Dispatcher>();
		//ServiceLocator::dispatcher = m_dispatcher.get();

		//m_box2dHelper->m_b2world->SetContactListener(&m_engineCallbacks);

		// Register all usertypes used by this application for Sol2.
		//LuaUtils::registergalaxyUsertypes();
	}

	Application::~Application()
	{
		/*
		// Iterate over entities, destroying their Box2D body data.
		// Yeah, no idea why this has to be done here, entt has some really weird behaviour that's causing a number of unexpected issues.
		ServiceLocator::world->m_registry.view<PhysicsComponent>().each([this](entt::DefaultRegistry::entity_type entity, PhysicsComponent& pc) {
			ServiceLocator::box2dHelper->m_b2world->DestroyBody(pc.m_body);
		});

		// We want to destroy everything in a specific order to make sure stuff is freed correctly.
		// It actually only really matters box2d is destroyed after the world because the physics code rely
		// on the box2d system to be destroyed unfortunately.
		// And of course the file system being the last to be destroyed.
		m_dispatcher.reset();
		m_soundPlayer.reset();
		m_musicPlayer.reset();
		m_shaderLibrary.reset();
		m_fontBook.reset();
		m_textureAtlas.reset();
		#ifdef _DEBUG 
			m_debugInterface.reset(); 
		#endif
		m_stateMachine.reset();
		m_box2dHelper.reset();
		m_world.reset();
		m_window.reset();
		m_configReader.reset();
		m_virtualFS.reset();
		m_lua.reset();

		// Clean up the event queue.
		al_destroy_event_queue(m_queue);

		// Clean up allegro aswell.
		al_shutdown_native_dialog_addon();
		al_shutdown_primitives_addon();
		al_shutdown_video_addon();
		al_shutdown_image_addon();
		al_shutdown_ttf_addon();
		al_shutdown_font_addon();

		al_uninstall_audio();
		al_uninstall_mouse();
		al_uninstall_keyboard();
		al_uninstall_system();
	}

	bool Application::run()
	{
		// This is to measure UPS and FPS.
		int frames = 0;
		int updates = 0;
		std::uint64_t timer = 0;
		
		// This is to ensure gameloop is running at 60 UPS, independant of FPS.
		double timePerFrame = 1.0 / 60.0;

		// Set system event sources. User event registration is handled by event manager.
		al_register_event_source(m_queue, al_get_display_event_source(m_window->getDisplay()));
		al_register_event_source(m_queue, al_get_mouse_event_source());
		al_register_event_source(m_queue, al_get_keyboard_event_source());
		
		// Our clock for ensuring gameloop speed.
		ALLEGRO_TIMER* clock = al_create_timer(timePerFrame);
		al_register_event_source(m_queue, al_get_timer_event_source(clock));
		al_start_timer(clock);
		
		// The timer in milliseconds for UPS and FPS.
		timer = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();

		// load the first state
		m_stateMachine->load();

		// Gameloop. Pretty easy to understand.
		// Simply loop the game until the window closes, then the mainloop can handle restarting the application if restart = true.
		while (m_window->isOpen())
		{
			ALLEGRO_EVENT event;
			while (al_get_next_event(m_queue, &event))
			{
				// Events
				m_stateMachine->event(&event);
				#ifdef _DEBUG 
					m_debugInterface->event(&event); 
				#endif

				switch (event.type)
				{
					case ALLEGRO_EVENT_TIMER:
						// Updates
						m_stateMachine->update(timePerFrame);
						m_world->update(timePerFrame);
						updates++;
						break;

					case ALLEGRO_EVENT_DISPLAY_CLOSE:
						m_window->close();
						break;

					case ALLEGRO_EVENT_DISPLAY_RESIZE:
						#ifdef _DEBUG
							ImGui_ImplAllegro5_InvalidateDeviceObjects();
							al_acknowledge_resize(m_window->getDisplay());
							ImGui_ImplAllegro5_CreateDeviceObjects();
						#endif
						break;
				}
			}
			
			// We need to "display" the debug ui before the renderer stuff is called.
			// Because this sets up all the textures, api calls, etc.
			#ifdef _DEBUG
				m_debugInterface->newFrame();
				m_debugInterface->displayMenu(&m_restart);
			#endif
			m_window->clear(0, 0, 0);
			
			m_stateMachine->render();
			#ifdef _DEBUG
				m_debugInterface->render();
			#endif

			m_window->display();

			frames++;
			if ((std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count() - timer) > 1000)
			{
				timer += 1000;
				LOG_S(INFO) << updates << " ups, " << frames << " fps";

				updates = 0;
				frames = 0;
			}
		}

		// unload the last state
		m_stateMachine->unload();

		// Clean up the gameloop timer clock.
		al_stop_timer(clock);
		al_destroy_timer(clock);

		return m_restart;
		*/
	}
}