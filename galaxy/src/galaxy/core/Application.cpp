///
/// Application.cpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#include <fmt/format.h>
#include <glad/glad.h>
#include <sol/sol.hpp>

#include "galaxy/core/ServiceLocator.hpp"
#include "galaxy/scripting/LuaUtils.hpp"
#include "galaxy/fs/FileSystem.hpp"
#include "galaxy/graphics/Colour.hpp"
#include "galaxy/graphics/WindowSettings.hpp"
#include "galaxy/graphics/text/FreeType.hpp"

#include "Application.hpp"

namespace galaxy
{
	namespace core
	{
		Application::Application(std::unique_ptr<fs::Config>& config)
		    : m_visible_tools {true}
		{
			// Seed pseudo-random algorithms.
			std::srand(static_cast<unsigned int>(std::time(nullptr)));

			// Set up all of the difference services.
			// The services are configured based off of the config file.

			// Config reader.
			if (!config)
			{
				GALAXY_LOG(GALAXY_FATAL, "Failed to initialize config.");
			}
			else
			{
				m_config           = std::move(config);
				SL_HANDLE.m_config = m_config.get();
			}

			// FS paths.
			fs::s_root     = m_config->get<std::string>("root-path");
			fs::s_textures = m_config->get<std::string>("textures-path");
			fs::s_shaders  = m_config->get<std::string>("shaders-path");
			fs::s_scripts  = m_config->get<std::string>("scripts-path");
			fs::s_audio    = m_config->get<std::string>("audio-path");
			fs::s_json     = m_config->get<std::string>("json-path");
			fs::s_fonts    = m_config->get<std::string>("font-path");
			fs::s_saves    = m_config->get<std::string>("save-folder");

			// threadpool
			m_threadpool = std::make_unique<async::ThreadPool<4>>();
			m_threadpool->start();
			SL_HANDLE.m_threadpool = m_threadpool.get();

			// Window
			// clang-format off
			graphics::WindowSettings settings
			{
				.m_anti_aliasing = m_config->get<int>("anti-aliasing"),
				.m_ansio_filtering = m_config->get<int>("ansio-filter"),
				.m_vsync = m_config->get<bool>("vsync"),
				.m_srgb = m_config->get<bool>("srgb"),
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

			m_window           = std::make_unique<graphics::Window>();
			SL_HANDLE.m_window = m_window.get();
			if (!m_window->create(settings))
			{
				GALAXY_LOG(GALAXY_FATAL, "Failed to create window! Aborting...");
			}
			else
			{
				m_window->request_attention();

				bool cursor = m_config->get<bool>("is-cursor-visible");
				m_window->set_cursor_visibility(cursor);
				if (cursor)
				{
					auto cursor_path = fs::s_root + fs::s_textures + m_config->get<std::string>("cursor-image");
					m_window->set_cursor_icon(cursor_path);
				}

				auto icon_path = fs::s_root + fs::s_textures + m_config->get<std::string>("icon-file");
				m_window->set_icon(icon_path);

				// renderer
				m_renderer           = std::make_unique<graphics::Renderer>();
				SL_HANDLE.m_renderer = m_renderer.get();

				// Freetype.
				FTLIB.open();

				// Create lua instance and open libraries.
				m_lua = std::make_unique<sol::state>();
				m_lua->open_libraries(sol::lib::base, sol::lib::package, sol::lib::coroutine, sol::lib::string, sol::lib::os, sol::lib::math, sol::lib::table, sol::lib::io, sol::lib::utf8);
				SL_HANDLE.m_lua = m_lua.get();

				// State Machine.
				m_state           = std::make_unique<StateMachine>();
				SL_HANDLE.m_state = m_state.get();

				// Event dispatcher.
				m_dispatcher           = std::make_unique<events::Dispatcher>();
				SL_HANDLE.m_dispatcher = m_dispatcher.get();

				// Game "world".
				m_world           = std::make_unique<World>();
				SL_HANDLE.m_world = m_world.get();

				// Serializer.
				m_serializer           = std::make_unique<fs::Serializer>();
				SL_HANDLE.m_serializer = m_serializer.get();

				// ShaderBook
				m_shaderbook           = std::make_unique<res::ShaderBook>(m_config->get<std::string>("shaderbook-json"));
				SL_HANDLE.m_shaderbook = m_shaderbook.get();

				// FontBook
				m_fontbook           = std::make_unique<res::FontBook>(m_config->get<std::string>("fontbook-json"));
				SL_HANDLE.m_fontbook = m_fontbook.get();

				// Texture Atlas.
				m_texture_atlas           = std::make_unique<graphics::TextureAtlas>();
				SL_HANDLE.m_texture_atlas = m_texture_atlas.get();

				// Set up custom lua functions and types.
				lua::register_functions();
				lua::register_audio();

				// Setup dev tools.
				m_tools.create();
			}
		}

		Application::~Application()
		{
			// We want to destroy everything in a specific order to make sure stuff is freed correctly.
			// And of course the file system being the last to be destroyed.

			m_world.reset();
			m_dispatcher.reset();
			m_state.reset();
			m_lua.reset();
			m_renderer.reset();
			m_window.reset();
			m_threadpool.reset();
			m_config.reset();
		}

		bool Application::run()
		{
			using clock     = std::chrono::high_resolution_clock;
			using ups_ratio = std::chrono::duration<double, std::ratio<1, 60>>;

			const constexpr ups_ratio ups {1};
			const constexpr auto ups_as_nano = std::chrono::duration_cast<std::chrono::nanoseconds>(ups);
			const constexpr auto ups_s       = std::chrono::duration_cast<std::chrono::milliseconds>(ups).count() / 1000.0;

			std::chrono::nanoseconds accumulator {0};
			auto previous = clock::now();
			auto current  = clock::now();
			auto elapsed  = current - previous;
			while (m_window->is_open())
			{
				current  = clock::now();
				elapsed  = current - previous;
				previous = current;
				accumulator += elapsed;

				m_window->poll_events();
				m_state->events();

				if (m_window->key_pressed(input::Keys::GRAVE))
				{
					m_visible_tools = !m_visible_tools;
				}

				while (accumulator >= ups)
				{
					m_state->update(ups_s);
					accumulator -= ups_as_nano;
				}

				m_state->pre_render();

				if (m_visible_tools)
				{
					m_tools.prepare();
				}

				m_window->begin();

				m_state->render();

				if (m_visible_tools)
				{
					m_tools.draw();
				}

				m_window->end(m_renderer.get());
			}

			m_state->clear();
			GALAXY_LOG_FINISH;

			FTLIB.close();

			m_threadpool->end();
			m_window->destroy();

			return SL_HANDLE.m_restart;
		}
	} // namespace core
} // namespace galaxy