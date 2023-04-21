///
/// SceneManager.cpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#include <nlohmann/json.hpp>

#include "galaxy/algorithm/Base64.hpp"
#include "galaxy/algorithm/ZLib.hpp"
#include "galaxy/core/Loader.hpp"
#include "galaxy/core/ServiceLocator.hpp"
#include "galaxy/error/Log.hpp"
#include "galaxy/fs/VirtualFileSystem.hpp"
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

		std::weak_ptr<Scene> SceneManager::make(const std::string& name)
		{
			if (!m_scenes.contains(name))
			{
				m_scenes[name] = std::make_shared<Scene>();
				m_scenes[name]->set_name(name);

				return m_scenes[name];
			}
			else
			{
				GALAXY_LOG(GALAXY_ERROR, "Tried to create a duplicate scene '{0}'.", name);
				return {};
			}
		}

		void SceneManager::set(const std::string& name)
		{
			if (m_scenes.contains(name))
			{
				// need to draw loading screen?

				m_current = m_scenes[name];
				m_current->load();
			}
			else
			{
				GALAXY_LOG(GALAXY_ERROR, "Tried to set a non-existent scene '{0}'.", name);
			}
		}

		void SceneManager::change(const std::string& name)
		{
			if (!name.empty())
			{
				if (m_scenes.contains(name))
				{
					// need to draw loading screen?
					m_current->unload();
					m_current = m_scenes[name];
					m_current->load();
				}
				else
				{
					GALAXY_LOG(GALAXY_ERROR, "Tried to change to a non-existent scene '{0}'.", name);
				}
			}
		}

		void SceneManager::load(const std::string& data)
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

		void SceneManager::load_from_appdata(std::string_view appdata_file)
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
						load(decoded_base64);
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

		void SceneManager::load_assets()
		{
			auto& loader = core::ServiceLocator<core::Loader>::ref();
			loader.start();
			loader.finish();
		}

		void SceneManager::save(std::string_view file)
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
					if (m_current->get_name() != name)
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
					if (name != m_current->get_name() || clear_current)
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
				json["current_scene"] = m_current->get_name();
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
				auto scene = make(key);
				if (auto ptr = scene.lock())
				{
					ptr->deserialize(value);
				}
			}

			change(json.at("current_scene"));
		}
	} // namespace scene
} // namespace galaxy