///
/// Application.cpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#include <glad/glad.h>
#include <portable-file-dialogs.h>
#include <sol/sol.hpp>

#include "galaxy/core/ServiceLocator.hpp"
#include "galaxy/fs/FileSystem.hpp"
#include "galaxy/graphics/Colour.hpp"
#include "galaxy/graphics/text/FreeType.hpp"
#include "galaxy/graphics/Renderer2D.hpp"
#include "galaxy/graphics/Renderer3D.hpp"
#include "galaxy/graphics/shaders/BlinnPhong.hpp"
#include "galaxy/graphics/SpriteBatch.hpp"
#include "galaxy/scripting/LuaUtils.hpp"

#include "Application.hpp"

namespace galaxy
{
	namespace core
	{
		Application::Application(std::string_view asset_dir, std::string_view config_file)
		    : m_instance {nullptr}, m_openal {}
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
				create_asset_layout(root, "models/");
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
				m_config->define<int>("ansio-filter", 1);
				m_config->define<bool>("vsync", true);
				m_config->define<int>("aspect-ratio-x", -1);
				m_config->define<int>("aspect-ratio-y", -1);
				m_config->define<bool>("raw-mouse-input", true);
				m_config->define<std::string>("window-name", "Title");
				m_config->define<int>("window-width", 1280);
				m_config->define<int>("window-height", 720);
				m_config->define<bool>("is-cursor-visible", true);
				m_config->define<bool>("gl-debug", false);
				m_config->define<bool>("trilinear-filtering", false);
				m_config->define<bool>("maximized", false);
				m_config->define<bool>("log-perf", false);
				m_config->define<int>("max-batched-quads", 2048);
				m_config->define<float>("audio-volume", 0.7f);
				m_config->define<float>("mouse-sensitivity", 1.0f);
				m_config->define<float>("camera-near", 0.1f);
				m_config->define<float>("camera-far", 100.0f);
				m_config->define<float>("camera-fov", 90.0f);
				m_config->define<int>("shadow-size", 1024);
				m_config->define<std::string>("cursor-image", "cursor.png");
				m_config->define<std::string>("icon-file", "icon.png");
				m_config->define<std::string>("fontbook-json", "fontbook.json");
				m_config->define<std::string>("shaderbook-json", "shaderbook.json");
				m_config->define<std::string>("scriptbook-json", "scriptbook.json");
				m_config->define<std::string>("textureatlas-json", "textureatlas.json");
				m_config->define<std::string>("soundbook-json", "soundbook.json");
				m_config->define<std::string>("musicbook-json", "musicbook.json");
				m_config->define<std::string>("spritebatch-shader", "spritebatch");
				m_config->define<std::string>("2d-fb-shader", "2d_framebuffer");
				m_config->define<std::string>("key-forward", "W");
				m_config->define<std::string>("key-back", "S");
				m_config->define<std::string>("key-left", "A");
				m_config->define<std::string>("key-right", "D");
				m_config->define<std::string>("key-freecam-up", "Q");
				m_config->define<std::string>("key-freecam-down", "E");
			}
			m_config->save();

