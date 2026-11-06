///
/// Application.cpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#define BS_THREAD_POOL_NATIVE_EXTENSIONS

#include <filesystem>
#include <format>
#include <chrono>

#include <BS_thread_pool.hpp>
#include <error.h>
#include <entt/locator/locator.hpp>
#include <mimalloc.h>
#include <nlohmann/json.hpp>
#include <SDL3/SDL.h>
#include <SDL3_TTF/SDL_ttf.h>
#include <SDL3_mixer/SDL_mixer.h>
#include <sol/sol.hpp>

#include "galaxy/core/Config.hpp"
#include "galaxy/core/Window.hpp"
#include "galaxy/core/Settings.hpp"
#include "galaxy/factory/SystemFactory.hpp"
#include "galaxy/fs/VirtualFileSystem.hpp"
#include "galaxy/input/Keyboard.hpp"
#include "galaxy/input/Mouse.hpp"
#include "galaxy/logging/ConsoleSink.hpp"
#include "galaxy/logging/FileSink.hpp"
#include "galaxy/logging/Log.hpp"
#include "galaxy/lua/Lua.hpp"
#include "galaxy/math/Base64.hpp"
#include "galaxy/math/ZLib.hpp"
#include "galaxy/platform/Platform.hpp"
#include "galaxy/scene/SceneManager.hpp"
#include "galaxy/time/Time.hpp"
#include "galaxy/ui/ImGuiHelpers.hpp"

#include "Application.hpp"

using namespace std::chrono_literals;

namespace galaxy
{
	App::App(const std::string& config_file)
	{
		SDL_SetMemoryFunctions(&mi_malloc, &mi_calloc, &mi_realloc, &mi_free);

		setup_async();
		setup_config(config_file);
		setup_logging();
		setup_platform();
		setup_fs();
		setup_rendering();
		setup_input();
		setup_factory();
		// setup_loader();
		// setup_meta();
		setup_services();
		setup_ui();
		setup_scripting();

		// Load game assets.
		// core::entt::locator<core::Loader>::ref().load_all();
	}

	App::~App()
	{
		entt::locator<BS::priority_thread_pool>::value().wait();

		entt::locator<sol::state>::reset();
		ui::imgui_exit();
		entt::locator<SceneManager>::reset();
		entt::locator<SystemFactory>::reset();
		entt::locator<Mouse>::reset();
		entt::locator<Keyboard>::reset();
		entt::locator<Window>::reset();
		entt::locator<VirtualFileSystem>::reset();
		entt::locator<Log>::reset();
		entt::locator<Config>::reset();
		entt::locator<BS::priority_thread_pool>::reset();

		MIX_Quit();
		TTF_Quit();
		SDL_Quit();
	}

	void App::load(const std::string& app_data)
	{
		// This just loads the scenes, the actual asset data is already loaded (and specificed) by config.
		// and done in the VFS.

		auto&      fs   = entt::locator<VirtualFileSystem>::value();
		const auto data = fs.read(Settings::base_data());

		if (!data.empty())
		{
			const auto decoded_zlib = ZLib::decode(data);
			if (!decoded_zlib.empty())
			{
				const auto decoded_base64 = math::decode_base64(decoded_zlib);
				if (!decoded_base64.empty())
				{
					const auto parsed = nlohmann::json::parse(decoded_base64);

					if (!parsed.empty())
					{
						auto& sm = entt::locator<SceneManager>::value();
						sm.deserialize(parsed);
					}
					else
					{
						GALAXY_LOG(GALAXY_ERROR, "Failed to parse scenemanger JSON data from memory.");
					}
				}
				else
				{
					GALAXY_LOG(GALAXY_ERROR, "Failed to decode base64 app_data '{0}'.", app_data);
				}
			}
			else
			{
				GALAXY_LOG(GALAXY_ERROR, "Failed to decode zlib app_data '{0}'.", app_data);
			}
		}
		else
		{
			GALAXY_LOG(GALAXY_ERROR, "Failed to load app_data '{0}'.", app_data);
		}
	}

