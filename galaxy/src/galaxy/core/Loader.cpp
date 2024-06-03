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
#include "galaxy/graphics/Renderer.hpp"
#include "galaxy/input/Input.hpp"
#include "galaxy/media/AudioEngine.hpp"
#include "galaxy/resource/Animations.hpp"
#include "galaxy/resource/embedded/RenderTextureShader.hpp"
#include "galaxy/resource/Fonts.hpp"
#include "galaxy/resource/Prefabs.hpp"
#include "galaxy/resource/Scripts.hpp"
#include "galaxy/resource/Shaders.hpp"
#include "galaxy/resource/Textures.hpp"

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
			auto& tp     = ServiceLocator<BS::thread_pool>::ref();
			auto& config = ServiceLocator<Config>::ref();
			auto& w      = core::ServiceLocator<core::Window>::ref();

			// nk_size                 current = 0;
			// constexpr const nk_size total   = 11;

			tp.detach_task([this]() -> void {
				this->load_user_config();
			});

			tp.detach_task([this]() -> void {
				this->load_window();
			});

			load_resources();

			glBindFramebuffer(GL_FRAMEBUFFER, 0);

			do
			{
				// current = total - tp.get_tasks_total();

				// nui.poll_input();

				glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
				glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

				// nui.new_frame();
				// nui.show_loading_bar("Loading Assets...", total, current);
				// nui.render();

				glfwSwapBuffers(w.handle());
				std::this_thread::sleep_for(1ms);
			} while (tp.get_tasks_total() > 0);

			// nui.poll_input();
			// nui.new_frame();
			// nui.show_building_atlas();

			glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

			// nui.render();

			glfwSwapBuffers(w.handle());
			build_resources();
		}

		void Loader::load_resources()
		{
			auto& tp = ServiceLocator<BS::thread_pool>::ref();

			tp.detach_task([]() -> void {
				ServiceLocator<resource::SoundCache>::ref().load_folder(GALAXY_SFX_DIR);
			});

			tp.detach_task([]() -> void {
				ServiceLocator<resource::MusicCache>::ref().load_folder(GALAXY_MUSIC_DIR);
			});

			tp.detach_task([]() -> void {
				ServiceLocator<resource::VoiceCache>::ref().load_folder(GALAXY_VOICE_DIR);
			});

			tp.detach_task([]() -> void {
				ServiceLocator<resource::VideoCache>::ref().load_folder(GALAXY_VIDEO_DIR);
			});

			tp.detach_task([]() -> void {
				ServiceLocator<resource::Animations>::ref().load_folder(GALAXY_ANIM_DIR);
			});

			tp.detach_task([]() -> void {
				ServiceLocator<resource::Shaders>::ref().load_folder(GALAXY_SHADER_DIR);
			});

			tp.detach_task([]() -> void {
				ServiceLocator<resource::Fonts>::ref().load_folder(GALAXY_FONT_DIR);
			});

			tp.detach_task([]() -> void {
				ServiceLocator<resource::Scripts>::ref().load_folder(GALAXY_SCRIPT_DIR);
			});

			tp.detach_task([]() -> void {
				ServiceLocator<resource::Prefabs>::ref().load_folder(GALAXY_PREFABS_DIR);
			});
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

		void Loader::build_resources()
		{
			auto& shaders = core::ServiceLocator<resource::Shaders>::ref();

			auto rtt = std::make_unique<graphics::Shader>();
			if (rtt->parse(resource::render_texture_shader_vert, resource::render_texture_shader_frag))
			{
				shaders.insert("render_to_texture", rtt);
			}

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

			auto& tex = ServiceLocator<resource::Textures>::ref();
			tex.load_folder(GALAXY_TEXTURE_DIR);
			for (auto&& [key, value] : tex.cache())
			{
				value->make_bindless();
			}
		}
	} // namespace core
} // namespace galaxy
