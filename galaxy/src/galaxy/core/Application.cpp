///
/// Application.cpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#include <BS_thread_pool.hpp>
#include <entt/locator/locator.hpp>
#include <entt/signal/dispatcher.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <sol/sol.hpp>

#include "galaxy/components/Tag.hpp"
#include "galaxy/core/Config.hpp"
#include "galaxy/core/Settings.hpp"
#include "galaxy/fs/VirtualFileSystem.hpp"
#include "galaxy/logging/FileSink.hpp"
#include "galaxy/logging/ConsoleSink.hpp"
#include "galaxy/meta/EntityFactory.hpp"
#include "galaxy/meta/SystemFactory.hpp"
#include "galaxy/platform/Platform.hpp"
#include "galaxy/scene/SceneManager.hpp"
#include "galaxy/scripting/Lua.hpp"

#include "Application.hpp"

namespace galaxy
{
	namespace core
	{
		App::App(std::string_view log_dir, std::string_view config_file)
		{
			setup_platform();
			setup_logging(log_dir);
			setup_async();
			setup_config(config_file);
			setup_fs();
			setup_window();
			setup_events();
			setup_nuklear();
			setup_loader();
			setup_meta();
			setup_scripting();
			setup_services();

			//
			// Add external libraries to Lua.
			// Inject all configured galaxy into Lua.
			// Add engine services to lua.
			//
			lua::inject();

			//// Load game assets.
			// core::entt::locator<core::Loader>::ref().load_all();
		}

		App::~App()
		{
			entt::locator<scene::SceneManager>::reset();
			entt::locator<sol::state>::reset();
			entt::locator<meta::EntityFactory>::reset();
			entt::locator<meta::SystemFactory>::reset();
			entt::locator<entt::dispatcher>::reset();
			entt::locator<sf::RenderWindow>::reset();
			entt::locator<fs::VirtualFileSystem>::reset();
			entt::locator<Config>::reset();
			entt::locator<BS::light_thread_pool>::reset();

			GALAXY_LOG(GALAXY_INFO, "Application closed.");
			entt::locator<logging::Log>::reset();
		}

		void App::load()
		{
			const auto path = settings::root_dir() / settings::asset_pack();

			auto& sm = entt::locator<scene::SceneManager>::value();
			sm.load_app(path.string());
		}

		void App::run()
		{
			// https://stackoverflow.com/a/59446610

			auto& window     = entt::locator<sf::RenderWindow>::value();
			auto& dispatcher = entt::locator<entt::dispatcher>::value();
			auto& manager    = entt::locator<scene::SceneManager>::value();

			using clock      = std::chrono::high_resolution_clock;
			using duration   = std::chrono::duration<double>;
			using time_point = std::chrono::time_point<clock, duration>;

			constexpr const auto one_second = std::chrono::seconds {1};

			// Number of updates per second.
			// Same as std::ratio<1, ups>, using 1000 with std::milli, etc.
			const auto dt = one_second / settings::ups();

			duration accum {0};
			duration elapsed {0};
			double   alpha {0.0};

			time_point now      = clock::now();
			time_point previous = now;

			unsigned int frames  = 0u;
			unsigned int updates = 0u;
			duration     perf_counter {0};

			while (window.isOpen())
			{
				now      = clock::now();
				elapsed  = now - previous;
				previous = now;

				accum        += elapsed;
				perf_counter += elapsed;

				while (accum >= dt)
				{
					accum -= dt;
					alpha  = dt / one_second;
					settings::set_delta_time(alpha);

					// nui.begin_input();
					handle_events(window);
					// nui.end_input();
					manager.update();

					updates++;
				}

				window.clear(sf::Color::White);
				manager.render();
				window.display();

				frames++;

				if (perf_counter >= one_second)
				{
					window.setTitle(std::format("{0} | UPS: {1}, FPS: {2}", settings::window_title(), updates, frames));

					frames       = 0;
					updates      = 0;
					perf_counter = std::chrono::nanoseconds {0};
				}
			}
		}

		void App::setup_platform()
		{
			// Configure platform specific behaviour.
			platform::configure_terminal();

			// Seed pseudo-random algorithms.
			std::srand(static_cast<unsigned int>(std::time(nullptr)));
		}

		void App::setup_logging(std::string_view log_dir)
		{
			// Configure logging.
			const auto        now      = std::chrono::zoned_time {std::chrono::current_zone(), std::chrono::system_clock::now()}.get_local_time();
			const std::string log_path = std::format("{0}{1}{2}", log_dir, std::format("{0:%d-%m-%Y-[%H-%M]}", now), ".log");
			if (!std::filesystem::exists(log_dir))
			{
				std::filesystem::create_directory(log_dir);
			}

			entt::locator<logging::Log>::emplace();
			GALAXY_ADD_SINK(logging::FileSink, log_path);
			GALAXY_ADD_SINK(logging::ConsoleSink);
			GALAXY_LOG(GALAXY_INFO, "App started.");
		}