	void App::save(const std::string& app_data)
	{
		auto& sm = entt::locator<SceneManager>::value();

		const auto json = sm.serialize();
		auto       data = json.dump(4);

		if constexpr (!GALAXY_DEBUG_BUILD)
		{
			data = math::encode_base64(data);
			if (!data.empty())
			{
				data = ZLib::encode(data);
				if (data.empty())
				{
					GALAXY_LOG(GALAXY_ERROR, "Failed to encode scenemanager to zlib '{0}'.", app_data);
				}
			}
			else
			{
				GALAXY_LOG(GALAXY_ERROR, "Failed to encode scenemanager to base64 '{0}'.", app_data);
			}

			if (data.empty())
			{
				return;
			}
		}

		auto& fs = entt::locator<VirtualFileSystem>::value();
		if (!fs.write(data, app_data))
		{
			GALAXY_LOG(GALAXY_ERROR, "Failed to save '{0}' to disk.", app_data);
		}
	}

	void App::run()
	{
		// https://stackoverflow.com/a/59446610
		// We dont need 't' or 'alpha/render' sections.

		auto& window = entt::locator<Window>::value();
		auto& scenes = entt::locator<SceneManager>::value();

		// The expression dt/1s simply converts the double-based chrono seconds
		// into a double so it can participate in the physics computation.
		constexpr const auto dt = std::chrono::duration<long long, std::ratio<1, 60>> {1};
		time::dt(dt / 1.0s);

		using clock      = std::chrono::steady_clock;
		using duration   = decltype(clock::duration {} + dt);
		using time_point = std::chrono::time_point<clock, duration>;

		duration   accum = 0s;
		time_point prev  = clock::now();
		time_point now   = clock::now();

		auto     updates = 0u;
		auto     frames  = 0u;
		duration perf    = 0s;

		while (window.is_open())
		{
			now          = clock::now();
			auto elapsed = now - prev;

			// 250ms is the limit put in place on the frame time to cope with the spiral of death.
			// It doesn't have to be 250ms exactly but it should be sufficiently high enough to deal with spikes in load.
			if (elapsed > 250ms)
			{
				elapsed = 250ms;
			}

			prev   = now;
			accum += elapsed;

			while (accum >= dt)
			{
				perf  += dt;
				accum -= dt;

				while (SDL_PollEvent(&m_events))
				{
					switch (m_events.type)
					{
						case SDL_EVENT_LOCALE_CHANGED:
							// TODO change language
							break;

						case SDL_EVENT_WINDOW_SHOWN:
							// TODO enable input/audio
							break;

						case SDL_EVENT_WINDOW_HIDDEN:
						case SDL_EVENT_WINDOW_MINIMIZED:
							// TODO disable input/audio
							break;

							/*case SDL_EVENT_NOTIFICATION_ACTION_INVOKED:
								/// A user response to a system notification was received.
								break;*/

							/*case SDL_EVENT_CLIPBOARD_UPDATE:
								/// Just use clipboard methods.
								break;*/

						default:
							ui::imgui_handle_events(m_events);
							window.handle_window_events(m_events);
							scenes.on_event(m_events);
							break;
					}
				}

				scenes.update();

				updates++;
			}

			scenes.render();

			ui::imgui_new_frame();
			ui::imgui_draw_editor();
			ui::imgui_render();

			window.swap();

			frames++;

			if (perf >= 1s)
			{
				window.append_title(std::format(" | UPS: {0}, FPS: {1}", updates, frames));

				frames  = 0;
				updates = 0;
				perf    = 0s;
			}
		}
	}

	SDL_Event& App::events() noexcept
	{
		return m_events;
	}

	void App::setup_async()
	{
		// Configure threadpool.

		// Calc threads.
		// We optimize for 6: 1 for audio, 1 for main, 4 for tasks.
		auto system_cores = std::thread::hardware_concurrency();
		if (system_cores < 6)
		{
			system_cores = std::thread::hardware_concurrency();
		}

		// Check for highest available priority.
		BS::set_os_process_priority(BS::os_process_priority::high);
		entt::locator<BS::priority_thread_pool>::emplace(system_cores, [](const std::size_t idx) {
			BS::this_thread::set_os_thread_priority(BS::os_thread_priority::highest);
		});
	}

