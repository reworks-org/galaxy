///
/// Application.cpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#include <BS_thread_pool.hpp>

#include "galaxy/components/Animated.hpp"
#include "galaxy/components/Circle.hpp"
#include "galaxy/components/Ellipse.hpp"
#include "galaxy/components/Point.hpp"
#include "galaxy/components/Polygon.hpp"
#include "galaxy/components/Polyline.hpp"
#include "galaxy/components/RenderCommand.hpp"
#include "galaxy/components/RigidBody.hpp"
#include "galaxy/components/Script.hpp"
#include "galaxy/components/Sprite.hpp"
#include "galaxy/components/Tag.hpp"
#include "galaxy/components/Text.hpp"
#include "galaxy/components/TileMap.hpp"
#include "galaxy/components/Transform.hpp"
#include "galaxy/core/Config.hpp"
#include "galaxy/core/Loader.hpp"
#include "galaxy/core/ServiceLocator.hpp"
#include "galaxy/core/Window.hpp"
#include "galaxy/error/ConsoleSink.hpp"
#include "galaxy/error/FileSink.hpp"
#include "galaxy/flags/DenySerialization.hpp"
#include "galaxy/flags/Disabled.hpp"
#include "galaxy/fs/VirtualFileSystem.hpp"
#include "galaxy/graphics/Renderer.hpp"
#include "galaxy/graphics/text/FontContext.hpp"
#include "galaxy/input/Input.hpp"
#include "galaxy/media/AudioEngine.hpp"
#include "galaxy/meta/EntityMeta.hpp"
#include "galaxy/platform/Platform.hpp"
#include "galaxy/resource/Animations.hpp"
#include "galaxy/resource/Fonts.hpp"
#include "galaxy/resource/Media.hpp"
#include "galaxy/resource/Prefabs.hpp"
#include "galaxy/resource/Scripts.hpp"
#include "galaxy/resource/Shaders.hpp"
#include "galaxy/resource/Textures.hpp"
#include "galaxy/scene/SceneManager.hpp"
#include "galaxy/scripting/Lua.hpp"
#include "galaxy/ui/ImGuiHelpers.hpp"

#include "Application.hpp"

