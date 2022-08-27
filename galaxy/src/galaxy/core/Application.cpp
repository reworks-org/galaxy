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
#include "galaxy/error/ConsoleSink.hpp"
#include "galaxy/error/FileSink.hpp"
#include "galaxy/fs/VirtualFileSystem.hpp"
#include "galaxy/graphics/FontContext.hpp"
#include "galaxy/platform/Platform.hpp"
#include "galaxy/resource/Fonts.hpp"
#include "galaxy/resource/Language.hpp"
#include "galaxy/resource/Scripts.hpp"
#include "galaxy/resource/Shaders.hpp"
#include "galaxy/resource/Sounds.hpp"
#include "galaxy/resource/TextureAtlas.hpp"
#include "galaxy/scripting/JSON.hpp"
#include "galaxy/scripting/Lua.hpp"
#include "galaxy/state/SceneManager.hpp"
#include "galaxy/state/layers/RuntimeLayer.hpp"
#include "galaxy/state/layers/UILayer.hpp"
#include "galaxy/ui/RMLFile.hpp"
#include "galaxy/ui/RMLSystem.hpp"
#include "galaxy/ui/RMLRenderer.hpp"

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

			// Set up all of the difference services.
			// The services are configured based off of the config file.
			// Services are created in dependency order.

			// Use half of available threads, rounded up.
			const auto hardware_count = std::thread::hardware_concurrency();
			if (hardware_count < 4)
			{
				GALAXY_LOG(GALAXY_FATAL, "You do not have enough hardware threads.");
			}

			auto thread_count = static_cast<int>(std::ceil(static_cast<float>(hardware_count) / 2.0f));
			if (thread_count < 2)
			{
				thread_count = 2;
			}

			//
			// Threadpool.
			//
			auto& tp = ServiceLocator<BS::thread_pool>::make(thread_count);

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
			if (config.empty())
			{
				config.set<std::string>("asset_dir", "assets/");
				config.set<std::string>("default_lang", "en_au");
				config.set<std::string>("app_data", "default.galaxy");
				config.set<bool>("log_performance", true);
				config.set<std::string>("title", "Title", "window");
				config.set<std::string>("icon", "", "window");
				config.set<std::string>("cursor_icon", "", "window");
				config.set<bool>("vsync", false, "window");
				config.set<bool>("maximized", false, "window");
				config.set<bool>("debug", true, "window");
				config.set<bool>("visible_cursor", true, "window");
				config.set<bool>("allow_native_closing", true, "window");
				config.set<bool>("scale_to_monitor", true, "window");
				config.set<int>("width", 1280, "window");
				config.set<int>("height", 720, "window");
				config.set<float>("sfx_volume", 1.0f, "audio");
				config.set<float>("music_volume", 1.0f, "audio");
				config.set<float>("dialogue_volume", 1.0f, "audio");
				config.set<bool>("trilinear_filtering", false, "graphics");
				config.set<int>("ansiotrophic_filtering", 2, "graphics");
				config.set<std::string>("shader_folder", "shaders/", "resource_folders");
				config.set<std::string>("scripts_folder", "scripts/", "resource_folders");
				config.set<std::string>("font_folder", "fonts/", "resource_folders");
				config.set<std::string>("lang_folder", "lang/", "resource_folders");
				config.set<std::string>("texture_folder", "textures/", "resource_folders");
				config.set<std::string>("atlas_folder", "atlas/", "resource_folders");
				config.set<std::string>("music_folder", "audio/music/", "resource_folders");
				config.set<std::string>("sfx_folder", "audio/sfx/", "resource_folders");
				config.set<std::string>("dialogue_folder", "audio/dialogue/", "resource_folders");
				config.set<bool>("enable_aa", false, "graphics");
				config.set<bool>("enable_sharpen", false, "graphics");
				config.set<std::string>("bg", "", "loading");
				config.set<std::string>("logo", "", "loading");
				config.set<std::string>("bg_atlas", "", "loading");
				config.set<std::string>("bg_shader", "", "loading");
				config.set<std::string>("bg_fonts", "", "loading");

				config.save();
			}

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

				// create_asset_layout(root, "json/");
				// create_asset_layout(root, "maps/");

				// Generate default language file.
				if (!fs.save("lang={}", config.get<std::string>("lang_folder", "resource_folders") + "en_au.lua"))
				{
					GALAXY_LOG(GALAXY_FATAL, "Failed to save default language file.");
				}
			}
			else
			{
				GALAXY_LOG(GALAXY_FATAL, "Could not parse root asset directory.");
			}

			//
			// WINDOW
			//

			// clang-format off
			const auto window_settings = WindowSettings
			{
				.m_title = config.get<std::string>("title", "window"),
				.m_width = config.get<int>("width", "window"),
				.m_height = config.get<int>("height", "window"),
				.m_vsync = config.get<bool>("vsync", "window"),
				.m_maximized = config.get<bool>("maximized", "window"),
				.m_debug = config.get<bool>("debug", "window"),
				.m_scale_to_monitor = config.get<bool>("scale_to_monitor", "window"),
				.m_enable_aa = config.get<bool>("enable_aa", "graphics"),
				.m_enable_sharpen= config.get<bool>("enable_sharpen", "graphics")
			};
			// clang-format on

			auto& window = ServiceLocator<Window>::make(window_settings);

			if (config.get<bool>("allow_native_closing", "window"))
			{
				window.allow_native_closing();
			}
			else
			{
				window.prevent_native_closing();
			}

			auto icon = config.get<std::string>("icon", "window");
			if (!icon.empty())
			{
				window.set_icon(icon);
			}

			auto& cursor = window.get_input<input::Cursor>();
			cursor.toggle(config.get<bool>("visible_cursor", "window"));

			auto cursor_icon = config.get<std::string>("cursor_icon", "window");
			if (!cursor_icon.empty())
			{
				cursor.set_cursor_icon(cursor_icon);
			}

			//
			// Load rest of data with a load screen.
			//

			Loading loading;
			loading.prep_window_for_loading();

			//
			// Begin loading shader related data.
			//
			loading.display_loadingscreen(config.get<std::string>("bg_shader", "loading"));

			auto& shaders = ServiceLocator<resource::Shaders>::make();
			shaders.load(config.get<std::string>("shader_folder", "resource_folders"));

			//
			// Begin loading fonts.
			//
			loading.display_loadingscreen(config.get<std::string>("bg_fonts", "loading"));
			ServiceLocator<graphics::FontContext>::make();

			auto& fonts = ServiceLocator<resource::Fonts>::make();
			fonts.load(config.get<std::string>("font_folder", "resource_folders"));

			//
			// RML Renderer.
			//
			m_rml_rendering_interface = std::make_unique<ui::RMLRenderer>();

			//
			// Begin loading textures.
			//
			loading.display_loadingscreen(config.get<std::string>("bg_atlas", "loading"));

			auto& textureatlas = ServiceLocator<resource::TextureAtlas>::make();
			textureatlas.add_folder(config.get<std::string>("atlas_folder", "resource_folders"));

			//
			// Begin offthread loading.
			// clang-format off
			//
			loading.start_offthread_loading([&, this]()
			{
				// NOTE: You CANT call OpenGL here. This runs on a separate thread.

				//
				// LUA.
				//
				auto& lua = ServiceLocator<sol::state>::make();

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
				// clang-format on

				//
				// LANGUAGES.
				//
				auto& lang = ServiceLocator<resource::Language>::make();
				lang.load(config.get<std::string>("lang_folder", "resource_folders"));
				lang.set(config.get<std::string>("default_lang"));

				//
				// Audio Engine.
				//
				auto& ae = ServiceLocator<audio::AudioEngine>::make();
				ae.set_sfx_volume(config.get<float>("sfx_volume", "audio"));
				ae.set_music_volume(config.get<float>("music_volume", "audio"));
				ae.set_voice_volume(config.get<float>("dialogue_volume", "audio"));

				//
				// Game Resources.
				//
				auto& sounds = ServiceLocator<resource::Sounds>::make();
				sounds.load_sfx(config.get<std::string>("sfx_folder", "resource_folders"));
				sounds.load_music(config.get<std::string>("music_folder", "resource_folders"));
				sounds.load_dialogue(config.get<std::string>("dialogue_folder", "resource_folders"));

				auto& scripts = ServiceLocator<resource::Scripts>::make();
				scripts.load(config.get<std::string>("scripts_folder", "resource_folders"));

				//
				// UI.
				//
				m_rml_system_interface = std::make_unique<ui::RMLSystem>();
				m_rml_file_interface   = std::make_unique<ui::RMLFile>();

				Rml::SetSystemInterface(m_rml_system_interface.get());
				Rml::SetFileInterface(m_rml_file_interface.get());
				Rml::SetRenderInterface(m_rml_rendering_interface.get());

				if (Rml::Initialise())
				{
					Rml::Lua::Initialise(lua.lua_state());

					const auto dir      = config.get<std::string>("font_folder", "resource_folders");
					const auto contents = ServiceLocator<fs::VirtualFileSystem>::ref().list_directory(dir);

					if (!contents.empty())
					{
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
						GALAXY_LOG(GALAXY_WARNING, "Found no fonts to load into RmlUi at '{0}'.", dir);
					}
				}
				else
				{
					GALAXY_LOG(GALAXY_FATAL, "Failed to initialize RmlUi.");
				}

				//
				// SceneManager.
				//
				state::LayerRegistry::register_type<state::UILayer>("UI");
				state::LayerRegistry::register_type<state::RuntimeLayer>("Runtime");
				ServiceLocator<state::SceneManager>::make();

				//
				// Inject all configured galaxy into Lua.
				//
				lua::inject_galaxy_into_lua();
			});

			loading.display_bg_until_finished(config.get<std::string>("bg", "loading"), config.get<std::string>("logo", "loading"));
		}

		Application::~Application()
		{
			ServiceLocator<state::SceneManager>::del();

			Rml::Shutdown();
			m_rml_system_interface.reset();
			m_rml_file_interface.reset();
			m_rml_rendering_interface.reset();

			ServiceLocator<resource::Scripts>::del();
			ServiceLocator<resource::Sounds>::del();
			ServiceLocator<audio::AudioEngine>::del();
			ServiceLocator<resource::Language>::del();
			ServiceLocator<sol::state>::del();
			ServiceLocator<resource::TextureAtlas>::del();
			ServiceLocator<resource::Fonts>::del();
			ServiceLocator<graphics::FontContext>::del();
			ServiceLocator<resource::Shaders>::del();
			ServiceLocator<Window>::del();
			ServiceLocator<fs::VirtualFileSystem>::del();
			ServiceLocator<Config>::del();

			GALAXY_LOG_FINISH();

			ServiceLocator<BS::thread_pool>::ref().wait_for_tasks();
			ServiceLocator<BS::thread_pool>::del();
		}

		void Application::load(std::string_view json_file)
		{
			auto& fs      = ServiceLocator<fs::VirtualFileSystem>::ref();
			auto& manager = ServiceLocator<state::SceneManager>::ref();

			const auto default_data = fs.open(json_file);
			if (default_data.has_value())
			{
				auto decoded_zlib   = algorithm::decode_zlib(default_data.value());
				auto decoded_base64 = algorithm::decode_base64(decoded_zlib);

				manager.load(decoded_base64);
			}
			else
			{
				GALAXY_LOG(GALAXY_FATAL, "Failed to load json data file '{0}'.", json_file);
			}
		}

		void Application::run()
		{
			auto& config  = ServiceLocator<Config>::ref();
			auto& window  = ServiceLocator<Window>::ref();
			auto& manager = ServiceLocator<state::SceneManager>::ref();

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

				if (log_perf)
				{
					perf_counter += elapsed;
				}

				while (accumulator >= GALAXY_UPS)
				{
					window.poll_events();

					manager.current()->events();
					manager.current()->update();

					accumulator -= ups_as_nano;

					if (log_perf)
					{
						updates++;
					}
				}

				window.begin();
				manager.current()->render();
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