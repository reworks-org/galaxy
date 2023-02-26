///
/// Application.cpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#include <BS_thread_pool.hpp>
#include <RmlUi/Core.h>
#include <RmlUi/Lua.h>

#include "galaxy/algorithm/Base64.hpp"
#include "galaxy/algorithm/ZLib.hpp"
#include "galaxy/audio/AudioEngine.hpp"
#include "galaxy/core/Config.hpp"
#include "galaxy/core/Loading.hpp"
#include "galaxy/core/ServiceLocator.hpp"
#include "galaxy/core/Window.hpp"
#include "galaxy/embedded/RobotoLight.hpp"
#include "galaxy/error/ConsoleSink.hpp"
#include "galaxy/error/FileSink.hpp"
#include "galaxy/fs/VirtualFileSystem.hpp"
#include "galaxy/graphics/FontContext.hpp"
#include "galaxy/graphics/Renderer.hpp"
#include "galaxy/input/Input.hpp"
#include "galaxy/physics/Constants.hpp"
#include "galaxy/platform/Platform.hpp"
#include "galaxy/resource/Fonts.hpp"
#include "galaxy/resource/Language.hpp"
#include "galaxy/resource/Maps.hpp"
#include "galaxy/resource/Prefabs.hpp"
#include "galaxy/resource/Scripts.hpp"
#include "galaxy/resource/Shaders.hpp"
#include "galaxy/resource/Sounds.hpp"
#include "galaxy/resource/TextureAtlas.hpp"
#include "galaxy/scripting/JSON.hpp"
#include "galaxy/scripting/Lua.hpp"
#include "galaxy/scene/SceneManager.hpp"
#include "galaxy/scene/layers/RuntimeLayer.hpp"
#include "galaxy/scene/layers/UILayer.hpp"

#include "Application.hpp"

