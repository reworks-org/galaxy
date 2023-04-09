///
/// Loading.cpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#include <BS_thread_pool.hpp>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <RmlUi/Core.h>
#include <RmlUi/Core/Context.h>
#include <RmlUi/Debugger.h>

#include "galaxy/audio/AudioEngine.hpp"
#include "galaxy/core/Config.hpp"
#include "galaxy/core/ServiceLocator.hpp"
#include "galaxy/core/Window.hpp"
#include "galaxy/fs/VirtualFileSystem.hpp"
#include "galaxy/input/Input.hpp"
#include "galaxy/physics/Constants.hpp"
#include "galaxy/resource/Fonts.hpp"
#include "galaxy/resource/Language.hpp"
#include "galaxy/resource/Maps.hpp"
#include "galaxy/resource/Prefabs.hpp"
#include "galaxy/resource/Scripts.hpp"
#include "galaxy/resource/Shaders.hpp"
#include "galaxy/resource/Sounds.hpp"
#include "galaxy/resource/TextureAtlas.hpp"

#include "Loader.hpp"

namespace galaxy
{
	namespace core
	{
		Loader::Loader()
		{
			auto& config = ServiceLocator<Config>::ref();
			auto& fs     = ServiceLocator<fs::VirtualFileSystem>::ref();
			auto& window = ServiceLocator<Window>::ref();

			auto doc = fs.find(config.get<std::string>("load_screen_rml"));

			m_renderer.init();

			m_context = Rml::CreateContext("loader", Rml::Vector2i(window.get_width(), window.get_height()));
			Rml::Debugger::Initialise(m_context);

			auto rml_doc = m_context->LoadDocument(doc.string);
			if (rml_doc)
			{
				rml_doc->Show();
			}
			else
			{
				GALAXY_LOG(GALAXY_ERROR, "Failed to load '{0}' for loading screen.", doc.string);
			}
		}

		Loader::~Loader()
		{
			m_renderer.destroy();
		}

		void Loader::start()
		{
			auto& tp        = ServiceLocator<BS::thread_pool>::ref();
			m_thread_handle = tp.submit([]() {
				try
				{
					//
					// Fetch services.
					//
					auto& config  = ServiceLocator<Config>::ref();
					auto& window  = ServiceLocator<Window>::ref();
					auto& ae      = ServiceLocator<audio::AudioEngine>::ref();
					auto& maps    = ServiceLocator<resource::Maps>::ref();
					auto& prefabs = ServiceLocator<resource::Prefabs>::ref();
					auto& scripts = ServiceLocator<resource::Scripts>::ref();
					auto& sounds  = ServiceLocator<resource::Sounds>::ref();
					auto& lang    = ServiceLocator<resource::Language>::ref();
					auto& fonts   = ServiceLocator<resource::Fonts>::ref();
					auto& shaders = ServiceLocator<resource::Shaders>::ref();

					//
					// Set inputs from config.
					//
					input::CameraKeys::FORWARD      = input::int_to_key(config.get<int>("camera_foward", "input"));
					input::CameraKeys::BACKWARD     = input::int_to_key(config.get<int>("camera_backward", "input"));
					input::CameraKeys::LEFT         = input::int_to_key(config.get<int>("camera_left", "input"));
					input::CameraKeys::RIGHT        = input::int_to_key(config.get<int>("camera_right", "input"));
					input::CameraKeys::ROTATE_LEFT  = input::int_to_key(config.get<int>("camera_rotate_left", "input"));
					input::CameraKeys::ROTATE_RIGHT = input::int_to_key(config.get<int>("camera_rotate_right", "input"));

					//
					// Window closing config.
					//
					if (config.get<bool>("allow_native_closing", "window"))
					{
						window.allow_native_closing();
					}
					else
					{
						window.prevent_native_closing();
					}

					//
					// Window cursor.
					//
					auto& cursor = window.get_input<input::Cursor>();
					cursor.toggle(config.get<bool>("visible_cursor", "window"));

					auto cursor_icon = config.get<std::string>("cursor_icon", "window");
					if (!cursor_icon.empty())
					{
						cursor.set_cursor_icon(cursor_icon);
					}

					//
					// Physics constants.
					//
					physics::Constants::gravity.x           = config.get<float>("x", "box2d.gravity");
					physics::Constants::gravity.y           = config.get<float>("y", "box2d.gravity");
					physics::Constants::velocity_iterations = config.get<int>("velocity_iterations", "box2d");
					physics::Constants::position_iterations = config.get<int>("position_iterations", "box2d");
					physics::Constants::pixels_per_meter    = config.get<float>("ppm", "box2d");

					//
					// Resources.
					//
					shaders.load(config.get<std::string>("shader_folder", "resource_folders"));
					fonts.load(config.get<std::string>("font_folder", "resource_folders"));
					lang.load(config.get<std::string>("lang_folder", "resource_folders"));
					lang.set(config.get<std::string>("default_lang"));
					ae.set_sfx_volume(config.get<float>("sfx_volume", "audio"));
					ae.set_music_volume(config.get<float>("music_volume", "audio"));
					ae.set_dialogue_volume(config.get<float>("dialogue_volume", "audio"));
					sounds.load_sfx(config.get<std::string>("sfx_folder", "resource_folders"));
					sounds.load_music(config.get<std::string>("music_folder", "resource_folders"));
					sounds.load_dialogue(config.get<std::string>("dialogue_folder", "resource_folders"));
					scripts.load(config.get<std::string>("scripts_folder", "resource_folders"));
					prefabs.load(config.get<std::string>("prefabs_folder", "resource_folders"));
					maps.load(config.get<std::string>("maps_folder", "resource_folders"));
				}
				catch (const std::exception& e)
				{
					GALAXY_LOG(GALAXY_ERROR, e.what());
				}
			});
		}

		void Loader::finish()
		{
			// Use custom rendering interface for load screen.
			auto old = Rml::GetRenderInterface();
			Rml::SetRenderInterface(&m_renderer);

			while (!meta::is_work_done(m_thread_handle))
			{
				glfwPollEvents();
				m_context->Update();

				m_renderer.begin_frame();
				m_context->Render();
				m_renderer.end_frame();
			}

			m_thread_handle.get();
			Rml::SetRenderInterface(old);

			ServiceLocator<resource::Shaders>::ref().compile();
			ServiceLocator<resource::Fonts>::ref().build();

			ServiceLocator<resource::TextureAtlas>::ref().add_folder(ServiceLocator<Config>::ref().get<std::string>("atlas_folder", "resource_folders"));
		}
	} // namespace core
} // namespace galaxy