	void App::setup_config(std::string_view config_file)
	{
		auto& config = entt::locator<Config>::emplace(config_file);
		Settings::set_config_to_default();
		Settings::set_settings_from_config();
	}

	void App::setup_logging()
	{
		platform::configure_terminal();
		if (!std::filesystem::exists(Settings::logs_dir()))
		{
			std::filesystem::create_directory(Settings::logs_dir());
		}
		entt::locator<Log>::emplace();

		const auto path = std::format("{0}{1}{2}", Settings::logs_dir(), std::format("{0:%d-%m-%Y-[%H-%M]}", time::now()), ".log");
		GALAXY_ADD_SINK(FileSink, path);

		GALAXY_ADD_SINK(ConsoleSink);
		GALAXY_LOG(GALAXY_INFO, "App started.");

		sl::set_log_callback([](const std::string& msg) {
			GALAXY_LOG(GALAXY_ERROR, "simlib | {0}", msg);
		});
	}

	void App::setup_platform()
	{
		platform::seed_random();

		platform::set_metadata(SDL_PROP_APP_METADATA_NAME_STRING, Settings::title().c_str());
		platform::set_metadata(SDL_PROP_APP_METADATA_VERSION_STRING, Settings::version().c_str());
		platform::set_metadata(SDL_PROP_APP_METADATA_IDENTIFIER_STRING, Settings::identifier().c_str());
		platform::set_metadata(SDL_PROP_APP_METADATA_CREATOR_STRING, Settings::creator().c_str());
		platform::set_metadata(SDL_PROP_APP_METADATA_COPYRIGHT_STRING, Settings::copyright().c_str());
		platform::set_metadata(SDL_PROP_APP_METADATA_URL_STRING, Settings::website().c_str());
		platform::set_metadata(SDL_PROP_APP_METADATA_TYPE_STRING, "game");

		// https://wiki.libsdl.org/SDL3/CategoryHints
		platform::set_hint(SDL_HINT_ALLOW_ALT_TAB_WHILE_GRABBED, "1");
		platform::set_hint(SDL_HINT_ASSERT, "break");
		platform::set_hint(SDL_HINT_AUDIO_CATEGORY, "playback");
		platform::set_hint(SDL_HINT_AUDIO_CHANNELS, "2");
		platform::set_hint(SDL_HINT_AUDIO_INCLUDE_MONITORS, "0");
		platform::set_hint(SDL_HINT_AUTO_UPDATE_JOYSTICKS, "1");
		platform::set_hint(SDL_HINT_AUTO_UPDATE_SENSORS, "1");
		platform::set_hint(SDL_HINT_LOGGING, "warn");
		platform::set_hint(SDL_HINT_ENABLE_SCREEN_KEYBOARD, "0");
		platform::set_hint(SDL_HINT_EVENT_LOGGING, "0");
		platform::set_hint(SDL_HINT_FRAMEBUFFER_ACCELERATION, "opengl");
		platform::set_hint(SDL_HINT_GAMECONTROLLER_SENSOR_FUSION, "1");
		platform::set_hint(SDL_HINT_GPU_DRIVER, "vulkan");
		platform::set_hint(SDL_HINT_HIDAPI_ENUMERATE_ONLY_CONTROLLERS, "1");
		platform::set_hint(SDL_HINT_JOYSTICK_ALLOW_BACKGROUND_EVENTS, "0");
		platform::set_hint(SDL_HINT_JOYSTICK_DIRECTINPUT, "1");
		platform::set_hint(SDL_HINT_TV_REMOTE_AS_JOYSTICK, "1");
		platform::set_hint(SDL_HINT_MOUSE_AUTO_CAPTURE, "1");
		platform::set_hint(SDL_HINT_MOUSE_FOCUS_CLICKTHROUGH, "1");
		platform::set_hint(SDL_HINT_MOUSE_RELATIVE_MODE_CENTER, "0");
		platform::set_hint(SDL_HINT_MOUSE_TOUCH_EVENTS, "1");
		platform::set_hint(SDL_HINT_MOUSE_DPI_SCALE_CURSORS, "1");
		platform::set_hint(SDL_HINT_PEN_MOUSE_EVENTS, "1");
		platform::set_hint(SDL_HINT_PEN_TOUCH_EVENTS, "1");
		platform::set_hint(SDL_HINT_QUIT_ON_LAST_WINDOW_CLOSE, "1");
		platform::set_hint(SDL_HINT_RENDER_DRIVER, "opengl");
		platform::set_hint(SDL_HINT_RENDER_GPU_DEBUG, "0");
		platform::set_hint(SDL_HINT_RENDER_GPU_LOW_POWER, "0");
		platform::set_hint(SDL_HINT_TOUCH_MOUSE_EVENTS, "1");
		platform::set_hint(SDL_HINT_TRACKPAD_IS_TOUCH_ONLY, "0");
		platform::set_hint(SDL_HINT_THREAD_FORCE_REALTIME_TIME_CRITICAL, "0");
		platform::set_hint(SDL_HINT_XINPUT_ENABLED, "1");
		platform::set_hint(SDL_HINT_VIDEO_ALLOW_SCREENSAVER, "0");
		platform::set_hint(SDL_HINT_VIDEO_DUMMY_SAVE_FRAMES, "0");
		platform::set_hint(SDL_HINT_VIDEO_MATCH_EXCLUSIVE_MODE_ON_MOVE, "1");
		platform::set_hint(SDL_HINT_VIDEO_MINIMIZE_ON_FOCUS_LOSS, "0");
		platform::set_hint(SDL_HINT_VIDEO_OFFSCREEN_SAVE_FRAMES, "0");
		platform::set_hint(SDL_HINT_VIDEO_SYNC_WINDOW_OPERATIONS, "0");
		platform::set_hint(SDL_HINT_WINDOW_FRAME_USABLE_WHILE_CURSOR_HIDDEN, "1");
		platform::set_hint(SDL_HINT_WINDOWS_CLOSE_ON_ALT_F4, "1");
		platform::set_hint(SDL_HINT_WINDOWS_ENABLE_MENU_MNEMONICS, "0");
		platform::set_hint(SDL_HINT_WINDOWS_GAMEINPUT, "1");
		platform::set_hint(SDL_HINT_WINDOWS_RAW_KEYBOARD, "0");
		platform::set_hint(SDL_HINT_IME_IMPLEMENTED_UI, "0");

		const auto vsync = Settings::vsync() ? "1" : "0";
		platform::set_hint(SDL_HINT_RENDER_VSYNC, vsync);

		const auto audio_freq = std::to_string(Settings::audio_freq());
		platform::set_hint(SDL_HINT_AUDIO_FREQUENCY, audio_freq.c_str());

		if (!SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO | SDL_INIT_EVENTS | SDL_INIT_GAMEPAD | SDL_INIT_JOYSTICK))
		{
			GALAXY_LOG(GALAXY_FATAL, SDL_GetError());
		}