namespace galaxy
{
	namespace core
	{
		Application::Application(std::string_view log_dir, std::string_view config_file)
		{
			platform::configure_terminal();

			// Seed pseudo-random algorithms.
			std::srand(static_cast<unsigned int>(std::time(nullptr)));

			// Make sure there is enough cores to run.
			const auto hardware_count = std::thread::hardware_concurrency();
			if (hardware_count < 4)
			{
				GALAXY_LOG(GALAXY_FATAL, "You do not have enough hardware threads. 4 is the minimum required.");
			}

			//
			// LOGGING.
			//
			const auto now             = std::chrono::zoned_time {std::chrono::current_zone(), std::chrono::system_clock::now()}.get_local_time();
			const std::string log_path = std::format("{0}{1}{2}", log_dir, std::format("{0:%d-%m-%Y-[%H-%M]}", now), ".log");
			if (!std::filesystem::exists(log_dir))
			{
				std::filesystem::create_directory(log_dir);
			}

			GALAXY_LOG_START();
			GALAXY_ADD_SINK(error::FileSink, log_path);
			GALAXY_ADD_SINK(error::ConsoleSink);

			//
			// CONFIG.
			//
			auto& config = ServiceLocator<Config>::make(config_file);
			config.restore<std::string>("asset_dir", "assets/");
			config.restore<std::string>("default_lang", "en_au");
			config.restore<std::string>("app_data", "default.galaxy");
			config.restore<std::string>("load_screen_rml", "ui/load.rml");
			config.restore<bool>("log_performance", true);
			config.restore<std::string>("title", "Title", "window");
			config.restore<std::string>("icon", "", "window");
			config.restore<std::string>("cursor_icon", "", "window");
			config.restore<bool>("vsync", false, "window");
			config.restore<bool>("maximized", false, "window");
			config.restore<bool>("debug", true, "window");
			config.restore<bool>("visible_cursor", true, "window");
			config.restore<bool>("allow_native_closing", true, "window");
			config.restore<bool>("scale_to_monitor", true, "window");
			config.restore<int>("width", 1280, "window");
			config.restore<int>("height", 720, "window");
			config.restore<float>("sfx_volume", 1.0f, "audio");
			config.restore<float>("music_volume", 1.0f, "audio");
			config.restore<float>("dialogue_volume", 1.0f, "audio");
			config.restore<int>("listener_count", 1, "audio");
			config.restore<bool>("trilinear_filtering", false, "graphics");
			config.restore<int>("ansiotrophic_filtering", 2, "graphics");
			config.restore<std::string>("maps_folder", "maps/", "resource_folders");
			config.restore<std::string>("shader_folder", "shaders/", "resource_folders");
			config.restore<std::string>("scripts_folder", "scripts/", "resource_folders");
			config.restore<std::string>("font_folder", "fonts/", "resource_folders");
			config.restore<std::string>("lang_folder", "lang/", "resource_folders");
			config.restore<std::string>("prefabs_folder", "prefabs/", "resource_folders");
			config.restore<std::string>("texture_folder", "textures/", "resource_folders");
			config.restore<std::string>("atlas_folder", "atlas/", "resource_folders");
			config.restore<std::string>("music_folder", "audio/music/", "resource_folders");
			config.restore<std::string>("sfx_folder", "audio/sfx/", "resource_folders");
			config.restore<std::string>("dialogue_folder", "audio/dialogue/", "resource_folders");
			config.restore<std::string>("ui_folder", "ui/", "resource_folders");
			config.restore<bool>("enable_aa", false, "graphics");
			config.restore<bool>("enable_sharpen", false, "graphics");
			config.restore<int>("camera_foward", static_cast<int>(input::Keys::W), "input");
			config.restore<int>("camera_backward", static_cast<int>(input::Keys::S), "input");
			config.restore<int>("camera_left", static_cast<int>(input::Keys::A), "input");
			config.restore<int>("camera_right", static_cast<int>(input::Keys::D), "input");
			config.restore<int>("camera_rotate_left", static_cast<int>(input::Keys::Q), "input");
			config.restore<int>("camera_rotate_right", static_cast<int>(input::Keys::E), "input");
			config.restore<float>("ppm", 64.0f, "box2d");
			config.restore<float>("x", 0.0f, "box2d.gravity");
			config.restore<float>("y", 0.0f, "box2d.gravity");
			config.restore<int>("velocity_iterations", 8, "box2d");
			config.restore<int>("position_iterations", 3, "box2d");

			config.save();

			//
			// VIRTUAL FILE SYSTEM.
			//
			auto root_opt = config.get<std::string>("asset_dir");
			if (!root_opt.empty())
			{
				auto& root = root_opt;
				if (root.back() != '/')
				{
					root += '/';
				}

				create_asset_layout(root, "");

				auto& fs = ServiceLocator<fs::VirtualFileSystem>::make(root);

				create_asset_layout(root, config.get<std::string>("music_folder", "resource_folders"));
				create_asset_layout(root, config.get<std::string>("sfx_folder", "resource_folders"));
				create_asset_layout(root, config.get<std::string>("dialogue_folder", "resource_folders"));
				create_asset_layout(root, config.get<std::string>("font_folder", "resource_folders"));
				create_asset_layout(root, config.get<std::string>("scripts_folder", "resource_folders"));
				create_asset_layout(root, config.get<std::string>("shader_folder", "resource_folders"));
				create_asset_layout(root, config.get<std::string>("texture_folder", "resource_folders"));
				create_asset_layout(root, config.get<std::string>("atlas_folder", "resource_folders"));
				create_asset_layout(root, config.get<std::string>("lang_folder", "resource_folders"));
				create_asset_layout(root, config.get<std::string>("prefabs_folder", "resource_folders"));
				create_asset_layout(root, config.get<std::string>("maps_folder", "resource_folders"));
				create_asset_layout(root, config.get<std::string>("ui_folder", "resource_folders"));

				// Generate default language file.
				if (!fs.save("lang={}", config.get<std::string>("lang_folder", "resource_folders") + "en_au.lang"))
				{
					GALAXY_LOG(GALAXY_FATAL, "Failed to save default language file.");
				}

				// Generate default RML for load screen.
				if (!fs.exists(config.get<std::string>("load_screen_rml")))
				{
					if (!fs.save("<rml><head><title>Loading</title><style>p{font-family: Roboto;}</style></head><body><p>Loading</p></body></rml>",
							config.get<std::string>("load_screen_rml")))
					{
						GALAXY_LOG(GALAXY_ERROR, "Failed to save default load screen rml.");
					}
				}
			}
			else
			{
				GALAXY_LOG(GALAXY_FATAL, "Could not parse root asset directory.");
			}

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
				.scale_to_monitor = config.get<bool>("scale_to_monitor", "window"),
				.enable_aa = config.get<bool>("enable_aa", "graphics"),
				.enable_sharpen= config.get<bool>("enable_sharpen", "graphics")
			};
			// clang-format on

