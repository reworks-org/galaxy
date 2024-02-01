///
/// Loader.cpp
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
#include "galaxy/graphics/DefaultShaders.hpp"
#include "galaxy/graphics/Renderer.hpp"
#include "galaxy/input/Input.hpp"
#include "galaxy/media/AudioEngine.hpp"
#include "galaxy/resource/Fonts.hpp"
#include "galaxy/resource/Maps.hpp"
#include "galaxy/resource/Prefabs.hpp"
#include "galaxy/resource/Scripts.hpp"
#include "galaxy/resource/Shaders.hpp"
#include "galaxy/resource/TextureAtlas.hpp"
#include "galaxy/ui/NuklearUI.hpp"

#include "Loader.hpp"

using namespace std::chrono_literals;

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

		void Loader::load_all()
		{
			auto& tp       = ServiceLocator<BS::thread_pool>::ref();
			auto& config   = ServiceLocator<Config>::ref();
			auto& nui      = ServiceLocator<ui::NuklearUI>::ref();
			auto& renderer = ServiceLocator<graphics::Renderer>::ref();

			tp.push_task(&Loader::load_user_config, this);
			tp.push_task(&Loader::load_window, this);
			tp.push_task(&Loader::load_resources, this);

			nui.enable_input();
			renderer.prepare_default();

			nk_size                 current = 0;
			constexpr const nk_size total   = 3;

			do
			{
				if (tp.get_tasks_total() == 0)
				{
					current = total;
				}

				glfwPollEvents();
				nui.new_frame();
				nui.show_loading_bar("Loading Assets...", total, current);

				renderer.clear();
				nui.render();
				renderer.swap_buffers();

				current++;
				// std::this_thread::sleep_for(.1s);
			} while (tp.get_tasks_total() > 0);

			glfwPollEvents();
			nui.new_frame();
			nui.show_building_atlas();
			renderer.clear();
			nui.render();
			renderer.swap_buffers();

			build_resources();
			nui.disable_input();
		}

		void Loader::load_resources()
		{
			ServiceLocator<resource::SoundCache>::ref().load_folder(GALAXY_SFX_DIR);
			ServiceLocator<resource::MusicCache>::ref().load_folder(GALAXY_MUSIC_DIR);
			ServiceLocator<resource::VoiceCache>::ref().load_folder(GALAXY_VOICE_DIR);
			ServiceLocator<resource::VideoCache>::ref().load_folder(GALAXY_VIDEO_DIR);
			ServiceLocator<resource::Shaders>::ref().load_folder(GALAXY_SHADER_DIR);
			ServiceLocator<resource::Fonts>::ref().load_folder(GALAXY_FONT_DIR);
			ServiceLocator<resource::Scripts>::ref().load_folder(GALAXY_SCRIPT_DIR);
			ServiceLocator<resource::Prefabs>::ref().load_folder(GALAXY_PREFABS_DIR);
			ServiceLocator<resource::Maps>::ref().load_folder(GALAXY_MAPS_DIR);
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
			auto& de = core::ServiceLocator<media::VoiceEngine>::ref();

			se.set_volume(config.get<float>("sfx_volume", "audio"));
			me.set_volume(config.get<float>("music_volume", "audio"));
			de.set_volume(config.get<float>("dialogue_volume", "audio"));
		}

		void Loader::load_window()
		{
			auto& config = core::ServiceLocator<core::Config>::ref();
			auto& window = core::ServiceLocator<core::Window>::ref();

			auto icon = config.get<std::string>("icon", "window");
			window.set_icon(icon);

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

		void Loader::load_maps(const std::vector<std::string>& maps, entt::registry& registry)
		{
			auto& nui       = ServiceLocator<ui::NuklearUI>::ref();
			auto& renderer  = ServiceLocator<graphics::Renderer>::ref();
			auto& maploader = core::ServiceLocator<resource::Maps>::ref();

			nui.enable_input();
			renderer.prepare_default();

			int cur = 0;
			for (auto& map : maps)
			{
				cur++;

				glfwPollEvents();
				nui.new_frame();
				nui.show_loading_maps(cur, static_cast<int>(maps.size()));
				renderer.clear();
				nui.render();
				renderer.swap_buffers();

				maploader.get(map)->create(registry);
			}

			nui.disable_input();
		}

		void Loader::build_resources()
		{
			auto& shaders = core::ServiceLocator<resource::Shaders>::ref();
			shaders.insert("RenderToTexture", std::make_shared<graphics::Shader>(shaders::render_to_texture_vert, shaders::render_to_texture_frag));

			for (const auto& [key, value] : shaders.cache())
			{
				value->compile();
			}

			for (const auto& [key, value] : core::ServiceLocator<resource::Fonts>::ref().cache())
			{
				value->build();
			}

			for (auto&& [key, value] : core::ServiceLocator<resource::VideoCache>::ref().cache())
			{
				value->build();
			}

			core::ServiceLocator<resource::TextureAtlas>::ref().add_from_vfs();
		}
	} // namespace core
} // namespace galaxy
