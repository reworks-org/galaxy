///
/// Application.cpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#include <fmt/format.h>
#include <galaxy/scripting/LuaUtils.hpp>
#include <glad/glad.h>
#include <protostar/graphics/Colour.hpp>
#include <protostar/system/Time.hpp>
#include <qs/core/WindowSettings.hpp>
#include <qs/text/FreeType.hpp>
#include <sol/sol.hpp>

#include "galaxy/core/ServiceLocator.hpp"
#include "galaxy/fs/FileSystem.hpp"

#include "Application.hpp"

///
/// Core namespace.
///
namespace galaxy
{
	Application::Application(std::unique_ptr<galaxy::Config>& config)
	    : m_visible_tools {true}
	{
		// Seed pseudo-random algorithms.
		std::srand(static_cast<unsigned int>(std::time(nullptr)));

		// Set up all of the difference services.
		// The services are configured based off of the config file.

		// Config reader.
		if (!config)
		{
			PL_LOG(PL_FATAL, "Failed to initialize config.");
		}
		else
		{
			m_config           = std::move(config);
			SL_HANDLE.m_config = m_config.get();
		}

		// FS paths.
		galaxy::FileSystem::s_root     = m_config->get<std::string>("root-path");
		galaxy::FileSystem::s_textures = m_config->get<std::string>("textures-path");
		galaxy::FileSystem::s_shaders  = m_config->get<std::string>("shaders-path");
		galaxy::FileSystem::s_scripts  = m_config->get<std::string>("scripts-path");
		galaxy::FileSystem::s_audio    = m_config->get<std::string>("audio-path");
		galaxy::FileSystem::s_json     = m_config->get<std::string>("json-path");
		galaxy::FileSystem::s_fonts    = m_config->get<std::string>("font-path");
		galaxy::FileSystem::s_saves    = m_config->get<std::string>("save-folder");

		// threadpool
		m_threadpool = std::make_unique<pr::ThreadPool<4>>();
		m_threadpool->start();
		SL_HANDLE.m_threadpool = m_threadpool.get();

		// Window
		// clang-format off
		qs::WindowSettings settings
		{
		    .m_anti_aliasing   = m_config->get<int>("anti-aliasing"),
		    .m_ansio_filtering = m_config->get<int>("ansio-filter"),
		    .m_vsync           = m_config->get<bool>("vsync"),
		    .m_srgb            = m_config->get<bool>("srgb"),
		    .m_aspect_ratio_x  = m_config->get<int>("aspect-ratio-x"),
		    .m_aspect_ratio_y  = m_config->get<int>("aspect-ratio-y"),
		    .m_raw_mouse_input = m_config->get<bool>("raw-mouse-input"),
		    .m_line_thickness  = m_config->get<float>("line-thickness"),
		    .m_gl_debug        = m_config->get<bool>("gl-debug"),
		    .m_title           = m_config->get<std::string>("window-name"),
		    .m_width           = m_config->get<int>("window-width"),
		    .m_height          = m_config->get<int>("window-height")
		};
		// clang-format on

		m_window           = std::make_unique<qs::Window>();
		SL_HANDLE.m_window = m_window.get();
		if (!m_window->create(settings))
		{
			PL_LOG(PL_FATAL, "Failed to create window! Aborting...");
		}
		else
		{
			m_window->request_attention();

			bool cursor = m_config->get<bool>("is-cursor-visible");
			m_window->set_cursor_visibility(cursor);
			if (cursor)
			{
				auto cursor_path = galaxy::FileSystem::s_root + galaxy::FileSystem::s_textures + m_config->get<std::string>("cursor-image");
				m_window->set_cursor_icon(cursor_path);
			}

			auto icon_path = galaxy::FileSystem::s_root + galaxy::FileSystem::s_textures + m_config->get<std::string>("icon-file");
			m_window->set_icon(icon_path);

			// renderer
			m_renderer           = std::make_unique<qs::Renderer>();
			SL_HANDLE.m_renderer = m_renderer.get();

			// Freetype.
			FTLIB.open();

			// Create lua instance and open libraries.
			m_lua = std::make_unique<sol::state>();
			m_lua->open_libraries(sol::lib::base, sol::lib::package, sol::lib::string, sol::lib::os, sol::lib::math, sol::lib::table, sol::lib::io);
			SL_HANDLE.m_lua = m_lua.get();

			m_state           = std::make_unique<pr::StateMachine>();
			SL_HANDLE.m_state = m_state.get();

			// Event dispatcher.
			m_dispatcher           = std::make_unique<sl::Dispatcher>();
			SL_HANDLE.m_dispatcher = m_dispatcher.get();

			// Game "world".
			m_world           = std::make_unique<galaxy::World>(glm::vec2 {m_config->get<float>("gravity-x"), m_config->get<float>("gravity-y")});
			SL_HANDLE.m_world = m_world.get();

			// Serializer.
			m_serializer           = std::make_unique<galaxy::Serializer>();
			SL_HANDLE.m_serializer = m_serializer.get();

			// ShaderBook
			m_shaderbook           = std::make_unique<galaxy::ShaderBook>(m_config->get<std::string>("shaderbook-json"));
			SL_HANDLE.m_shaderbook = m_shaderbook.get();

			// FontBook
			m_fontbook           = std::make_unique<galaxy::FontBook>(m_config->get<std::string>("fontbook-json"));
			SL_HANDLE.m_fontbook = m_fontbook.get();

			// Texture Atlas.
			m_texture_atlas           = std::make_unique<qs::TextureAtlas>();
			SL_HANDLE.m_texture_atlas = m_texture_atlas.get();

			// Register all usertypes used by this application for sol3.
			lua::register_types();
			lua::register_gui();

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

			if (m_window->key_pressed(pr::Keys::GRAVE))
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
		PL_LOG_FINISH;

		FTLIB.close();

		m_threadpool->end();
		m_window->destroy();

		return SL_HANDLE.m_restart;
	}
} // namespace galaxy