namespace galaxy
{
	namespace core
	{
		App::App(std::string_view log_dir, std::string_view config_file)
		{
			// Configure platform specific behaviour.
			platform::configure_terminal();

			// Seed pseudo-random algorithms.
			std::srand(static_cast<unsigned int>(std::time(nullptr)));

			//
			// Threadpool.
			//
			// Use half of available cores minus 2 for audio and main thread.
			const auto cores = (std::thread::hardware_concurrency() / 2) - 2;
			ServiceLocator<BS::thread_pool>::make(cores);

			//
			// LOGGING.
			//
			const auto        now      = std::chrono::zoned_time {std::chrono::current_zone(), std::chrono::system_clock::now()}.get_local_time();
			const std::string log_path = std::format("{0}{1}{2}", log_dir, std::format("{0:%d-%m-%Y-[%H-%M]}", now), ".log");
			if (!std::filesystem::exists(log_dir))
			{
				std::filesystem::create_directory(log_dir);
			}

			GALAXY_ADD_SINK(error::FileSink, log_path);
			GALAXY_ADD_SINK(error::ConsoleSink);
			GALAXY_LOG(GALAXY_INFO, "App started.");

			//
			// CONFIG.
			//
			auto& config = ServiceLocator<Config>::make(config_file);
			config.restore<bool>("use_loose_assets", true);
			config.restore<std::string>("default_lang", "en_au");
			config.restore<std::string>("title", "Title", "window");
			config.restore<std::string>("icon", "", "window");
			config.restore<std::string>("cursor_icon", "", "window");
			config.restore<float>("ui_font_size", 14.0f);
			config.restore<bool>("vsync", false, "window");
			config.restore<bool>("maximized", false, "window");
			config.restore<bool>("fullscreen", false, "window");
			config.restore<bool>("debug", true, "window");
			config.restore<bool>("visible_cursor", true, "window");
			config.restore<int>("window_width", 1920, "window");
			config.restore<int>("window_height", 1080, "window");
			config.restore<int>("frame_width", 640, "window");
			config.restore<int>("frame_height", 360, "window");
			config.restore<float>("sfx_volume", 1.0f, "audio");
			config.restore<float>("music_volume", 1.0f, "audio");
			config.restore<float>("dialogue_volume", 1.0f, "audio");
			config.restore<int>("listener_count", 1, "audio");
			config.restore<bool>("trilinear_filtering", false, "graphics");
			config.restore<int>("ansiotrophic_filtering", 2, "graphics");
			config.restore<bool>("smaa", false, "graphics.effects");
			config.restore<bool>("sharpen", false, "graphics.effects");
			config.restore<bool>("gamma_correction", false, "graphics.effects");
			config.restore<bool>("chromatic_abberation", false, "graphics.effects");
			config.restore<bool>("gaussian_blur", false, "graphics.effects");
			config.restore<bool>("film_grain", false, "graphics.effects");
			config.restore<int>("camera_foward", input::key_to_int(input::Keys::KEY_W), "input");
			config.restore<int>("camera_backward", input::key_to_int(input::Keys::KEY_S), "input");
			config.restore<int>("camera_left", input::key_to_int(input::Keys::KEY_A), "input");
			config.restore<int>("camera_right", input::key_to_int(input::Keys::KEY_D), "input");
			config.restore<int>("camera_rotate_left", input::key_to_int(input::Keys::KEY_Q), "input");
			config.restore<int>("camera_rotate_right", input::key_to_int(input::Keys::KEY_E), "input");

			config.save();

			//
			// VIRTUAL FILE SYSTEM.
			//
			auto& fs = ServiceLocator<fs::VirtualFileSystem>::make();

			// Generate default language file.
			if (!fs.exists("en_au.lang"))
			{
				if (!fs.write("region='en_au'\ndata={}", "lang/en_au.lang"))
				{
					GALAXY_LOG(GALAXY_FATAL, "Failed to save default language file.");
				}
			}

			//
			// Window Settings.
			// clang-format off
			//
			const auto window_settings = WindowSettings
			{
				.title = config.get<std::string>("title", "window"),
				.window_width = config.get<int>("window_width", "window"),
				.window_height = config.get<int>("window_height", "window"),
				.frame_width = config.get<int>("frame_width", "window"),
				.frame_height = config.get<int>("frame_height", "window"),
				.vsync = config.get<bool>("vsync", "window"),
				.maximized = config.get<bool>("maximized", "window"),
				.fullscreen = config.get<bool>("fullscreen", "window"),
				.debug = config.get<bool>("debug", "window")
			};
			// clang-format on

			//
			// Construct window and init glfw and OpenGL.
			//
			auto& window = ServiceLocator<Window>::make(window_settings);

			///
			/// Renderer init.
			///
			graphics::Renderer::ref().init();

			///
			/// Loader.
			///
			ServiceLocator<Loader>::make();

			//
			// Set up Font Context.
			//
			ServiceLocator<graphics::FontContext>::make();

			//
			// Set up entity metadata.
			//
			auto& em = ServiceLocator<meta::EntityMeta>::make();
			em.register_component<components::Animated>("Animated");
			em.register_component<components::Circle>("Circle");
			em.register_component<components::Ellipse>("Ellipse");
			em.register_component<components::Point>("Point");
			em.register_component<components::Polygon>("Polygon");
			em.register_component<components::Polyline>("Polyline");
			em.register_component<components::RenderCommand>("RenderCommand");
			em.register_component<components::RigidBody>("RigidBody");
			em.register_component<components::Script>("Script");
			em.register_component<components::Sprite>("Sprite");
			em.register_component<components::Tag>("Tag");
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

			//
			// Initialize Lua.
			//
			auto& lua = ServiceLocator<sol::state>::make();
			lua.open_libraries(sol::lib::base,
				sol::lib::package,
				sol::lib::coroutine,
				sol::lib::string,
				sol::lib::os,
				sol::lib::math,
				sol::lib::table,
				sol::lib::io,
				sol::lib::utf8);

			//
			// Services.
			//
			const auto listener_count = config.get<int>("listener_count", "audio");
			ServiceLocator<media::SoundEngine>::make(listener_count);
			ServiceLocator<media::MusicEngine>::make(listener_count);
			ServiceLocator<media::VoiceEngine>::make(listener_count);
			ServiceLocator<resource::SoundCache>::make();
			ServiceLocator<resource::MusicCache>::make();
			ServiceLocator<resource::VoiceCache>::make();
			ServiceLocator<resource::VideoCache>::make();
			ServiceLocator<resource::Animations>::make();
			ServiceLocator<resource::Shaders>::make();
			ServiceLocator<resource::Fonts>::make();
			ServiceLocator<resource::Textures>::make();
			ServiceLocator<resource::Prefabs>::make();
			ServiceLocator<resource::Scripts>::make();
			ServiceLocator<scene::SceneManager>::make();

			//
			// Add external libraries to Lua.
			// Inject all configured galaxy into Lua.
			// Add engine services to lua.
			//
			lua::inject();

			// Load game assets.
			core::ServiceLocator<core::Loader>::ref().load_all();
		}