			//
			// Construct window and init glfw and OpenGL.
			//
			auto& window = ServiceLocator<Window>::make(window_settings);
			auto icon    = config.get<std::string>("icon", "window");
			if (!icon.empty())
			{
				window.set_icon(icon);
			}

			//
			// Threadpool.
			//
			ServiceLocator<BS::thread_pool>::make(GALAXY_WORKER_THREADS);

			//
			// Set up Font Context.
			//
			ServiceLocator<graphics::FontContext>::make();

			//
			// UI.
			//
			m_rml_rendering_interface.init();
			Rml::SetSystemInterface(&m_rml_system_interface);
			Rml::SetFileInterface(&m_rml_file_interface);
			Rml::SetRenderInterface(&m_rml_rendering_interface);

			if (Rml::Initialise())
			{
				const auto dir      = config.get<std::string>("font_folder", "resource_folders");
				const auto contents = ServiceLocator<fs::VirtualFileSystem>::ref().list_directory(dir);

				// Load default font first.
				Rml::LoadFontFace(reinterpret_cast<Rml::byte*>(&embedded::roboto_light), embedded::roboto_light_len, "Roboto", Rml::Style::FontStyle::Normal);

				for (const auto& file : contents)
				{
					if (!Rml::LoadFontFace(file))
					{
						GALAXY_LOG(GALAXY_ERROR, "Failed to load '{0}' from '{1}' into RmlUi.", file, dir);
					}
				}
			}
			else
			{
				GALAXY_LOG(GALAXY_FATAL, "Failed to initialize RmlUi.");
			}

			//
			// Begin offthread loading.
			//
			Loading loading(config.get<std::string>("load_screen_rml"));

