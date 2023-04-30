///
/// SceneManager.cpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#include <nlohmann/json.hpp>

#include "galaxy/algorithm/Base64.hpp"
#include "galaxy/algorithm/ZLib.hpp"
#include "galaxy/audio/AudioEngine.hpp"
#include "galaxy/core/Config.hpp"
#include "galaxy/core/Loader.hpp"
#include "galaxy/core/ServiceLocator.hpp"
#include "galaxy/core/Window.hpp"
#include "galaxy/error/Log.hpp"
#include "galaxy/fs/VirtualFileSystem.hpp"
#include "galaxy/input/Input.hpp"
#include "galaxy/physics/Constants.hpp"
#include "galaxy/resource/Fonts.hpp"
#include "galaxy/resource/Language.hpp"
#include "galaxy/resource/Maps.hpp"
#include "galaxy/resource/Materials.hpp"
#include "galaxy/resource/Prefabs.hpp"
#include "galaxy/resource/Scripts.hpp"
#include "galaxy/resource/Shaders.hpp"
#include "galaxy/resource/Sounds.hpp"
#include "galaxy/resource/TextureAtlas.hpp"
#include "galaxy/scripting/JSON.hpp"

#include "SceneManager.hpp"

using namespace std::chrono_literals;

namespace galaxy
{
	namespace scene
	{
		SceneManager::SceneManager()
			: m_current {nullptr}
		{
		}

		SceneManager::~SceneManager()
		{
			clear();
		}

		void SceneManager::load_assets()
		{
			auto& loader = core::ServiceLocator<core::Loader>::ref();
			loader.load([]() {
				try
				{
					//
					// Fetch services.
					//
					auto& config    = core::ServiceLocator<core::Config>::ref();
					auto& window    = core::ServiceLocator<core::Window>::ref();
					auto& ae        = core::ServiceLocator<audio::AudioEngine>::ref();
					auto& maps      = core::ServiceLocator<resource::Maps>::ref();
					auto& prefabs   = core::ServiceLocator<resource::Prefabs>::ref();
					auto& materials = core::ServiceLocator<resource::Materials>::ref();
					auto& scripts   = core::ServiceLocator<resource::Scripts>::ref();
					auto& sounds    = core::ServiceLocator<resource::Sounds>::ref();
					auto& lang      = core::ServiceLocator<resource::Language>::ref();
					auto& fonts     = core::ServiceLocator<resource::Fonts>::ref();
					auto& shaders   = core::ServiceLocator<resource::Shaders>::ref();

					maps.clear();
					prefabs.clear();
					materials.clear();
					scripts.clear();
					sounds.clear();
					lang.clear();
					fonts.clear();
					shaders.clear();

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
					// Window Icon.
					//
					auto icon = config.get<std::string>("icon", "window");
					if (!icon.empty())
					{
						window.set_icon(icon);
					}

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
					materials.load(config.get<std::string>("materials_folder", "resource_folders"));
				}
				catch (const std::exception& e)
				{
					GALAXY_LOG(GALAXY_ERROR, e.what());
				}
			});

			core::ServiceLocator<resource::Shaders>::ref().compile();
			core::ServiceLocator<resource::Fonts>::ref().build();

			core::ServiceLocator<resource::TextureAtlas>::ref().add_folder(
				core::ServiceLocator<core::Config>::ref().get<std::string>("atlas_folder", "resource_folders"));
		}

		std::weak_ptr<Scene> SceneManager::make_scene(const std::string& name)
		{
			if (!m_scenes.contains(name))
			{
				m_scenes[name]         = std::make_shared<Scene>();
				m_scenes[name]->m_name = name;

				return m_scenes[name];
			}
			else
			{
				GALAXY_LOG(GALAXY_ERROR, "Tried to create a duplicate scene '{0}'.", name);
				return {};
			}
		}

		void SceneManager::add_existing_scene(const std::string& name, std::shared_ptr<Scene> scene)
		{
			if (!m_scenes.contains(name))
			{
				m_scenes[name] = scene;
			}
			else
			{
				GALAXY_LOG(GALAXY_WARNING, "Attempted to add duplicate scene.");
			}
		}