			// Window.
			// clang-format off
			core::WindowSettings settings
			{
				.m_vsync = m_config->get<bool>("vsync"),
				.m_aspect_ratio_x = m_config->get<int>("aspect-ratio-x"),
				.m_aspect_ratio_y = m_config->get<int>("aspect-ratio-y"),
				.m_raw_mouse_input = m_config->get<bool>("raw-mouse-input"),
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
				m_window->request_attention();

				if (!settings.m_raw_mouse_input)
				{
					const bool cursor = m_config->get<bool>("is-cursor-visible");
					m_window->set_cursor_visibility(cursor);

					if (cursor)
					{
						const auto cursor_image = m_config->get<std::string>("cursor-image");
						if (cursor_image != "")
						{
							m_window->set_cursor_icon(cursor_image);
						}
					}
				}

				const auto icon_file = m_config->get<std::string>("icon-file");
				if (icon_file != "")
				{
					m_window->set_icon(icon_file);
				}

				// Configure audio context.
				m_openal.set_listener_gain(m_config->get<float>("audio-volume"));
				SL_HANDLE.m_openal = &m_openal;

				// Freetype.
				FT_HANDLE.open();

				// Create lua instance and open libraries.
				m_lua = std::make_unique<sol::state>();
				m_lua->open_libraries(sol::lib::base, sol::lib::package, sol::lib::coroutine, sol::lib::string, sol::lib::os, sol::lib::math, sol::lib::table, sol::lib::io, sol::lib::utf8);
				SL_HANDLE.m_lua = m_lua.get();

				// Generate default assets specified by config.
				generate_default_assets(root);

				// Parse languages.
				m_language = std::make_unique<res::Language>();
				m_language->parse_language_folder(root + "lang/");
				m_language->set_language("en_au");
				SL_HANDLE.m_language = m_language.get();

				// ShaderBook.
				m_shaderbook           = std::make_unique<res::ShaderBook>(m_config->get<std::string>("shaderbook-json"));
				SL_HANDLE.m_shaderbook = m_shaderbook.get();

				// Set up renderers.
				RENDERER_2D().init(m_config->get<int>("max-batched-quads"), m_config->get<std::string>("spritebatch-shader"), m_config->get<std::string>("2d-fb-shader"));

				RENDERER_3D().get_gbuffer().init(m_window->get_width(), m_window->get_height());
				RENDERER_3D().get_gbuffer().add_attachments(3, graphics::GeomBuffer::AttachmentType::FLOAT);
				RENDERER_3D().get_gbuffer().add_attachments(1, graphics::GeomBuffer::AttachmentType::UNSIGNED_BYTE);
				RENDERER_3D().get_gbuffer().create();
				RENDERER_3D().init(m_window->get_width(), m_window->get_height());
				RENDERER_3D().reserve_ubo(0, sizeof(graphics::Camera3D::Data));

				const auto r3d_light_frag = std::format("{0}{1}{2}", shaders::blinnphong_frag_1, shaders::blinnphong_frag_2, shaders::blinnphong_frag_3);
				RENDERER_3D().add_lighting_shader(shaders::blinnphong_vert, r3d_light_frag);

				// ScriptBook.
				m_scriptbook           = std::make_unique<res::ScriptBook>(m_config->get<std::string>("scriptbook-json"));
				SL_HANDLE.m_scriptbook = m_scriptbook.get();

				// FontBook.
				m_fontbook           = std::make_unique<res::FontBook>(m_config->get<std::string>("fontbook-json"));
				SL_HANDLE.m_fontbook = m_fontbook.get();

				// Texture Atlas.
				m_texture_atlas = std::make_unique<res::TextureAtlas>(m_config->get<std::string>("textureatlas-json"));
				m_texture_atlas->create("render_to_texture");
				RENDERER_2D().create_default_batches(m_texture_atlas->get_atlas());
				SL_HANDLE.m_texture_atlas = m_texture_atlas.get();

				// SoundBook.
				m_soundbook           = std::make_unique<res::SoundBook>(m_config->get<std::string>("soundbook-json"));
				SL_HANDLE.m_soundbook = m_soundbook.get();

				// MusicBook.
				m_musicbook           = std::make_unique<res::MusicBook>(m_config->get<std::string>("musicbook-json"));
				SL_HANDLE.m_musicbook = m_musicbook.get();

				// MaterialBook.
				m_materialbook           = std::make_unique<res::MaterialBook>();
				SL_HANDLE.m_materialbook = m_materialbook.get();

				// Set up custom lua functions and types.
				lua::register_functions();
				lua::register_audio();
				lua::register_ecs();
				lua::register_events();
				lua::register_fs();
				lua::register_graphics();
				lua::register_mapping();
				lua::register_math();
				lua::register_physics();
				lua::register_platform();
				lua::register_res();
				lua::register_scripting();

				// Register services with Lua.
				m_lua->set("galaxy_audio_context", &m_openal);
				m_lua->set("galaxy_config", m_config.get());
				m_lua->set("galaxy_vfs", m_vfs.get());
				m_lua->set("galaxy_shaderboox", m_shaderbook.get());
				m_lua->set("galaxy_fontbook", m_fontbook.get());
				m_lua->set("galaxy_tex_atlas", m_texture_atlas.get());
				m_lua->set("galaxy_soundbook", m_soundbook.get());
				m_lua->set("galaxy_musicbook", m_musicbook.get());
				m_lua->set("galaxy_scriptbook", m_scriptbook.get());
				m_lua->set("galaxy_language", m_language.get());

				// Bind filesystem listener.
				m_vfs->m_listener.m_on_file_change = std::bind(&Application::reload_assets, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3, std::placeholders::_4);
			}
		}

		Application::~Application()
		{
			// We want to destroy everything in a specific order to make sure stuff is freed correctly.
			// And of course the file system being the last to be destroyed.
			m_pool->finish();
			m_window->destroy();

			m_instance = nullptr;

			m_materialbook.reset();
			m_musicbook.reset();
			m_soundbook.reset();
			m_texture_atlas.reset();
			m_fontbook.reset();
			m_shaderbook.reset();
			m_scriptbook.reset();
			m_language.reset();
			m_lua.reset();
			m_window.reset();
			m_config.reset();
			m_vfs.reset();
			m_pool.reset();
		}

		void Application::set_instance(std::shared_ptr<Instance> instance)
		{
			m_instance = instance;
		}

		const bool Application::run()
		{
			const bool log_perf  = m_config->get<bool>("log-perf");
			unsigned int frames  = 0;
			unsigned int updates = 0;

			using clock     = std::chrono::high_resolution_clock;
			using ups_ratio = std::chrono::duration<double, std::ratio<1, 60>>;

			constexpr const ups_ratio ups {1};
			constexpr const auto ups_as_nano = std::chrono::duration_cast<std::chrono::nanoseconds>(ups);
			constexpr const auto ups_s       = std::chrono::duration_cast<std::chrono::milliseconds>(ups).count() / 1000.0;
			constexpr const auto one_second  = std::chrono::seconds {1};

			std::chrono::nanoseconds accumulator {0};
			std::chrono::nanoseconds perf_counter {0};
			std::chrono::nanoseconds elapsed {0};
			auto previous = clock::now();
			auto current  = clock::now();

			while (m_window->is_open())
			{
				current  = clock::now();
				elapsed  = current - previous;
				previous = current;
				accumulator += elapsed;

				if (log_perf)
				{
					perf_counter += elapsed;
				}

				while (accumulator >= ups)
				{
					m_window->poll_events();
					m_instance->events();

					m_vfs->m_watcher.update();
					m_instance->update(ups_s);
					accumulator -= ups_as_nano;

					if (log_perf)
					{
						updates++;
					}
				}

				m_instance->pre_render();

				m_window->begin();
				m_instance->render();
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
			RENDERER_2D().clean_up();
			RENDERER_3D().clean_up();
			FT_HANDLE.close();
			GALAXY_LOG_FINISH;

			return SL_HANDLE.m_restart;
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

		void Application::generate_default_assets(const std::string& root)
		{
			constexpr const char* const font   = "{\"fontbook\":{}}";
			constexpr const char* const music  = "{\"musicbook\":{}}";
			constexpr const char* const script = "{\"scriptbook\":{}}";
			constexpr const char* const shader = "{\"vertex-extension\":\".vs\",\"fragment-extension\":\".fs\",\"shaderbook\":[]}";
			constexpr const char* const sound  = "{\"soundbook\":{}}";
			constexpr const char* const atlas  = "{\"textures\":[]}";

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

			const auto ab_path = root + "json/" + m_config->get<std::string>("textureatlas-json");
			if (!std::filesystem::exists(ab_path))
			{
				if (!m_vfs->save(atlas, ab_path))
				{
					GALAXY_LOG(GALAXY_FATAL, "Failed to create default texture assets.");
				}

				GALAXY_LOG(GALAXY_INFO, "Missing default asset, creating: {0}.", ab_path);
			}
		}

		void Application::reload_assets(FW::WatchID watch_id, const FW::String& dir, const FW::String& file_name, FW::Action action)
		{
			m_window->request_attention();

			// NOTE: We dont need to reload 3D renderer since it has no static data.

			if (dir.find("shaders") != std::string::npos)
			{
				// ShaderBook.
				m_shaderbook->clear();
				m_shaderbook->create_from_json(m_config->get<std::string>("shaderbook-json"));

				// Reconfigure 2D renderer.
				RENDERER_2D().clean_up();
				RENDERER_2D().init(m_config->get<int>("max-batched-quads"), m_config->get<std::string>("spritebatch-shader"), m_config->get<std::string>("2d-fb-shader"));

				GALAXY_LOG(GALAXY_INFO, "Reloading shaders due to change in filesystem.");
			}
			else if (dir.find("scripts") != std::string::npos)
			{
				// ScriptBook.
				m_scriptbook->clear();
				m_scriptbook->create_from_json(m_config->get<std::string>("scriptbook-json"));

				GALAXY_LOG(GALAXY_INFO, "Reloading scripts due to change in filesystem.");
			}
			else if (dir.find("fonts") != std::string::npos)
			{
				// FontBook.
				m_fontbook->clear();
				m_fontbook->create_from_json(m_config->get<std::string>("fontbook-json"));

				GALAXY_LOG(GALAXY_INFO, "Reloading fonts due to change in filesystem.");
			}
			else if (dir.find("textures") != std::string::npos)
			{
				// Texture Atlas.
				m_texture_atlas->clear();
				m_texture_atlas->add_from_json(m_config->get<std::string>("textureatlas-json"));
				m_texture_atlas->create("render_to_texture");
				RENDERER_2D().create_default_batches(m_texture_atlas->get_atlas());

				GALAXY_LOG(GALAXY_INFO, "Reloading textures due to change in filesystem.");
			}
			else if (dir.find("sfx") != std::string::npos)
			{
				// SoundBook.
				m_soundbook->clear();
				m_soundbook->create_from_json(m_config->get<std::string>("soundbook-json"));

				GALAXY_LOG(GALAXY_INFO, "Reloading sfx due to change in filesystem.");
			}
			else if (dir.find("music") != std::string::npos)
			{
				// MusicBook.
				m_musicbook->clear();
				m_musicbook->create_from_json(m_config->get<std::string>("musicbook-json"));

				GALAXY_LOG(GALAXY_INFO, "Reloading music due to change in filesystem.");
			}
			else if (dir.find("maps") != std::string::npos)
			{
				static_assert("Not yet implemented");
			}
			else if (dir.find("models") != std::string::npos)
			{
				static_assert("Not yet implemented");
			}
		}
	} // namespace core
} // namespace galaxy