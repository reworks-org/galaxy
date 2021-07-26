///
/// Application.cpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#include <portable-file-dialogs.h>
#include <sol/sol.hpp>
#include <RmlUi/Core.h>

#include "galaxy/core/GalaxyConfig.hpp"
#include "galaxy/core/LoadingScreen.hpp"
#include "galaxy/core/ServiceLocator.hpp"
#include "galaxy/fs/FileSystem.hpp"
#include "galaxy/graphics/Colour.hpp"
#include "galaxy/graphics/text/FreeType.hpp"
#include "galaxy/graphics/Renderer2D.hpp"
#include "galaxy/graphics/SpriteBatch.hpp"
#include "galaxy/scripting/LuaUtils.hpp"

#include "Application.hpp"

namespace galaxy
{
	namespace core
	{
		Application::Application(std::string_view asset_dir, std::string_view config_file)
			: m_filewatcher {false}
			, m_filelistener {nullptr}
		{
			// Seed pseudo-random algorithms.
			std::srand(static_cast<unsigned int>(std::time(nullptr)));

			// Set up all of the difference services.
			// The services are configured based off of the config file.
			// Services are created in dependency order.

			// Log.
			const auto now             = std::chrono::zoned_time {std::chrono::current_zone(), std::chrono::system_clock::now()}.get_local_time();
			const std::string log_path = std::format("{0}{1}{2}", "logs/", std::format("{0:%d-%m-%Y-[%H-%M]}", now), ".log");
			if (!std::filesystem::exists("logs/"))
			{
				std::filesystem::create_directory("logs");
			}

			GALAXY_LOG_START(log_path);

			// Threadpool setup.
			m_pool           = std::make_unique<async::ThreadPool>();
			SL_HANDLE.m_pool = m_pool.get();

			// Virtual filesystem setup.
			m_filelistener = std::make_unique<fs::FileListener>();
			m_filelistener->set_action(std::bind(&Application::reload_assets,
												 this,
												 std::placeholders::_1,
												 std::placeholders::_2,
												 std::placeholders::_3,
												 std::placeholders::_4,
												 std::placeholders::_5));

			auto root = static_cast<std::string>(asset_dir);
			if (root.back() != '/')
			{
				root += '/';
			}

			m_vfs = std::make_unique<fs::Virtual>();
			create_asset_layout(root, "");

			if (!m_vfs->mount(root))
			{
				GALAXY_LOG(GALAXY_FATAL, "Failed to mount VFS.");
			}
			else
			{
				// Generate default asset folder layout.
				create_asset_layout(root, "audio/music/");
				create_asset_layout(root, "audio/sfx/");
				create_asset_layout(root, "fonts/");
				create_asset_layout(root, "json/");
				create_asset_layout(root, "scripts/");
				create_asset_layout(root, "shaders/");
				create_asset_layout(root, "textures/");
				create_asset_layout(root, "maps/");
				create_asset_layout(root, "lang/");
			}
			SL_HANDLE.m_vfs = m_vfs.get();

			// Config reader.
			m_config           = std::make_unique<fs::Config>();
			SL_HANDLE.m_config = m_config.get();

			m_config->init(config_file);
			if (m_config->is_blank())
			{
				m_config->define<bool>("anti-aliasing", false);
				m_config->define<bool>("sharpen", false);
				m_config->define<int>("ansio-filter", 1);
				m_config->define<bool>("vsync", true);
				m_config->define<int>("aspect-ratio-x", -1);
				m_config->define<int>("aspect-ratio-y", -1);
				m_config->define<std::string>("window-name", "Title");
				m_config->define<int>("window-width", 1280);
				m_config->define<int>("window-height", 720);
				m_config->define<bool>("is-cursor-visible", true);
				m_config->define<bool>("gl-debug", false);
				m_config->define<bool>("trilinear-filtering", false);
				m_config->define<bool>("maximized", false);
				m_config->define<bool>("log-perf", false);
				m_config->define<float>("audio-volume", 0.7f);
				m_config->define<std::string>("logo", "logo.png");
				m_config->define<std::string>("cursor-image", "cursor.png");
				m_config->define<std::string>("icon-file", "icon.png");
				m_config->define<std::string>("fontbook-json", "fontbook.json");
				m_config->define<std::string>("shaderbook-json", "shaderbook.json");
				m_config->define<std::string>("scriptbook-json", "scriptbook.json");
				m_config->define<std::string>("texturebook-json", "texturebook.json");
				m_config->define<std::string>("soundbook-json", "soundbook.json");
				m_config->define<std::string>("musicbook-json", "musicbook.json");
				m_config->define<std::string>("renderlayers-json", "renderlayers.json");
				m_config->define<std::string>("key-forward", "W");
				m_config->define<std::string>("key-back", "S");
				m_config->define<std::string>("key-left", "A");
				m_config->define<std::string>("key-right", "D");
			}
			m_config->save();

			// Window.
			// clang-format off
			core::WindowSettings settings
			{
				.m_vsync = m_config->get<bool>("vsync"),
				.m_aspect_ratio_x = m_config->get<int>("aspect-ratio-x"),
				.m_aspect_ratio_y = m_config->get<int>("aspect-ratio-y"),
				.m_gl_debug = m_config->get<bool>("gl-debug"),
				.m_title = m_config->get<std::string>("window-name"),
				.m_width = m_config->get<int>("window-width"),
				.m_height = m_config->get<int>("window-height"),
				.m_maximized = m_config->get<bool>("maximized")
			};
			// clang-format on

			m_window           = std::make_unique<core::Window>();
			SL_HANDLE.m_window = m_window.get();
			if (!m_window->create(settings))
			{
				GALAXY_LOG(GALAXY_FATAL, "Failed to create window! Aborting...");
			}
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

				// ShaderBook.
				m_shaderbook           = std::make_unique<res::ShaderBook>(m_config->get<std::string>("shaderbook-json"));
				SL_HANDLE.m_shaderbook = m_shaderbook.get();

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
				lua::register_functions();

				// Register services with Lua.
				m_lua->set("galaxy_audio_context", &m_openal);
				m_lua->set("galaxy_config", m_config.get());
				m_lua->set("galaxy_vfs", m_vfs.get());
				m_lua->set("galaxy_shaderboox", m_shaderbook.get());
				m_lua->set("galaxy_fontbook", m_fontbook.get());
				m_lua->set("galaxy_texturebook", m_texturebook.get());
				m_lua->set("galaxy_soundbook", m_soundbook.get());
				m_lua->set("galaxy_musicbook", m_musicbook.get());
				m_lua->set("galaxy_scriptbook", m_scriptbook.get());
				m_lua->set("galaxy_language", m_langs.get());

				// Begin watching files now that default asset creation is over.
				m_filewatcher.watch();

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
					for (auto& [key, font] : m_fontbook->cache())
					{
						if (!Rml::LoadFontFace(font->get_filename()))
						{
							GALAXY_LOG(GALAXY_ERROR, "Failed to load RML font: {0}.", key);
						}
					}
				}

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
			const bool log_perf  = m_config->get<bool>("log-perf");
			unsigned int frames  = 0;
			unsigned int updates = 0;

			constexpr const auto ups_as_nano = std::chrono::duration_cast<std::chrono::nanoseconds>(GALAXY_UPS);
			constexpr const auto one_second  = std::chrono::seconds {1};

			std::chrono::nanoseconds accumulator {0};
			std::chrono::nanoseconds perf_counter {0};
			std::chrono::nanoseconds elapsed {0};
			auto previous = std::chrono::high_resolution_clock::now();
			auto current  = std::chrono::high_resolution_clock::now();

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
			GALAXY_LOG_FINISH;

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
			constexpr const char* const font          = "{\"fontbook\": {}}";
			constexpr const char* const music         = "{\"musicbook\": {}}";
			constexpr const char* const script        = "{\"scriptbook\": {}}";
			constexpr const char* const shader        = "{\"vertex-extension\": \".vs\",\"fragment-extension\": \".fs\",\"shaderbook\": []}";
			constexpr const char* const sound         = "{\"soundbook\": {}}";
			constexpr const char* const atlas         = "{\"textures\": []}";
			constexpr const char* const render_layers = "{\"layers\":{\"bottom\": 0, \"top\": 1}}";

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