			// clang-format off
			//
            // NOTE: You CANT call OpenGL here. This runs on a separate thread.
			//
			loading.start([&, this]()
			{
				try
				{
					// clang-format on

					//
					// Set inputs from config.
					//
					input::CameraKeys::FORWARD      = input::int_to_key(config.get<int>("camera_foward", "input"));
					input::CameraKeys::BACKWARD     = input::int_to_key(config.get<int>("camera_backward", "input"));
					input::CameraKeys::LEFT         = input::int_to_key(config.get<int>("camera_left", "input"));
					input::CameraKeys::RIGHT        = input::int_to_key(config.get<int>("camera_right", "input"));
					input::CameraKeys::ROTATE_LEFT  = input::int_to_key(config.get<int>("camera_rotate_left", "input"));
					input::CameraKeys::ROTATE_RIGHT = input::int_to_key(config.get<int>("camera_rotate_right", "input"));

					//
					// Window closing config.
					//
					if (config.get<bool>("allow_native_closing", "window"))
					{
						window.allow_native_closing();
					}
					else
					{
						window.prevent_native_closing();
					}

					//
					// Window cursor.
					//
					auto& cursor = window.get_input<input::Cursor>();
					cursor.toggle(config.get<bool>("visible_cursor", "window"));

					auto cursor_icon = config.get<std::string>("cursor_icon", "window");
					if (!cursor_icon.empty())
					{
						cursor.set_cursor_icon(cursor_icon);
					}

					//
					// Physics config data.
					//
					physics::Constants::gravity.x           = config.get<float>("x", "box2d.gravity");
					physics::Constants::gravity.y           = config.get<float>("y", "box2d.gravity");
					physics::Constants::velocity_iterations = config.get<int>("velocity_iterations", "box2d");
					physics::Constants::position_iterations = config.get<int>("position_iterations", "box2d");
					physics::Constants::pixels_per_meter    = config.get<float>("ppm", "box2d");

					//
					// Begin loading shader related data.
					//
					auto& shaders = ServiceLocator<resource::Shaders>::make();
					shaders.load(config.get<std::string>("shader_folder", "resource_folders"));

					//
					// Begin loading fonts.
					//
					auto& fonts = ServiceLocator<resource::Fonts>::make();
					fonts.load(config.get<std::string>("font_folder", "resource_folders"));

					//
					// Lua.
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
					// LANGUAGES.
					//
					auto& lang = ServiceLocator<resource::Language>::make();
					lang.load(config.get<std::string>("lang_folder", "resource_folders"));
					lang.set(config.get<std::string>("default_lang"));

					//
					// Audio Engine.
					//
					auto& ae = ServiceLocator<audio::AudioEngine>::make(config.get<int>("listener_count", "audio"));
					ae.set_sfx_volume(config.get<float>("sfx_volume", "audio"));
					ae.set_music_volume(config.get<float>("music_volume", "audio"));
					ae.set_dialogue_volume(config.get<float>("dialogue_volume", "audio"));

					//
					// Game audio files.
					//
					auto& sounds = ServiceLocator<resource::Sounds>::make();
					sounds.load_sfx(config.get<std::string>("sfx_folder", "resource_folders"));
					sounds.load_music(config.get<std::string>("music_folder", "resource_folders"));
					sounds.load_dialogue(config.get<std::string>("dialogue_folder", "resource_folders"));

					//
					// Generic non entity scripts.
					//
					auto& scripts = ServiceLocator<resource::Scripts>::make();
					scripts.load(config.get<std::string>("scripts_folder", "resource_folders"));

					//
					// Entity prefabs.
					//
					auto& prefabs = ServiceLocator<resource::Prefabs>::make();
					prefabs.load(config.get<std::string>("prefabs_folder", "resource_folders"));

					//
					// Maps.
					//
					auto& maps = ServiceLocator<resource::Maps>::make();
					maps.load(config.get<std::string>("maps_folder", "resource_folders"));

					//
					// SceneManager.
					//
					scene::LayerRegistry::register_type<scene::UILayer>("UI");
					scene::LayerRegistry::register_type<scene::RuntimeLayer>("Runtime");
					ServiceLocator<scene::SceneManager>::make();

					//
					// Add external libraries to Lua.
					//
					lua::load_external_libs();

					//
					// Inject all configured galaxy into Lua.
					//
					Rml::Lua::Initialise(lua.lua_state());
					lua::inject_galaxy_into_lua();
				}
				catch (const std::exception& e)
				{
					GALAXY_LOG(GALAXY_ERROR, e.what());
				}
			});

			// Will display until loading offthread is done.
			loading.display();

			ServiceLocator<resource::Shaders>::ref().compile();
			ServiceLocator<resource::Fonts>::ref().build();

			//
			// Build texture atlas.
			//
			auto& textureatlas = ServiceLocator<resource::TextureAtlas>::make();
			textureatlas.add_folder(config.get<std::string>("atlas_folder", "resource_folders"));

			//
			// Add engine services to lua.
			//
			lua::inject_services_into_lua();
		}

