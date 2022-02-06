///
/// Application.cpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#include "galaxy/core/Config.hpp"
#include "galaxy/core/ServiceLocator.hpp"
#include "galaxy/error/ConsoleSink.hpp"
#include "galaxy/error/FileSink.hpp"
#include "galaxy/error/Log.hpp"
#include "galaxy/fs/VirtualFileSystem.hpp"
#include "galaxy/platform/Platform.hpp"

#include "Application.hpp"

namespace galaxy
{
	namespace core
	{
		Application::Application(std::string_view log_dir, std::string_view config_file)
		{
			platform::configure_terminal();

			// Seed pseudo-random algorithms.
			std::srand(std::time(nullptr));

			// Set up all of the difference services.
			// The services are configured based off of the config file.
			// Services are created in dependency order.

			const auto        now      = std::chrono::zoned_time {std::chrono::current_zone(), std::chrono::system_clock::now()}.get_local_time();
			const std::string log_path = std::format("{0}{1}{2}", log_dir, std::format("{0:%d-%m-%Y-[%H-%M]}", now), ".log");
			if (!std::filesystem::exists(log_dir))
			{
				std::filesystem::create_directory(log_dir);
			}

			GALAXY_LOG_START();
			GALAXY_ADD_SINK(error::FileSink, log_path);
			GALAXY_ADD_SINK(error::ConsoleSink);

			auto& config = ServiceLocator<Config>::make(config_file);
			if (config.empty())
			{
				config.set<std::string>("asset-dir", "assets/");

				// config.set<bool>("anti-aliasing", false);
				// config.set<bool>("sharpen", false);
				// config.set<int>("ansio-filter", 1);
				// config.set<bool>("vsync", true);
				// config.set<int>("aspect-ratio-x", -1);
				// config.set<int>("aspect-ratio-y", -1);
				// config.set<std::string>("window-name", "Title");
				// config.set<int>("window-width", 1280);
				// config.set<int>("window-height", 720);
				// config.set<bool>("is-cursor-visible", true);
				// config.set<bool>("gl-debug", false);
				// config.set<bool>("trilinear-filtering", false);
				// config.set<bool>("maximized", false);
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

			auto root_opt = config.get<std::string>("asset-dir");
			if (root_opt != std::nullopt)
			{
				auto& root = root_opt.value();
				if (root.back() != '/')
				{
					root += '/';
				}

				create_asset_layout(root, "");

				auto& vfs = ServiceLocator<fs::VirtualFileSystem>::make(root);
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
		}

		Application::~Application()
		{
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