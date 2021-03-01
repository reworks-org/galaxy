///
/// Application.cpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#include <sstream>

#include <fmt/format.h>
#include <glad/glad.h>
#include <portable-file-dialogs.h>
#include <sol/sol.hpp>

#include "galaxy/core/ServiceLocator.hpp"
#include "galaxy/scripting/LuaUtils.hpp"
#include "galaxy/fs/FileSystem.hpp"
#include "galaxy/graphics/Colour.hpp"
#include "galaxy/graphics/text/FreeType.hpp"
#include "galaxy/graphics/Renderer2D.hpp"
#include "galaxy/graphics/SpriteBatch.hpp"

#include "Application.hpp"

namespace galaxy
{
	namespace core
	{
		Application::Application(std::string_view asset_dir, std::string_view config_file)
		    : m_openal {}
		{
			// Seed pseudo-random algorithms.
			std::srand(static_cast<unsigned int>(std::time(nullptr)));

			// Set up all of the difference services.
			// The services are configured based off of the config file.
			// Services are created in dependency order.

			// Log.
			const auto time = std::time(nullptr);
			std::stringstream sstream;
			sstream << std::put_time(std::localtime(&time), "%d-%m-%Y-[%H-%M]");

			const std::string log_path = fmt::format("{0}{1}{2}", "logs/", sstream.str(), ".log");
			if (!std::filesystem::exists("logs/"))
			{
				std::filesystem::create_directory("logs");
			}

			GALAXY_LOG_START(log_path);

			// Filesystem.
			m_vfs = std::make_unique<fs::Virtual>();
			if (!m_vfs->mount(asset_dir))
			{
				GALAXY_LOG(GALAXY_FATAL, "Failed to mount VFS.");
			}
			SL_HANDLE.m_vfs = m_vfs.get();

			// Config reader.
			m_config           = std::make_unique<fs::Config>();
			SL_HANDLE.m_config = m_config.get();

			m_config->init(config_file);
			if (m_config->is_blank())
			{
				m_config->define<int>("anti-aliasing", 1);
				m_config->define<int>("ansio-filter", 1);
				m_config->define<bool>("vsync", true);
				m_config->define<int>("aspect-ratio-x", -1);
				m_config->define<int>("aspect-ratio-y", -1);
				m_config->define<bool>("raw-mouse-input", true);
				m_config->define<std::string>("window-name", "Title");
				m_config->define<int>("window-width", 1280);
				m_config->define<int>("window-height", 720);
				m_config->define<float>("line-thickness", 1.0f);
				m_config->define<bool>("is-cursor-visible", true);
				m_config->define<bool>("gl-debug", false);
				m_config->define<bool>("trilinear-filtering", false);
				m_config->define<int>("max-batched-quads", 1000);
				m_config->define<float>("audio-volume", 0.7f);
				m_config->define<std::string>("cursor-image", "cursor.png");
				m_config->define<std::string>("icon-file", "icon.png");
				m_config->define<std::string>("fontbook-json", "fontbook.json");
				m_config->define<std::string>("shaderbook-json", "shaderbook.json");
				m_config->define<std::string>("textureatlas-json", "textureatlas.json");
				m_config->define<std::string>("soundbook-json", "soundbook.json");
				m_config->define<std::string>("musicbook-json", "musicbook.json");
				m_config->define<std::string>("spritebatch-shader", "spritebatch");
			}
			m_config->save();

			// Window.
			// clang-format off
			core::WindowSettings settings
			{
				.m_anti_aliasing = m_config->get<int>("anti-aliasing"),
				.m_ansio_filtering = m_config->get<int>("ansio-filter"),
				.m_vsync = m_config->get<bool>("vsync"),
				.m_aspect_ratio_x = m_config->get<int>("aspect-ratio-x"),
				.m_aspect_ratio_y = m_config->get<int>("aspect-ratio-y"),
				.m_raw_mouse_input = m_config->get<bool>("raw-mouse-input"),
				.m_line_thickness = m_config->get<float>("line-thickness"),
				.m_gl_debug = m_config->get<bool>("gl-debug"),
				.m_title = m_config->get<std::string>("window-name"),
				.m_width = m_config->get<int>("window-width"),
				.m_height = m_config->get<int>("window-height")
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

				const auto icon_file = m_config->get<std::string>("icon-file");
				if (icon_file != "")
				{
					m_window->set_icon(icon_file);
				}

				// Configure audio context.
				m_openal.set_listener_gain(m_config->get<float>("audio-volume"));

				// Freetype.
				FT_HANDLE.open();

				// Create lua instance and open libraries.
				m_lua = std::make_unique<sol::state>();
				m_lua->open_libraries(sol::lib::base, sol::lib::package, sol::lib::coroutine, sol::lib::string, sol::lib::os, sol::lib::math, sol::lib::table, sol::lib::io, sol::lib::utf8);
				SL_HANDLE.m_lua = m_lua.get();

				// Layer Stack.
				m_layerstack       = std::make_unique<LayerStack>();
				SL_HANDLE.m_layers = m_layerstack.get();

				// Event dispatcher.
				m_dispatcher           = std::make_unique<events::Dispatcher>();
				SL_HANDLE.m_dispatcher = m_dispatcher.get();

				// ShaderBook.
				m_shaderbook           = std::make_unique<res::ShaderBook>(m_config->get<std::string>("shaderbook-json"));
				SL_HANDLE.m_shaderbook = m_shaderbook.get();

				// Set up renderer.
				graphics::Renderer2D::init(m_config->get<int>("max-batched-quads"), m_config->get<std::string>("spritebatch-shader"));

				// FontBook.
				m_fontbook           = std::make_unique<res::FontBook>(m_config->get<std::string>("fontbook-json"));
				SL_HANDLE.m_fontbook = m_fontbook.get();

				// Texture Atlas.
				m_texture_atlas = std::make_unique<res::TextureAtlas>(m_config->get<std::string>("textureatlas-json"));
				m_texture_atlas->create("render_to_texture");
				graphics::Renderer2D::m_batch->set_texture(m_texture_atlas->get_atlas());
				SL_HANDLE.m_texture_atlas = m_texture_atlas.get();

				// SoundBook.
				m_soundbook           = std::make_unique<res::SoundBook>(m_config->get<std::string>("soundbook-json"));
				SL_HANDLE.m_soundbook = m_soundbook.get();

				// MusicBook.
				m_musicbook           = std::make_unique<res::MusicBook>(m_config->get<std::string>("musicbook-json"));
				SL_HANDLE.m_musicbook = m_musicbook.get();

				// Set up custom lua functions and types.
				lua::register_functions();
				lua::register_audio();
				lua::register_ecs();
				lua::register_events();
				lua::register_fs();
				lua::register_graphics();
				lua::register_json();
				lua::register_layerstack();
				lua::register_mapping();
				lua::register_math();
				lua::register_physics();
				lua::register_res();

				// Register services with Lua.
				m_lua->set("galaxy_audio_context", &m_openal);
				m_lua->set("galaxy_config", m_config.get());
				m_lua->set("galaxy_vfs", m_vfs.get());
				m_lua->set("galaxy_layers", m_layerstack.get());
				m_lua->set("galaxy_shaderboox", m_shaderbook.get());
				m_lua->set("galaxy_fontbook", m_fontbook.get());
				m_lua->set("galaxy_tex_atlas", m_texture_atlas.get());
				m_lua->set("galaxy_soundbook", m_soundbook.get());
				m_lua->set("galaxy_musicbook", m_musicbook.get());
			}
		}

