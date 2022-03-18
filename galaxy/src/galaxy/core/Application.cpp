///
/// Application.cpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#include <sol/sol.hpp>

#include "galaxy/core/AudioContext.hpp"
#include "galaxy/core/Config.hpp"
#include "galaxy/core/ServiceLocator.hpp"
#include "galaxy/core/Window.hpp"
#include "galaxy/error/ConsoleSink.hpp"
#include "galaxy/error/FileSink.hpp"
#include "galaxy/error/Log.hpp"
#include "galaxy/fs/VirtualFileSystem.hpp"
#include "galaxy/graphics/FreeType.hpp"
#include "galaxy/platform/Platform.hpp"
#include "galaxy/resource/Language.hpp"
#include "galaxy/scripting/Lua.hpp"
#include "galaxy/state/SceneManager.hpp"

#include "Application.hpp"

namespace galaxy
{
	namespace core
	{
		Application::Application(std::string_view log_dir, std::string_view config_file)
			: m_scene {nullptr}
		{
			platform::configure_terminal();

			// Seed pseudo-random algorithms.
			std::srand(static_cast<unsigned int>(std::time(nullptr)));

			// Set up all of the difference services.
			// The services are configured based off of the config file.
			// Services are created in dependency order.

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
				config.set<std::string>("title", "Title", "window");
				config.set<std::string>("icon", "", "window");
				config.set<std::string>("cursor_icon", "", "window");
				config.set<bool>("vsync", true, "window");
				config.set<bool>("maximized", false, "window");
				config.set<bool>("debug", false, "window");
				config.set<bool>("visible_cursor", true, "window");
				config.set<bool>("allow_native_closing", true, "window");
				config.set<bool>("scale_to_monitor", false, "window");
				config.set<int>("width", 1280, "window");
				config.set<int>("height", 720, "window");
				config.set<float>("volume", 1.0f, "audio");
				config.set<float>("doppler_factor", 1.0f, "audio");
				config.set<float>("speed_of_sound", 1.0f, "audio");
				config.set<std::string>("distance_model", "NONE", "audio");
				config.set<bool>("trilinear_filtering", false, "graphics");
				config.set<int>("ansiotrophic_filtering", 2, "graphics");
				config.set<std::string>("shader_folder", "shaders/", "resource_folders");
				config.set<std::string>("lang_folder", "lang/", "resource_folders");

				// config.set<bool>("anti-aliasing", false);
				// config.set<bool>("sharpen", false);
				// config.set<float>("audio-volume", 0.7f);
				// config.set<std::string>("logo", "logo.png");
				// config.set<std::string>("cursor-image", "cursor.png");
				// config.set<std::string>("icon-file", "icon.png");
				// config.set<std::string>("fontbook-json", "fontbook.json");
				// config.set<std::string>("scriptbook-json", "scriptbook.json");
				// config.set<std::string>("texturebook-json", "texturebook.json");
				// config.set<std::string>("soundbook-json", "soundbook.json");
				// config.set<std::string>("musicbook-json", "musicbook.json");
				// config.set<std::string>("renderlayers-json", "renderlayers.json");
				// config.set<std::string>("key-forward", "W");
				// config.set<std::string>("key-back", "S");
				// config.set<std::string>("key-left", "A");
				// config.set<std::string>("key-right", "D");

				config.save();
			}

			//
			// VIRTUAL FILE SYSTEM.
			//
			auto root_opt = config.get<std::string>("asset-dir");
			if (root_opt.has_value())
			{
				auto& root = root_opt.value();
				if (root.back() != '/')
				{
					root += '/';
				}

				create_asset_layout(root, "");

				auto& fs = ServiceLocator<fs::VirtualFileSystem>::make(root);
				// create_asset_layout(root, "audio/music/");
				// create_asset_layout(root, "audio/sfx/");
				// create_asset_layout(root, "fonts/");
				// create_asset_layout(root, "json/");
				// create_asset_layout(root, "scripts/actions/");
				// create_asset_layout(root, "scripts/definitions/");
				create_asset_layout(root, config.get<std::string>("shader_folder", "resource_folders").value());
				// create_asset_layout(root, "textures/");
				// create_asset_layout(root, "maps/");
				create_asset_layout(root, config.get<std::string>("lang_folder", "resource_folders").value());

				// Generate default language file.
				fs.save("lang={}", config.get<std::string>("lang_folder", "resource_folders").value() + "en_au.lua");
			}
			else
			{
				GALAXY_LOG(GALAXY_FATAL, "Could not parse root asset directory.");
			}

