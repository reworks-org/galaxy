///
/// Loading.cpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#include <BS_thread_pool.hpp>
#include <zip.h>

#include "galaxy/core/Config.hpp"
#include "galaxy/core/ServiceLocator.hpp"
#include "galaxy/core/Window.hpp"
#include "galaxy/fs/VirtualFileSystem.hpp"
#include "galaxy/graphics/Renderer.hpp"
#include "galaxy/input/Input.hpp"
#include "galaxy/media/AudioEngine.hpp"
#include "galaxy/resource/BasicScripts.hpp"
#include "galaxy/resource/Fonts.hpp"
#include "galaxy/resource/Language.hpp"
#include "galaxy/resource/Prefabs.hpp"
#include "galaxy/resource/Shaders.hpp"
#include "galaxy/resource/TextureAtlas.hpp"
#include "galaxy/ui/NuklearUI.hpp"

#include "Loader.hpp"

namespace galaxy
{
	namespace core
	{
		Loader::Loader()
		{
		}

		Loader::~Loader()
		{
		}

		void Loader::extract_assets()
		{
			auto& config   = ServiceLocator<Config>::ref();
			auto& nui      = ServiceLocator<ui::NuklearUI>::ref();
			auto& renderer = ServiceLocator<graphics::Renderer>::ref();

			const auto asset_dir         = config.get<std::string>("asset_dir");
			const auto compressed_assets = config.get<std::string>("compressed_assets");

			// Unpack assets if zip is present.
			if (std::filesystem::exists(compressed_assets))
			{
				GALAXY_LOG(GALAXY_INFO, "Detected asset pack. Extracting...");
				GALAXY_LOG(GALAXY_INFO, "Removing old assets...");
				std::filesystem::remove_all(asset_dir);

				// Get file count.
				auto z    = zip_open(compressed_assets.c_str(), 0, 'r');
				int count = static_cast<int>(zip_entries_total(z));
				zip_close(z);

				GALAXY_LOG(GALAXY_INFO, "Beginning asset extraction...");

				renderer.prepare_default();
				renderer.clear();

				auto extractor = [](const char* filename, void* arg) {
					glfwPollEvents();

					auto& nui      = ServiceLocator<ui::NuklearUI>::ref();
					auto& renderer = ServiceLocator<graphics::Renderer>::ref();

					static int i = 0;
					const int n  = *static_cast<int*>(arg);

					++i;
					nui.new_frame();
					nui.show_loading_bar("Extracting Assets...", i, n);

					renderer.clear();
					nui.render();
					renderer.swap_buffers();

					return 0;
				};

				if (zip_extract(compressed_assets.c_str(), "./", extractor, &count) == 0)
				{
					std::filesystem::remove(compressed_assets);
					GALAXY_LOG(GALAXY_INFO, "Successfully extracted assets.");
				}
				else
				{
					std::filesystem::remove_all(asset_dir);
					GALAXY_LOG(GALAXY_FATAL, "Failed to extract assets.");
				}
			}

			// Clear popup.
			renderer.clear();
			renderer.swap_buffers();
		}

		void Loader::load_all()
		{
			auto& tp = ServiceLocator<BS::thread_pool>::ref();

			tp.push_task(&Loader::load_user_config, this);
			tp.push_task(&Loader::load_window, this);

			load_resources();
		}