		Application::~Application()
		{
			ServiceLocator<scene::SceneManager>::del();
			scene::LayerRegistry::m_registry.clear();
			ServiceLocator<resource::Scripts>::del();
			ServiceLocator<resource::Sounds>::del();
			ServiceLocator<audio::AudioEngine>::del();
			ServiceLocator<resource::Language>::del();
			ServiceLocator<sol::state>::del();
			ServiceLocator<resource::Fonts>::del();
			ServiceLocator<resource::Shaders>::del();
			ServiceLocator<resource::TextureAtlas>::del();
			Rml::Shutdown();
			m_rml_rendering_interface.destroy();
			ServiceLocator<graphics::FontContext>::del();
			ServiceLocator<BS::thread_pool>::ref().wait_for_tasks();
			ServiceLocator<BS::thread_pool>::del();
			ServiceLocator<Window>::del();
			ServiceLocator<fs::VirtualFileSystem>::del();
			ServiceLocator<Config>::del();

			GALAXY_LOG_FINISH();
		}

		void Application::load(std::string_view json_file)
		{
			auto& fs      = ServiceLocator<fs::VirtualFileSystem>::ref();
			auto& manager = ServiceLocator<scene::SceneManager>::ref();

			const auto default_data = fs.open(json_file);
			if (default_data.has_value())
			{
				const auto decoded_zlib   = algorithm::decode_zlib(default_data.value());
				const auto decoded_base64 = algorithm::decode_base64(decoded_zlib);

				manager.load(decoded_base64);
			}
			else
			{
				GALAXY_LOG(GALAXY_FATAL, "Failed to load json data file '{0}'.", json_file);
			}
		}

		void Application::run()
		{
#ifdef NDEBUG
			try
			{
#endif
				auto& config  = ServiceLocator<Config>::ref();
				auto& window  = ServiceLocator<Window>::ref();
				auto& manager = ServiceLocator<scene::SceneManager>::ref();

				const bool log_perf = config.get<bool>("log_performance");

				unsigned int frames  = 0;
				unsigned int updates = 0;

				constexpr const auto ups_as_nano = std::chrono::duration_cast<std::chrono::nanoseconds>(GALAXY_UPS);
				constexpr const auto one_second  = std::chrono::seconds {1};

				std::chrono::nanoseconds accumulator {0};
				std::chrono::nanoseconds perf_counter {0};
				std::chrono::nanoseconds elapsed {0};

				auto current  = std::chrono::high_resolution_clock::now();
				auto previous = current;

				while (window.is_open())
				{
					current  = std::chrono::high_resolution_clock::now();
					elapsed  = current - previous;
					previous = current;
					accumulator += elapsed;

					auto& scene = manager.current();

					if (log_perf)
					{
						perf_counter += elapsed;
					}

					while (accumulator >= GALAXY_UPS)
					{
						graphics::Renderer::flush();

						window.poll_events();

						scene.events();
						scene.update();

						accumulator -= ups_as_nano;

						if (log_perf)
						{
							updates++;
						}
					}

					window.begin();
					scene.render();
					window.end();

					if (log_perf)
					{
						frames++;

						if (perf_counter >= one_second)
						{
							GALAXY_LOG(GALAXY_INFO, "FPS: {0} | UPS: {1}.", frames, updates);
							frames       = 0;
							updates      = 0;
							perf_counter = std::chrono::nanoseconds {0};
						}
					}
				}
#ifdef NDEBUG
			}
			catch (const std::exception& e)
			{
				GALAXY_LOG(GALAXY_ERROR, "Main loop exception: '{0}'.", e.what());
			}
#endif
		}

		void Application::create_asset_layout(const std::string& root, const std::string& asset_folder)
		{
			const auto merged = root + asset_folder;
			if (!std::filesystem::exists(merged))
			{
				GALAXY_LOG(GALAXY_INFO, "Missing asset folder, creating at: '{0}'.", merged);
				std::filesystem::create_directories(merged);
			}
		}
	} // namespace core
} // namespace galaxy