			//
			// LANGUAGES.
			//
			auto& lang = ServiceLocator<resource::Language>::make();
			lang.load(config.get<std::string>("lang_folder", "resource_folders").value());
			lang.set(config.get<std::string>("default_lang").value());

			//
			// WINDOW
			//

			// clang-format off
			const auto window_settings = WindowSettings
			{
				.m_title = config.get<std::string>("title", "window").value(),
				.m_width = config.get<int>("width", "window").value(),
				.m_height = config.get<int>("width", "window").value(),
				.m_vsync = config.get<bool>("vsync", "window").value(),
				.m_maximized = config.get<bool>("maximized", "window").value(),
				.m_debug = config.get<bool>("debug", "window").value(),
				.m_scale_to_monitor = config.get<bool>("scale_to_monitor", "window").value()
			};
			// clang-format on

			auto& window = ServiceLocator<Window>::make(window_settings);

			auto allow_native_closing = config.get<bool>("allow_native_closing", "window").value();
			if (!allow_native_closing)
			{
				window.prevent_native_closing();
			}

			auto icon = config.get<std::string>("icon", "window");
			if (icon.has_value())
			{
				window.set_icon(icon.value());
			}

			auto& cursor = window.get_input<input::Cursor>();
			cursor.toggle(config.get<bool>("visible_cursor", "window").value());

			auto cursor_icon = config.get<std::string>("cursor_icon", "window");
			if (cursor_icon.has_value())
			{
				cursor.set_cursor_icon(cursor_icon.value());
			}

			//
			// FREETYPE.
			//
			ServiceLocator<graphics::FreeTypeLib>::make();

			//
			// OpenAL.
			//
			auto& al = ServiceLocator<AudioContext>::make();
			al.set_listener_gain(config.get<float>("volume", "audio").value());
			al.set_doppler_factor(config.get<float>("doppler_factor", "audio").value());
			al.set_speed_of_sound(config.get<float>("speed_of_sound", "audio").value());

			auto distance_model = magic_enum::enum_cast<AudioContext::DistanceModel>(config.get<std::string>("distance_model", "audio").value());
			if (distance_model.has_value())
			{
				al.set_distance_model(distance_model.value());
			}
			else
			{
				GALAXY_LOG(GALAXY_ERROR, "Tried to set invalid distance model from config.");
			}

			//
			// SCENEMANAGER.
			//
			ServiceLocator<state::SceneManager>::make();

			//
			// LUA.
			//
			auto& lua = ServiceLocator<sol::state>::make();

			// clang-format off
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

			lua::inject_galaxy_into_lua();
		}

		Application::~Application()
		{
			ServiceLocator<sol::state>::del();
			ServiceLocator<graphics::FreeTypeLib>::del();
			ServiceLocator<Window>::del();
			ServiceLocator<resource::Language>::del();
			ServiceLocator<fs::VirtualFileSystem>::del();
			ServiceLocator<Config>::del();

			GALAXY_LOG_FINISH();
		}

		bool Application::run()
		{
			return false;
		}

		void Application::create_asset_layout(const std::string& root, const std::string& asset_folder)
		{
			const auto merged = root + asset_folder;
			if (!std::filesystem::exists(merged))
			{
				GALAXY_LOG(GALAXY_INFO, "Missing asset folder, creating at: {0}.", merged);
				std::filesystem::create_directories(merged);
			}
		}
	} // namespace core
} // namespace galaxy