		void SceneManager::set_scene(const std::string& name)
		{
			if (m_scenes.contains(name))
			{
				if (m_current)
				{
					m_current->unload();
				}

				m_current = m_scenes[name];
				m_current->load();
			}
			else
			{
				GALAXY_LOG(GALAXY_ERROR, "Tried to set a non-existent scene '{0}'.", name);
			}
		}

		void SceneManager::load_scene(const std::string& data)
		{
			const auto parsed = nlohmann::json::parse(data);

			if (!parsed.empty())
			{
				deserialize(parsed);
			}
			else
			{
				GALAXY_LOG(GALAXY_ERROR, "Failed to parse scenemanger JSON data from memory.");
			}
		}

		void SceneManager::unload_scene()
		{
			m_current->unload();
			m_current = nullptr;
		}

		void SceneManager::load_appdata(std::string_view appdata_file)
		{
			const auto data = core::ServiceLocator<fs::VirtualFileSystem>::ref().open(appdata_file);
			if (!data.empty())
			{
				const auto decoded_zlib = algorithm::decode_zlib(data);
				if (!decoded_zlib.empty())

				{
					const auto decoded_base64 = algorithm::decode_base64(decoded_zlib);
					if (!decoded_base64.empty())
					{
						load_scene(decoded_base64);
					}
					else
					{
						GALAXY_LOG(GALAXY_ERROR, "Failed to decode base64 appdata '{0}'.", appdata_file);
					}
				}
				else
				{
					GALAXY_LOG(GALAXY_ERROR, "Failed to decode zlib appdata '{0}'.", appdata_file);
				}
			}
			else
			{
				GALAXY_LOG(GALAXY_ERROR, "Failed to load appdata '{0}'.", appdata_file);
			}
		}

		void SceneManager::save_appdata(std::string_view file)
		{
			const auto json = serialize();

			if (!json::save_to_disk(file, json))
			{
				GALAXY_LOG(GALAXY_ERROR, "Failed to save '{0}' to disk.", file);
			}
		}

		std::weak_ptr<Scene> SceneManager::get(const std::string& name)
		{
			if (m_scenes.contains(name))
			{
				return m_scenes[name];
			}
			else
			{
				GALAXY_LOG(GALAXY_ERROR, "Failed to find '{0}'.", name);
				return {};
			}
		}

		bool SceneManager::remove(const std::string& name)
		{
			if (m_scenes.contains(name))
			{
				if (m_current != nullptr)
				{
					if (m_current->m_name != name)
					{
						m_scenes.erase(name);
						return true;
					}
					else
					{
						return false;
					}
				}
				else
				{
					m_scenes.erase(name);
					return true;
				}
			}
			else
			{
				return false;
			}
		}

		void SceneManager::clear(bool clear_current)
		{
			meta::vector<std::string> remove;
			remove.reserve(m_scenes.size());

			for (auto& [name, scene] : m_scenes)
			{
				if (m_current)
				{
					if (name != m_current->m_name || clear_current)
					{
						scene.reset();
						remove.push_back(name);
					}
				}
				else
				{
					scene.reset();
					remove.push_back(name);
				}
			}

			for (const auto& name : remove)
			{
				m_scenes.erase(name);
			}
		}

		Scene& SceneManager::current()
		{
			return *m_current;
		}

		bool SceneManager::has_current() const
		{
			return m_current != nullptr;
		}

		SceneContainer& SceneManager::all()
		{
			return m_scenes;
		}

		nlohmann::json SceneManager::serialize()
		{
			nlohmann::json json = "{\"current_scene\": \"\", \"scenes\": {}}"_json;
			if (m_current)
			{
				json["current_scene"] = m_current->m_name;
			}

			for (auto& [name, scene] : m_scenes)
			{
				json["scenes"][name] = scene->serialize();
			}

			return json;
		}

		void SceneManager::deserialize(const nlohmann::json& json)
		{
			clear(true);

			const auto& scenes = json.at("scenes");

			for (const auto& [key, value] : scenes.items())
			{
				auto scene = make_scene(key);
				if (auto ptr = scene.lock())
				{
					ptr->deserialize(value);
				}
			}

			set_scene(json.at("current_scene"));
		}
	} // namespace scene
} // namespace galaxy