		void Loader::load_resources()
		{
			auto& tp       = ServiceLocator<BS::thread_pool>::ref();
			auto& config   = ServiceLocator<Config>::ref();
			auto& nui      = ServiceLocator<ui::NuklearUI>::ref();
			auto& renderer = ServiceLocator<graphics::Renderer>::ref();

			const auto& audio_folder = config.get<std::string>("audio_folder", "resource_folders");

			tp.push_task(
				[](const std::string& folder) {
					auto& sounds = ServiceLocator<resource::SFXCache>::ref();
					sounds.load_folder(folder);
				},
				audio_folder + "sfx");

			tp.push_task(
				[](const std::string& folder) {
					auto& music = ServiceLocator<resource::MusicCache>::ref();
					music.load_folder(folder);
				},
				audio_folder + "music");

			tp.push_task(
				[](const std::string& folder) {
					auto& dialogue = ServiceLocator<resource::DialogueCache>::ref();
					dialogue.load_folder(folder);
				},
				audio_folder + "dialogue");

			tp.push_task([]() {
				auto& config = ServiceLocator<Config>::ref();
				auto& video  = ServiceLocator<resource::VideoCache>::ref();

				video.load_folder(config.get<std::string>("video_folder", "resource_folders"));
			});

			tp.push_task([]() {
				auto& config  = ServiceLocator<Config>::ref();
				auto& shaders = ServiceLocator<resource::Shaders>::ref();

				shaders.load_folder(config.get<std::string>("shader_folder", "resource_folders"));
			});

			tp.push_task([]() {
				auto& config = ServiceLocator<Config>::ref();
				auto& fonts  = ServiceLocator<resource::Fonts>::ref();

				fonts.load_folder(config.get<std::string>("font_folder", "resource_folders"));
			});

			tp.push_task([]() {
				auto& config       = ServiceLocator<Config>::ref();
				auto& basicscripts = ServiceLocator<resource::BasicScripts>::ref();

				basicscripts.load_folder(config.get<std::string>("scripts_folder", "resource_folders"));
			});

			tp.push_task([]() {
				auto& config  = ServiceLocator<Config>::ref();
				auto& prefabs = ServiceLocator<resource::Prefabs>::ref();

				prefabs.load_folder(config.get<std::string>("prefabs_folder", "resource_folders"));
			});

			tp.push_task([]() {
				auto& config = ServiceLocator<Config>::ref();
				auto& lang   = ServiceLocator<resource::Language>::ref();

				lang.load_folder(config.get<std::string>("lang_folder", "resource_folders"));
				lang.set(config.get<std::string>("default_lang"));
			});

			nui.enable_input();
			renderer.prepare_default();

			for (std::size_t tasks; (tasks = tp.get_tasks_total()) != 0;)
			// for (int tasks = 0; tasks < 10; tasks++)
			{
				glfwPollEvents();
				nui.new_frame();
				nui.show_loading_bar("Loading Assets...", 10, tasks);

				renderer.clear();
				nui.render();
				renderer.swap_buffers();
				// std::this_thread::sleep_for(.1s);
			}

			glfwPollEvents();
			nui.new_frame();
			nui.show_building_atlas();
			renderer.clear();
			nui.render();
			renderer.swap_buffers();

			build_resources();
			nui.disable_input();
		}

		void Loader::load_user_config()
		{
			auto& config = core::ServiceLocator<core::Config>::ref();
			auto& window = core::ServiceLocator<core::Window>::ref();

			input::CameraKeys::FORWARD      = input::int_to_key(config.get<int>("camera_foward", "input"));
			input::CameraKeys::BACKWARD     = input::int_to_key(config.get<int>("camera_backward", "input"));
			input::CameraKeys::LEFT         = input::int_to_key(config.get<int>("camera_left", "input"));
			input::CameraKeys::RIGHT        = input::int_to_key(config.get<int>("camera_right", "input"));
			input::CameraKeys::ROTATE_LEFT  = input::int_to_key(config.get<int>("camera_rotate_left", "input"));
			input::CameraKeys::ROTATE_RIGHT = input::int_to_key(config.get<int>("camera_rotate_right", "input"));

			auto& se = core::ServiceLocator<media::SoundEngine>::ref();
			auto& me = core::ServiceLocator<media::MusicEngine>::ref();
			auto& de = core::ServiceLocator<media::DialogueEngine>::ref();

			se.set_volume(config.get<float>("sfx_volume", "audio"));
			me.set_volume(config.get<float>("music_volume", "audio"));
			de.set_volume(config.get<float>("dialogue_volume", "audio"));
		}

		void Loader::load_window()
		{
			auto& config = core::ServiceLocator<core::Config>::ref();
			auto& window = core::ServiceLocator<core::Window>::ref();

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
				cursor.load_custom(cursor_icon);
				cursor.use_custom_else_pointer();
			}
			else
			{
				cursor.use_pointer();
			}
		}

		void Loader::build_resources()
		{
			core::ServiceLocator<resource::Shaders>::ref().build();
			core::ServiceLocator<resource::Fonts>::ref().build();

			core::ServiceLocator<resource::TextureAtlas>::ref().add_folder(
				core::ServiceLocator<core::Config>::ref().get<std::string>("atlas_folder", "resource_folders"));
		}
	} // namespace core
} // namespace galaxy