/*
		Application::Application(std::string_view asset_dir, std::string_view config_file)
			: m_filewatcher {false}
			, m_filelistener {nullptr}
		{
			// Threadpool setup.
			m_pool           = std::make_unique<async::ThreadPool>();
			SL_HANDLE.m_pool = m_pool.get();

			// window
			else
			{
				LoadingScreen logo;
				logo.load(m_config->get<std::string>("logo"));
				logo.display(m_window->get_width(), m_window->get_height(), m_window->gl_window());

				m_window->request_attention();
				m_window->set_cursor_visibility(m_config->get<bool>("is-cursor-visible"));

				if (!m_config->get<std::string>("cursor-image").empty())
				{
					m_window->set_cursor_icon(m_config->get<std::string>("cursor-image"));
				}

				if (!m_config->get<std::string>("icon-file").empty())
				{
					m_window->set_icon(m_config->get<std::string>("icon-file"));
				}

				// Configure audio context.
				m_openal.set_listener_gain(m_config->get<float>("audio-volume"));
				SL_HANDLE.m_openal = &m_openal;

				// Freetype.
				FT_HANDLE.open();

				// Create lua instance and open libraries.
				m_lua = std::make_unique<sol::state>();
				m_lua->open_libraries(sol::lib::base,
					sol::lib::package,
					sol::lib::coroutine,
					sol::lib::string,
					sol::lib::os,
					sol::lib::math,
					sol::lib::table,
					sol::lib::io,
					sol::lib::utf8);
				SL_HANDLE.m_lua = m_lua.get();

				// Generate default assets specified by config.
				generate_default_assets(root);

				// Parse languages.
				m_langs = std::make_unique<res::Language>();
				m_langs->parse_language_folder(root + "lang/");
				m_langs->set_language("en_au");
				SL_HANDLE.m_language = m_langs.get();

				// ScriptBook.
				m_scriptbook           = std::make_unique<res::ScriptBook>(m_config->get<std::string>("scriptbook-json"));
				SL_HANDLE.m_scriptbook = m_scriptbook.get();

				// FontBook.
				m_fontbook           = std::make_unique<res::FontBook>(m_config->get<std::string>("fontbook-json"));
				SL_HANDLE.m_fontbook = m_fontbook.get();

				// Texture Atlas.
				m_texturebook           = std::make_unique<res::TextureBook>(m_config->get<std::string>("texturebook-json"));
				SL_HANDLE.m_texturebook = m_texturebook.get();

				// Set up renderer.
				RENDERER_2D().init(m_config->get<std::string>("renderlayers-json"));

				// SoundBook.
				m_soundbook           = std::make_unique<res::SoundBook>(m_config->get<std::string>("soundbook-json"));
				SL_HANDLE.m_soundbook = m_soundbook.get();

				// MusicBook.
				m_musicbook           = std::make_unique<res::MusicBook>(m_config->get<std::string>("musicbook-json"));
				SL_HANDLE.m_musicbook = m_musicbook.get();

				// Set up custom lua functions and types.
				scripting::register_lua();

				// Register services with Lua.
				// m_lua->set("galaxy_audio_context", &m_openal);
				// m_lua->set("galaxy_config", m_config.get());
				// m_lua->set("galaxy_vfs", m_vfs.get());
				// m_lua->set("galaxy_shaderboox", m_shaderbook.get());
				// m_lua->set("galaxy_fontbook", m_fontbook.get());
				// m_lua->set("galaxy_texturebook", m_texturebook.get());
				// m_lua->set("galaxy_soundbook", m_soundbook.get());
				// m_lua->set("galaxy_musicbook", m_musicbook.get());
				// m_lua->set("galaxy_scriptbook", m_scriptbook.get());
				// m_lua->set("galaxy_language", m_langs.get());

				// Set up RMLUI.
				m_rml_system_interface    = std::make_unique<ui::RMLSystem>();
				m_rml_file_interface      = std::make_unique<ui::RMLFile>();
				m_rml_rendering_interface = std::make_unique<ui::RMLRenderer>();

				Rml::SetSystemInterface(m_rml_system_interface.get());
				Rml::SetFileInterface(m_rml_file_interface.get());
				Rml::SetRenderInterface(m_rml_rendering_interface.get());
				if (!Rml::Initialise())
				{
					GALAXY_LOG(GALAXY_FATAL, "Failed to initialize RmlUi.");
				}
				else
				{
					Rml::Lua::Initialise(m_lua->lua_state());

					for (auto& [key, font] : m_fontbook->cache())
					{
						if (!Rml::LoadFontFace(font->get_filename()))
						{
							GALAXY_LOG(GALAXY_ERROR, "Failed to load RML font: {0}.", key);
						}
					}
				}

				// Begin watching files now that default asset creation is over.
				m_filewatcher.watch();

				// Ensure minimum amount of time has passed.
				logo.wait();
			}
		}

		Application::~Application()
		{
			Rml::Shutdown();
			m_rml_rendering_interface.reset();
			m_rml_file_interface.reset();
			m_rml_system_interface.reset();

			// We want to destroy everything in a specific order to make sure stuff is freed correctly.
			// And of course the file system being the last to be destroyed.
			while (!m_layer_stack.empty())
			{
				m_layer_stack.pop();
			}

			for (auto& layer : m_layers)
			{
				layer.reset();
			}

			m_layers.clear();

			m_pool->finish();
			m_window->destroy();

			m_musicbook.reset();
			m_soundbook.reset();
			m_texturebook.reset();
			m_fontbook.reset();
			m_shaderbook.reset();
			m_scriptbook.reset();
			m_langs.reset();
			m_lua.reset();
			m_window.reset();
			m_config.reset();
			m_vfs.reset();
			m_pool.reset();
		}

		void Application::push_layer(std::shared_ptr<Layer> layer)
		{
			m_layer_stack.push(layer);
		}

		void Application::pop_layer()
		{
			m_layer_stack.pop();
		}

		const bool Application::run()
		{
			const bool   log_perf = m_config->get<bool>("log-perf");
			unsigned int frames   = 0;
			unsigned int updates  = 0;

			constexpr const auto ups_as_nano = std::chrono::duration_cast<std::chrono::nanoseconds>(GALAXY_UPS);
			constexpr const auto one_second  = std::chrono::seconds {1};

			std::chrono::nanoseconds accumulator {0};
			std::chrono::nanoseconds perf_counter {0};
			std::chrono::nanoseconds elapsed {0};
			auto                     previous = std::chrono::high_resolution_clock::now();
			auto                     current  = std::chrono::high_resolution_clock::now();

			while (m_window->is_open())
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
					m_window->poll_events();
					m_layer_stack.top()->events();

					m_layer_stack.top()->update();
					accumulator -= ups_as_nano;

					if (log_perf)
					{
						updates++;
					}
				}

				m_layer_stack.top()->pre_render();

				m_window->begin();
				m_layer_stack.top()->render();
				m_window->end();

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

			// Clean up static stuff here since we can't be sure of when the destructor is called.
			RENDERER_2D().clear();
			FT_HANDLE.close();
			GALAXY_LOG_END();

			return SL_HANDLE.m_restart;
		}

		void Application::create_asset_layout(const std::string& root, const std::string& asset_folder)
		{
			const auto merged = root + asset_folder;

			m_filewatcher.addWatch(merged, m_filelistener.get(), true);

			if (!std::filesystem::exists(merged))
			{
				GALAXY_LOG(GALAXY_INFO, "Missing asset folder, creating at: {0}.", merged);
				std::filesystem::create_directories(merged);
			}
		}

		void Application::generate_default_assets(const std::string& root)
		{
			constexpr const auto font          = "{\"fontbook\": {}}";
			constexpr const auto music         = "{\"musicbook\": {}}";
			constexpr const auto script        = "{\"scriptbook\": {},\"definitions\":[]}";
			constexpr const auto shader        = "{\"vertex-extension\": \".vs\",\"fragment-extension\": \".fs\",\"shaderbook\": []}";
			constexpr const auto sound         = "{\"soundbook\": {}}";
			constexpr const auto atlas         = "{\"textures\": []}";
			constexpr const auto render_layers = "{\"layers\":{\"bottom\": 0, \"top\": 1}}";

			const auto fb_path = root + "json/" + m_config->get<std::string>("fontbook-json");
			if (!std::filesystem::exists(fb_path))
			{
				if (!m_vfs->save(font, fb_path))
				{
					GALAXY_LOG(GALAXY_FATAL, "Failed to create default font assets.");
				}

				GALAXY_LOG(GALAXY_INFO, "Missing default asset, creating: {0}.", fb_path);
			}

			const auto mb_path = root + "json/" + m_config->get<std::string>("musicbook-json");
			if (!std::filesystem::exists(mb_path))
			{
				if (!m_vfs->save(music, mb_path))
				{
					GALAXY_LOG(GALAXY_FATAL, "Failed to create default music assets.");
				}

				GALAXY_LOG(GALAXY_INFO, "Missing default asset, creating: {0}.", mb_path);
			}

			const auto sb_path = root + "json/" + m_config->get<std::string>("scriptbook-json");
			if (!std::filesystem::exists(sb_path))
			{
				if (!m_vfs->save(script, sb_path))
				{
					GALAXY_LOG(GALAXY_FATAL, "Failed to create default script assets.");
				}

				GALAXY_LOG(GALAXY_INFO, "Missing default asset, creating: {0}.", sb_path);
			}

			const auto shb_path = root + "json/" + m_config->get<std::string>("shaderbook-json");
			if (!std::filesystem::exists(shb_path))
			{
				if (!m_vfs->save(shader, shb_path))
				{
					GALAXY_LOG(GALAXY_FATAL, "Failed to create default shader assets.");
				}

				GALAXY_LOG(GALAXY_INFO, "Missing default asset, creating: {0}.", shb_path);
			}

			const auto sfxb_path = root + "json/" + m_config->get<std::string>("soundbook-json");
			if (!std::filesystem::exists(sfxb_path))
			{
				if (!m_vfs->save(sound, sfxb_path))
				{
					GALAXY_LOG(GALAXY_FATAL, "Failed to create default sound assets.");
				}

				GALAXY_LOG(GALAXY_INFO, "Missing default asset, creating: {0}.", sfxb_path);
			}

			const auto tb_path = root + "json/" + m_config->get<std::string>("texturebook-json");
			if (!std::filesystem::exists(tb_path))
			{
				if (!m_vfs->save(atlas, tb_path))
				{
					GALAXY_LOG(GALAXY_FATAL, "Failed to create default texture assets.");
				}

				GALAXY_LOG(GALAXY_INFO, "Missing default asset, creating: {0}.", tb_path);
			}

			const auto rl_path = root + "json/" + m_config->get<std::string>("renderlayers-json");
			if (!std::filesystem::exists(rl_path))
			{
				if (!m_vfs->save(render_layers, rl_path))
				{
					GALAXY_LOG(GALAXY_FATAL, "Failed to create default texture assets.");
				}

				GALAXY_LOG(GALAXY_INFO, "Missing default asset, creating: {0}.", rl_path);
			}
		}

		void
		Application::reload_assets(efsw::WatchID watch_id, const std::string& dir, const std::string& filename, efsw::Action action, std::string old_filename)
		{
			GALAXY_UNUSED(filename);
			GALAXY_UNUSED(action);
			GALAXY_UNUSED(watch_id);

			m_window->request_attention();

			if (dir.find("music") != std::string::npos)
			{
				m_musicbook->clear();
				m_musicbook->create_from_json(m_config->get<std::string>("musicbook-json"));

				GALAXY_LOG(GALAXY_INFO, "Reloading music due to change in filesystem.");
			}

			else if (dir.find("sfx") != std::string::npos)
			{
				m_soundbook->clear();
				m_soundbook->create_from_json(m_config->get<std::string>("soundbook-json"));

				GALAXY_LOG(GALAXY_INFO, "Reloading sfx due to change in filesystem.");
			}

			else if (dir.find("fonts") != std::string::npos)
			{
				m_fontbook->clear();
				m_fontbook->create_from_json(m_config->get<std::string>("fontbook-json"));

				GALAXY_LOG(GALAXY_INFO, "Reloading fonts due to change in filesystem.");
			}

			else if (dir.find("scripts") != std::string::npos)
			{
				m_scriptbook->clear();
				m_scriptbook->create_from_json(m_config->get<std::string>("scriptbook-json"));

				GALAXY_LOG(GALAXY_INFO, "Reloading scripts due to change in filesystem.");
			}

			else if (dir.find("shaders") != std::string::npos)
			{
				m_shaderbook->clear();
				m_shaderbook->create_from_json(m_config->get<std::string>("shaderbook-json"));

				GALAXY_LOG(GALAXY_INFO, "Reloading shaders due to change in filesystem.");
			}

			else if (dir.find("textures") != std::string::npos)
			{
				m_texturebook->clear();
				m_texturebook->add_json(m_config->get<std::string>("texturebook-json"));

				GALAXY_LOG(GALAXY_INFO, "Reloading textures due to change in filesystem.");
			}

			else if (dir.find("lang") != std::string::npos)
			{
				m_langs->clear();
				m_langs->parse_language_folder(dir);
				m_langs->reload();

				GALAXY_LOG(GALAXY_INFO, "Reloading langauges due to change in filesystem.");
			}
		}
	} // namespace core
} // namespace galaxy
*/