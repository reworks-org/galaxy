///
/// Application.cpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#include <BS_thread_pool.hpp>

#include "galaxy/components/Animated.hpp"
#include "galaxy/components/MapData.hpp"
#include "galaxy/components/ParticleGenerator.hpp"
#include "galaxy/components/Primitive.hpp"
#include "galaxy/components/RigidBody.hpp"
#include "galaxy/components/Script.hpp"
#include "galaxy/components/Sprite.hpp"
#include "galaxy/components/Tag.hpp"
#include "galaxy/components/Text.hpp"
#include "galaxy/components/Transform.hpp"
#include "galaxy/components/UIScript.hpp"
#include "galaxy/core/Config.hpp"
#include "galaxy/core/Loader.hpp"
#include "galaxy/core/ServiceLocator.hpp"
#include "galaxy/core/Window.hpp"
#include "galaxy/embedded/RobotoLight.hpp"
#include "galaxy/error/ConsoleSink.hpp"
#include "galaxy/error/FileSink.hpp"
#include "galaxy/flags/DenySerialization.hpp"
#include "galaxy/flags/Disabled.hpp"
#include "galaxy/fs/VirtualFileSystem.hpp"
#include "galaxy/graphics/Renderer.hpp"
#include "galaxy/input/Input.hpp"
#include "galaxy/media/AudioEngine.hpp"
#include "galaxy/meta/EntityMeta.hpp"
#include "galaxy/platform/Platform.hpp"
#include "galaxy/resource/Fonts.hpp"
#include "galaxy/resource/Maps.hpp"
#include "galaxy/resource/Media.hpp"
#include "galaxy/resource/Prefabs.hpp"
#include "galaxy/resource/Scripts.hpp"
#include "galaxy/resource/Shaders.hpp"
#include "galaxy/resource/TextureAtlas.hpp"
#include "galaxy/scene/SceneManager.hpp"
#include "galaxy/scripting/Lua.hpp"
#include "galaxy/ui/ImGuiHelpers.hpp"
#include "galaxy/ui/ImGuiTheme.hpp"
#include "galaxy/ui/NuklearUI.hpp"

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
			ServiceLocator<BS::thread_pool>::make(4);

			//
			// LOGGING.
			//
			const auto        now      = std::chrono::zoned_time {std::chrono::current_zone(), std::chrono::system_clock::now()}.get_local_time();
			const std::string log_path = std::format("{0}{1}{2}", log_dir, std::format("{0:%d-%m-%Y-[%H-%M]}", now), ".log");
			if (!std::filesystem::exists(log_dir))
			{
				std::filesystem::create_directory(log_dir);
			}

			GALAXY_LOG_INIT();
			GALAXY_ADD_SINK(error::FileSink, log_path);
			GALAXY_ADD_SINK(error::ConsoleSink);
			GALAXY_LOG(GALAXY_INFO, "App started.");

			//
			// CONFIG.
			//
			auto& config = ServiceLocator<Config>::make(config_file);
			config.restore<bool>("use_loose_assets", true);
			config.restore<bool>("log_performance", true);
			config.restore<std::string>("default_lang", "en_au");
			config.restore<std::string>("title", "Title", "window");
			config.restore<std::string>("icon", "", "window");
			config.restore<std::string>("cursor_icon", "", "window");
			config.restore<float>("ui_font_size", 14.0f);
			config.restore<bool>("vsync", false, "window");
			config.restore<bool>("maximized", false, "window");
			config.restore<bool>("debug", true, "window");
			config.restore<bool>("visible_cursor", true, "window");
			config.restore<bool>("scale_to_monitor", true, "window");
			config.restore<int>("width", 1280, "window");
			config.restore<int>("height", 720, "window");
			config.restore<float>("sfx_volume", 1.0f, "audio");
			config.restore<float>("music_volume", 1.0f, "audio");
			config.restore<float>("dialogue_volume", 1.0f, "audio");
			config.restore<int>("listener_count", 1, "audio");
			config.restore<bool>("trilinear_filtering", false, "graphics");
			config.restore<int>("ansiotrophic_filtering", 2, "graphics");
			config.restore<int>("texture_atlas_size", 2048, "graphics");
			config.restore<bool>("smaa", false, "graphics.effects");
			config.restore<bool>("sharpen", false, "graphics.effects");
			config.restore<bool>("gamma_correction", false, "graphics.effects");
			config.restore<bool>("chromatic_abberation", false, "graphics.effects");
			config.restore<bool>("gaussian_blur", false, "graphics.effects");
			config.restore<bool>("film_grain", false, "graphics.effects");
			config.restore<int>("camera_foward", static_cast<int>(input::Keys::W), "input");
			config.restore<int>("camera_backward", static_cast<int>(input::Keys::S), "input");
			config.restore<int>("camera_left", static_cast<int>(input::Keys::A), "input");
			config.restore<int>("camera_right", static_cast<int>(input::Keys::D), "input");
			config.restore<int>("camera_rotate_left", static_cast<int>(input::Keys::Q), "input");
			config.restore<int>("camera_rotate_right", static_cast<int>(input::Keys::E), "input");

			config.save();

			//
			// Window Settings.
			// clang-format off
			//
			const auto window_settings = WindowSettings
			{
				.title = config.get<std::string>("title", "window"),
				.width = config.get<int>("width", "window"),
				.height = config.get<int>("height", "window"),
				.vsync = config.get<bool>("vsync", "window"),
				.maximized = config.get<bool>("maximized", "window"),
				.debug = config.get<bool>("debug", "window"),
				.scale_to_monitor = config.get<bool>("scale_to_monitor", "window")
			};
			// clang-format on

			//
			// Construct window and init glfw and OpenGL.
			//
			auto& window   = ServiceLocator<Window>::make(window_settings);
			auto& renderer = ServiceLocator<graphics::Renderer>::make();

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
			// Nuklear UI.
			//
			auto& nui = ServiceLocator<ui::NuklearUI>::make();
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
			em.register_component<components::MapData>("MapData");
			em.register_component<components::ParticleGenerator>("ParticleGenerator");
			em.register_component<components::Primitive>("Primitive");
			em.register_component<components::RigidBody>("RigidBody");
			em.register_component<components::Script>("Script");
			em.register_component<components::Sprite>("Sprite");
			em.register_component<components::Tag>("Tag");
			em.register_component<components::Text>("Text");
			em.register_component<components::Transform>("Transform");
			em.register_component<components::UIScript>("UIScript");
			em.register_component<flags::DenySerialization>("DenySerialization");
			em.register_component<flags::Disabled>("Disabled");

			em.register_dependencies<components::Sprite, components::Transform>();
			em.register_dependencies<components::Primitive, components::Transform>();
			em.register_dependencies<components::Text, components::Transform>();
			em.register_dependencies<components::Animated, components::Sprite>();
			em.register_dependencies<components::RigidBody, components::Transform>();
			em.register_dependencies<components::MapData, components::Transform, components::Animated>();

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
			ServiceLocator<resource::Shaders>::make();
			ServiceLocator<resource::Fonts>::make();
			ServiceLocator<resource::TextureAtlas>::make();
			ServiceLocator<resource::Maps>::make();
			ServiceLocator<resource::Prefabs>::make();
			ServiceLocator<resource::Scripts>::make();
			auto& dispatcher = ServiceLocator<entt::dispatcher>::make();
			ServiceLocator<scene::SceneManager>::make();

			// Late event subscribing.
			dispatcher.sink<events::WindowResized>().connect<&graphics::Renderer::on_window_resized>(renderer);
			dispatcher.sink<events::MousePressed>().connect<&ui::NuklearUI::on_mouse_pressed>(nui);
			dispatcher.sink<events::MouseWheel>().connect<&ui::NuklearUI::on_mouse_wheel>(nui);
			dispatcher.sink<events::KeyChar>().connect<&ui::NuklearUI::on_key_char>(nui);

			//
			// Add external libraries to Lua.
			// Inject all configured galaxy into Lua.
			// Add engine services to lua.
			//
			lua::inject_external_modules();
			lua::inject_dependencies();
			lua::inject_nuklear();
			lua::inject_galaxy();
			lua::inject_services();

			core::ServiceLocator<core::Loader>::ref().load_all();
		}

		App::~App()
		{
			ServiceLocator<scene::SceneManager>::del();
			ServiceLocator<entt::dispatcher>::del();
			ServiceLocator<resource::Scripts>::del();
			ServiceLocator<resource::Prefabs>::del();
			ServiceLocator<resource::Maps>::del();
			ServiceLocator<resource::TextureAtlas>::del();
			ServiceLocator<resource::Fonts>::del();
			ServiceLocator<resource::Shaders>::del();
			ServiceLocator<resource::SoundCache>::del();
			ServiceLocator<resource::MusicCache>::del();
			ServiceLocator<resource::VoiceCache>::del();
			ServiceLocator<resource::VideoCache>::del();
			ServiceLocator<media::SoundEngine>::del();
			ServiceLocator<media::MusicEngine>::del();
			ServiceLocator<media::VoiceEngine>::del();
			ServiceLocator<sol::state>::del();
			ServiceLocator<meta::EntityMeta>::del();
			ServiceLocator<graphics::FontContext>::del();
			ServiceLocator<ui::NuklearUI>::del();
			ServiceLocator<fs::VirtualFileSystem>::del();
			ServiceLocator<Loader>::del();
			ServiceLocator<graphics::Renderer>::del();
			ServiceLocator<Window>::del();
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

			auto& config   = ServiceLocator<Config>::ref();
			auto& window   = ServiceLocator<Window>::ref();
			auto& manager  = ServiceLocator<scene::SceneManager>::ref();
			auto& nui      = ServiceLocator<ui::NuklearUI>::ref();
			auto& renderer = ServiceLocator<graphics::Renderer>::ref();

			const auto log_perf = config.get<bool>("log_performance");

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
					renderer.flush();

					window.poll_events();
					manager.update();

					accumulator -= ups_as_nano;

					updates++;
				}

				manager.render();
				renderer.swap_buffers();

				frames++;

				if (perf_counter >= one_second)
				{
					if (log_perf)
					{
						GALAXY_LOG(GALAXY_INFO, "FPS: {0} | UPS: {1}.", frames, updates);
					}

					GALAXY_CUR_UPS = updates;
					GALAXY_CUR_FPS = frames;

					frames       = 0;
					updates      = 0;
					perf_counter = std::chrono::nanoseconds {0};
				}
			}
		}
	} // namespace core
} // namespace galaxy