		Application::~Application()
		{
			// We want to destroy everything in a specific order to make sure stuff is freed correctly.
			// And of course the file system being the last to be destroyed.

			m_musicbook.reset();
			m_soundbook.reset();
			m_texture_atlas.reset();
			m_fontbook.reset();
			m_shaderbook.reset();
			m_dispatcher.reset();
			m_layerstack.reset();
			m_lua.reset();
			m_window.reset();
			m_config.reset();
			m_vfs.reset();
		}

		const bool Application::run()
		{
			using clock     = std::chrono::high_resolution_clock;
			using ups_ratio = std::chrono::duration<double, std::ratio<1, 60>>;

			const constexpr ups_ratio ups {1};
			const constexpr auto ups_as_nano = std::chrono::duration_cast<std::chrono::nanoseconds>(ups);
			const constexpr auto ups_s       = std::chrono::duration_cast<std::chrono::milliseconds>(ups).count() / 1000.0;

			std::chrono::nanoseconds accumulator {0};
			std::chrono::nanoseconds elapsed {0};
			auto previous = clock::now();
			auto current  = clock::now();

			while (m_window->is_open())
			{
				current  = clock::now();
				elapsed  = current - previous;
				previous = current;
				accumulator += elapsed;

				while (accumulator >= ups)
				{
					m_window->poll_events();
					m_layerstack->events();

					m_layerstack->update(ups_s);
					accumulator -= ups_as_nano;
				}

				m_layerstack->pre_render();

				m_window->begin();

				m_layerstack->render();

				m_window->end();
			}

			m_layerstack->clear();
			GALAXY_LOG_FINISH;

			FT_HANDLE.close();

			m_window->destroy();
			graphics::Renderer2D::clean_up();

			return SL_HANDLE.m_restart;
		}
	} // namespace core
} // namespace galaxy