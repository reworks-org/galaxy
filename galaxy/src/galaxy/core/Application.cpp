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
#include "galaxy/fs/Serializer.hpp"
#include "galaxy/fs/VirtualFileSystem.hpp"
#include "galaxy/graphics/FreeType.hpp"
#include "galaxy/platform/Platform.hpp"
#include "galaxy/scripting/Lua.hpp"

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
			// SERIALIZER.
			//
			ServiceLocator<fs::Serializer>::make();

			//
			// CONFIG.
			//
			auto& config = ServiceLocator<Config>::make(config_file);
			if (config.empty())
			{
				config.set<std::string>("asset_dir", "assets/");
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

				// config.set<bool>("anti-aliasing", false);
				// config.set<bool>("sharpen", false);
				// config.set<int>("ansio-filter", 1);
				// config.set<bool>("trilinear-filtering", false);
				// config.set<bool>("log-perf", false);
				// config.set<float>("audio-volume", 0.7f);
				// config.set<std::string>("logo", "logo.png");
				// config.set<std::string>("cursor-image", "cursor.png");
				// config.set<std::string>("icon-file", "icon.png");
				// config.set<std::string>("fontbook-json", "fontbook.json");
				// config.set<std::string>("shaderbook-json", "shaderbook.json");
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

				ServiceLocator<fs::VirtualFileSystem>::make(root);
				create_asset_layout(root, "audio/music/");
				create_asset_layout(root, "audio/sfx/");
				create_asset_layout(root, "fonts/");
				create_asset_layout(root, "json/");
				create_asset_layout(root, "scripts/actions/");
				create_asset_layout(root, "scripts/definitions/");
				create_asset_layout(root, "shaders/");
				create_asset_layout(root, "textures/");
				create_asset_layout(root, "maps/");
				create_asset_layout(root, "lang/");

				generate_default_assets(root);
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
			ServiceLocator<fs::VirtualFileSystem>::del();
			ServiceLocator<Config>::del();
			ServiceLocator<fs::Serializer>::del();

			GALAXY_LOG_FINISH();
		}

		const bool Application::run()
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

		void Application::generate_default_assets(const std::string& root)
		{
			constexpr const auto font          = "{\"fonts\": {}}";
			constexpr const auto music         = "{\"music\": {}}";
			constexpr const auto script        = "{\"scripts\": {},\"definitions\":[]}";
			constexpr const auto shader        = "{\"vertex-extension\": \".vs\",\"fragment-extension\": \".fs\",\"shaders\": []}";
			constexpr const auto sound         = "{\"sounds\": {}}";
			constexpr const auto atlas         = "{\"textures\": []}";
			constexpr const auto render_layers = "{\"layers\":{\"bottom\": 0, \"top\": 1}}";

			auto& vfs = ServiceLocator<fs::VirtualFileSystem>::ref();

			const auto fb_path = root + "json/fonts.json";
			if (!std::filesystem::exists(fb_path))
			{
				if (!vfs.save(font, fb_path))
				{
					GALAXY_LOG(GALAXY_FATAL, "Failed to create default font assets.");
				}

				GALAXY_LOG(GALAXY_INFO, "Missing default asset, creating: {0}.", fb_path);
			}

			const auto mb_path = root + "json/music.json";
			if (!std::filesystem::exists(mb_path))
			{
				if (!vfs.save(music, mb_path))
				{
					GALAXY_LOG(GALAXY_FATAL, "Failed to create default music assets.");
				}

				GALAXY_LOG(GALAXY_INFO, "Missing default asset, creating: {0}.", mb_path);
			}

			const auto sb_path = root + "json/scripts.json";
			if (!std::filesystem::exists(sb_path))
			{
				if (!vfs.save(script, sb_path))
				{
					GALAXY_LOG(GALAXY_FATAL, "Failed to create default script assets.");
				}

				GALAXY_LOG(GALAXY_INFO, "Missing default asset, creating: {0}.", sb_path);
			}

			const auto shb_path = root + "json/shaders.json";
			if (!std::filesystem::exists(shb_path))
			{
				if (!vfs.save(shader, shb_path))
				{
					GALAXY_LOG(GALAXY_FATAL, "Failed to create default shader assets.");
				}

				GALAXY_LOG(GALAXY_INFO, "Missing default asset, creating: {0}.", shb_path);
			}

			const auto sfxb_path = root + "json/sounds.json";
			if (!std::filesystem::exists(sfxb_path))
			{
				if (!vfs.save(sound, sfxb_path))
				{
					GALAXY_LOG(GALAXY_FATAL, "Failed to create default sound assets.");
				}

				GALAXY_LOG(GALAXY_INFO, "Missing default asset, creating: {0}.", sfxb_path);
			}

			const auto tb_path = root + "json/textures.json";
			if (!std::filesystem::exists(tb_path))
			{
				if (!vfs.save(atlas, tb_path))
				{
					GALAXY_LOG(GALAXY_FATAL, "Failed to create default texture assets.");
				}

				GALAXY_LOG(GALAXY_INFO, "Missing default asset, creating: {0}.", tb_path);
			}

			const auto rl_path = root + "json/render-layers.json";
			if (!std::filesystem::exists(rl_path))
			{
				if (!vfs.save(render_layers, rl_path))
				{
					GALAXY_LOG(GALAXY_FATAL, "Failed to create default rendering layers.");
				}

				GALAXY_LOG(GALAXY_INFO, "Missing default asset, creating: {0}.", rl_path);
			}
		}
	} // namespace core
} // namespace galaxy