		if (!TTF_Init())
		{
			GALAXY_LOG(GALAXY_FATAL, SDL_GetError());
		}

		if (!MIX_Init())
		{
			GALAXY_LOG(GALAXY_FATAL, SDL_GetError());
		}
	}

	void App::setup_fs()
	{
		entt::locator<VirtualFileSystem>::emplace();
	}

	void App::setup_rendering()
	{
		auto& window = entt::locator<Window>::emplace();
		window.set_icon(Settings::icon());
		window.show();

		// auto& sampler = entt::locator<Sampler>::emplace();

		//// Need to create our default texture sampler object.
		// if (Settings::mipmap())
		//{
		//	switch (Settings::texture_filter())
		//	{
		//		case GLTextureFilter::NEAREST:
		//			sampler.set(GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_NEAREST);
		//			sampler.set(GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		//			break;
		//		case GLTextureFilter::BILINEAR:
		//			sampler.set(GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST);
		//			sampler.set(GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		//			break;
		//		case GLTextureFilter::TRILINEAR:
		//			sampler.set(GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		//			sampler.set(GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		//			break;
		//	}
		// }
		// else
		//{
		//	if (Settings::texture_filter() == GLTextureFilter::NEAREST)
		//	{
		//		sampler.set(GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		//		sampler.set(GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		//	}
		//	else
		//	{
		//		sampler.set(GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		//		sampler.set(GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		//	}
		// }

		// auto ansio = std::clamp(Settings::ansiotrophy(), 1, 16);
		// if (ansio == 3)
		//{
		//	ansio = 4;
		// }
		// else if (ansio > 4 && ansio < 8)
		//{
		//	ansio = 8;
		// }
		// else
		//{
		//	ansio = 16;
		// }
		// sampler.setf(GL_TEXTURE_MAX_ANISOTROPY, static_cast<float>(ansio));

		// sampler.set(GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		// sampler.set(GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		// sampler.set(GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
		// sampler.set(GL_TEXTURE_LOD_BIAS, GL_NONE);
	}

	void App::setup_input()
	{
		SDL_zero(m_events);

		auto& keyboard = entt::locator<Keyboard>::emplace();
		auto& mouse    = entt::locator<Mouse>::emplace();

		mouse.use_custom_mouse(Settings::cursor_icon(), Settings::cursor_offset());
		if (Settings::cursor_show())
		{
			mouse.show_cursor();
		}
		else
		{
			mouse.hide_cursor();
		}
	}

	void App::setup_factory()
	{
		entt::locator<SystemFactory>::emplace();
	}

	/*
	// void App::setup_loader()
	//{
	//	// entt::locator<Loader>::make();
	// }

	void App::setup_meta()
	{

		// auto& ef = entt::locator<meta::EntityFactory>::emplace();

		/*ef.register_component<components::Tag>("Tag");

		em.register_component<components::Animated>("Animated");
		em.register_component<components::Circle>("Circle");
		em.register_component<components::Ellipse>("Ellipse");
		em.register_component<components::Point>("Point");
		em.register_component<components::Polygon>("Polygon");
		em.register_component<components::Polyline>("Polyline");
		em.register_component<components::RigidBody>("RigidBody");
		em.register_component<components::Script>("Script");
		em.register_component<components::Sprite>("Sprite");
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
	}*/

	void App::setup_services()
	{
		// entt::locator<media::SoundEngine>::make(listener_count);
		// entt::locator<media::MusicEngine>::make(listener_count);
		// entt::locator<media::VoiceEngine>::make(listener_count);
		// entt::locator<resource::SoundCache>::make();
		// entt::locator<resource::MusicCache>::make();
		// entt::locator<resource::VoiceCache>::make();
		// entt::locator<resource::VideoCache>::make();
		// entt::locator<resource::Animations>::make();
		// entt::locator<resource::Shaders>::make();
		// entt::locator<resource::Fonts>::make();
		// entt::locator<resource::Textures>::make();
		// entt::locator<resource::Prefabs>::make();
		//	  entt::locator<resource::Scripts>::make();
		entt::locator<SceneManager>::emplace();
	}

	void App::setup_ui()
	{
		// ServiceLocator<ui::NuklearUI>::make();

		ui::imgui_init("editor.ini");
	}

	void App::setup_scripting()
	{
		auto& lua = entt::locator<sol::state>::emplace();
		lua.open_libraries(sol::lib::base, sol::lib::package, sol::lib::coroutine, sol::lib::string, sol::lib::os, sol::lib::math, sol::lib::table, sol::lib::io, sol::lib::utf8);

		//
		// Add external libraries to Lua.
		// Inject all configured galaxy into Lua.
		// Add engine services to lua.
		//
		Lua::inject();
	}
} // namespace galaxy