		App::~App()
		{
			ServiceLocator<scene::SceneManager>::del();
			ServiceLocator<resource::Scripts>::del();
			ServiceLocator<resource::Prefabs>::del();
			ServiceLocator<resource::Textures>::del();
			ServiceLocator<resource::Fonts>::del();
			ServiceLocator<resource::Shaders>::del();
			ServiceLocator<resource::Animations>::del();
			ServiceLocator<resource::VideoCache>::del();
			ServiceLocator<resource::VoiceCache>::del();
			ServiceLocator<resource::MusicCache>::del();
			ServiceLocator<resource::SoundCache>::del();
			ServiceLocator<media::VoiceEngine>::del();
			ServiceLocator<media::MusicEngine>::del();
			ServiceLocator<media::SoundEngine>::del();
			ServiceLocator<sol::state>::del();
			ServiceLocator<meta::EntityMeta>::del();
			ServiceLocator<graphics::FontContext>::del();
			ServiceLocator<Loader>::del();
			graphics::Renderer::ref().destroy();
			ServiceLocator<Window>::del();
			ServiceLocator<fs::VirtualFileSystem>::del();
			ServiceLocator<Config>::del();

			GALAXY_LOG(GALAXY_INFO, "Application closed.");
			GALAXY_LOG_FINISH();

			ServiceLocator<BS::thread_pool>::ref().wait();
			ServiceLocator<BS::thread_pool>::del();
		}

		void App::load()
		{
			const auto path = GALAXY_ROOT_DIR / GALAXY_APPDATA;

			auto& sm = core::ServiceLocator<scene::SceneManager>::ref();
			sm.load_app(path.string());
		}

		void App::run()
		{
			GALAXY_CUR_UPS = 0;
			GALAXY_CUR_FPS = 0;

			auto& window  = ServiceLocator<Window>::ref();
			auto& manager = ServiceLocator<scene::SceneManager>::ref();

			unsigned int frames  = 0u;
			unsigned int updates = 0u;

			constexpr const auto ups_as_nano = std::chrono::duration_cast<std::chrono::nanoseconds>(GALAXY_UPS);
			constexpr const auto one_second  = std::chrono::seconds {1};

			std::chrono::nanoseconds accumulator {0};
			std::chrono::nanoseconds perf_counter {0};
			std::chrono::nanoseconds elapsed {0};

			auto current  = std::chrono::high_resolution_clock::now();
			auto previous = current;

			while (window.is_open())
			{
				current      = std::chrono::high_resolution_clock::now();
				elapsed      = current - previous;
				previous     = current;
				accumulator += elapsed;

				perf_counter += elapsed;

				while (accumulator >= GALAXY_UPS)
				{
					window.poll_events();
					manager.update();

					accumulator -= ups_as_nano;
					updates++;
				}

				manager.render();

				frames++;

				if (perf_counter >= one_second)
				{
					GALAXY_CUR_UPS = updates;
					GALAXY_CUR_FPS = frames;
					window.append_title(std::format(" | UPS: {0}, FPS: {1}", updates, frames));

					frames       = 0;
					updates      = 0;
					perf_counter = std::chrono::nanoseconds {0};
				}
			}
		}
	} // namespace core
} // namespace galaxy
