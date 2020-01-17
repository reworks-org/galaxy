///
/// Application.cpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#include <sol/sol.hpp>
#include <nlohmann/json.hpp>
#include <SFML/Window/Event.hpp>

#include "galaxy/fs/FileSystem.hpp"
#include "galaxy/core/ServiceLocator.hpp"
#include "galaxy/fs/PhysfsInputStream.hpp"

#include "Application.hpp"

///
/// Core namespace.
///
namespace galaxy
{
	Application::Application(std::unique_ptr<galaxy::Config>& config)
	:m_lua(nullptr), m_config(nullptr), m_window(nullptr), m_restart(false)
	{
		// Seed pseudo-random algorithms.
		std::srand(std::time(nullptr));

		// Supposed to improve performance. Need to run tests and ensure we aren't using C stdio.
		std::ios::sync_with_stdio(false);

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
		m_lua->open_libraries(sol::lib::base, sol::lib::math, sol::lib::os, sol::lib::package, sol::lib::string, sol::lib::table, sol::lib::utf8, sol::lib::io);

		// Config reader.
		m_config = std::move(config);
		galaxy::ServiceLocator::i().m_config = m_config.get();

		m_fs = std::make_unique<galaxy::FileSystem>();
		m_fs->mount(m_config->get<std::string>("archive"));
		m_fs->setWriteDir(m_config->get<std::string>("write-dir"));
		galaxy::ServiceLocator::i().m_fs = m_fs.get();

		m_window = std::make_unique<sf::RenderWindow>();
		m_window->create(sf::VideoMode(m_config->get<unsigned int>("width"),
			m_config->get<unsigned int>("height")),
			m_config->get<std::string>("title"),
			sf::Style::Titlebar | sf::Style::Close,
			sf::ContextSettings(0, 0, m_config->get<unsigned int>("anti-alias")));
		
		m_window->requestFocus();
		m_window->setActive();
		m_window->setFramerateLimit(0);
		m_window->setVerticalSyncEnabled(m_config->get<bool>("vsync"));

		sf::Image icon;
		icon.loadFromStream(galaxy::PhysfsInputStream(m_config->get<std::string>("icon")));
		m_window->setIcon(icon.getSize().x, icon.getSize().y, icon.getPixelsPtr());
		
		m_window->setKeyRepeatEnabled(m_config->get<bool>("key-repeat"));
		m_window->setMouseCursorGrabbed(false);
		m_window->setMouseCursorVisible(m_config->get<bool>("mouse-cursor-visible"));
		m_window->setVisible(true);
		galaxy::ServiceLocator::i().m_window = m_window.get();

		m_world = std::make_unique<World>();
		galaxy::ServiceLocator::i().m_world = m_world.get();

		m_editor = std::make_unique<galaxy::Editor>();
		m_editor->init(m_window.get());
		galaxy::ServiceLocator::i().m_editor = m_editor.get();

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
		// We want to destroy everything in a specific order to make sure stuff is freed correctly.
		// And of course the file system being the last to be destroyed.
		m_editor.reset();
		m_world.reset();
		m_window.reset();
		m_fs.reset();
		m_config.reset();
		m_lua.reset();
	}

	bool Application::run()
	{
		// This is to measure UPS and FPS.
		int frames = 0;
		int updates = 0;
		std::uint64_t timer = 0;
		sf::Time timeSinceLastUpdate = sf::Time::Zero;

		// This is to ensure gameloop is running at 60 UPS, independant of FPS.
		const sf::Time timePerFrame = sf::seconds(1.f / 60.f);

		// The timer in milliseconds for UPS and FPS.
		timer = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();

		// load the first state
		//m_stateMachine->load();

		// Fixed timestep gameloop. Pretty easy to understand.
		// Simply loop the game until the window closes, then the mainloop can handle restarting the application if restart = true.
		
		sf::Clock clock;
		while (m_window->isOpen())
		{
			sf::Time dt = clock.restart();
			timeSinceLastUpdate += dt;

			while (timeSinceLastUpdate > timePerFrame)
			{
				timeSinceLastUpdate -= timePerFrame;

				sf::Event event;
				while (m_window->pollEvent(event))
				{
					//m_stateMachine->event(&event);
					//m_debugInterface->event(&event);
					m_editor->event(event);

					if (event.type == sf::Event::Closed)
					{
						m_window->close();
					}

					if (event.type == sf::Event::KeyPressed)
					{
						if (event.key.code == sf::Keyboard::Space)
						{
							m_window->close();
						}
					}
				}

				//m_stateMachine->update(timeSinceLastUpdate);
				//m_world->update(timeSinceLastUpdate);
				
				updates++;
			}

			m_editor->update(timeSinceLastUpdate);

			// We need to "display" the debug ui before the renderer stuff is called.
			// Because this sets up all the textures, api calls, etc.
			m_editor->display(&m_restart);

			m_window->clear(sf::Color::Green);
			
			//m_stateMachine->render();
			//m_debugInterface->render();
			m_editor->render(); 
			
			m_window->display();

			frames++;
			if ((std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count() - timer) > 1000)
			{
				timer += 1000;
				PL_LOG(pl::Log::Level::INFO, std::to_string(updates) + " ups, " + std::to_string(frames) + " fps.");

				updates = 0;
				frames = 0;
			}
		}

		// unload the last state
		//m_stateMachine->unload();

		return m_restart;
	}
}