		void App::setup_async()
		{
			// Configure threadpool.
			// Use half of available cores minus 2 for audio and main thread.
			auto cores = (std::thread::hardware_concurrency() / 2) - 2;
			if (cores < 4)
			{
				GALAXY_LOG(GALAXY_FATAL, "This app requires a minimum of 4 cores, only {0} were found.", cores);
			}

			entt::locator<BS::light_thread_pool>::emplace(cores);
		}

		void App::setup_config(std::string_view config_file)
		{
			auto& config = entt::locator<Config>::emplace(config_file);
			settings::set_config_to_default(config);
			settings::set_settings_from_config(config);
		}

		void App::setup_fs()
		{
			entt::locator<fs::VirtualFileSystem>::emplace();
		}

		void App::setup_window()
		{
			auto& window = entt::locator<sf::RenderWindow>::emplace();
			window.setVisible(false);

			sf::VideoMode mode;
			mode.bitsPerPixel = sf::VideoMode::getDesktopMode().bitsPerPixel;
			mode.size.x       = settings::window_width();
			mode.size.y       = settings::window_height();

			sf::ContextSettings context;
			context.antiAliasingLevel = settings::msaa();
			context.attributeFlags    = sf::ContextSettings::Default;
			context.depthBits         = 24;
			context.majorVersion      = 3;
			context.minorVersion      = 2;
			context.sRgbCapable       = false;
			context.stencilBits       = 8;

			const auto state = settings::fullscreen() ? sf::State::Fullscreen : sf::State::Windowed;
			window.create(mode, settings::window_title(), sf::Style::Default, state, context);

			if (!settings::window_icon().empty())
			{
				auto& fs   = entt::locator<fs::VirtualFileSystem>::value();
				auto  data = fs.read_binary(settings::window_icon());

				sf::Image image;
				if (image.loadFromMemory(data.data(), data.size()))
				{
					// Copies, so can delete image after.
					window.setIcon(image);
				}
				else
				{
					GALAXY_LOG(GALAXY_ERROR, "Failed to load window icon '{0}'.", settings::window_icon());
				}
			}

			if (!settings::cursor_icon().empty())
			{
				window.setMouseCursorVisible(true);

				if (settings::cursor_icon_size().x == 0 || settings::cursor_icon_size().y == 0)
				{
					GALAXY_LOG(GALAXY_WARNING, "Did not specify cursor size properly, must be same size as texture. Reverting to system default.");
				}
				else
				{
					auto& fs   = entt::locator<fs::VirtualFileSystem>::value();
					auto  data = fs.read_binary(settings::window_icon());

					m_cursor = sf::Cursor::createFromPixels(data.data(), settings::cursor_icon_size(), settings::cursor_hotspot());
					if (m_cursor.has_value())
					{
						window.setMouseCursor(m_cursor.value());
					}
					else
					{
						GALAXY_LOG(GALAXY_ERROR, "Failed to load custom cursor: {0}.", settings::cursor_icon());
					}
				}
			}
			else
			{
				window.setMouseCursorVisible(settings::cursor_visible());
			}

			window.setMouseCursorGrabbed(settings::cursor_grabbed());
			window.setVerticalSyncEnabled(settings::vsync());

			if (!window.setActive(true))
			{
				GALAXY_LOG(GALAXY_FATAL, "Failed to get OpenGL active context.");
			}

			window.setVisible(true);

			if (!window.hasFocus())
			{
				window.requestFocus();
			}
		}

		void App::setup_events()
		{
			entt::locator<entt::dispatcher>::emplace();
			for (auto i = 0; i < sf::Joystick::Count; i++)
			{
				if (sf::Joystick::isConnected(i))
				{
					GALAXY_LOG(GALAXY_WARNING, "Gamepad detected at index {0}. Gamepads are not supported.", i);
				}
			}
		}

		void App::setup_nuklear()
		{
		}

		void App::setup_loader()
		{
			// entt::locator<Loader>::make();
		}

		void App::setup_meta()
		{
			auto& sf = entt::locator<meta::SystemFactory>::emplace();
			auto& ef = entt::locator<meta::EntityFactory>::emplace();

			ef.register_component<components::Tag>("Tag");
			/*
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

		void App::setup_scripting()
		{
			auto& lua = entt::locator<sol::state>::emplace();
			lua.open_libraries(
				sol::lib::base,
				sol::lib::package,
				sol::lib::coroutine,
				sol::lib::string,
				sol::lib::os,
				sol::lib::math,
				sol::lib::table,
				sol::lib::io,
				sol::lib::utf8
			);
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
			// entt::locator<resource::Scripts>::make();
			entt::locator<scene::SceneManager>::emplace();
		}

		void App::handle_events(sf::RenderWindow& window)
		{
			window.handleEvents([](auto&& event) {
				using Event = std::decay_t<decltype(event)>;

				if constexpr (std::is_same_v<Event, sf::Event::Closed>)
				{
					entt::locator<sf::RenderWindow>::value().close();
				}
				else
				{
					entt::locator<entt::dispatcher>::value().trigger(event);
				}
			});
		}
	} // namespace core
} // namespace galaxy
