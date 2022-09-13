///
/// SceneManager.cpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#include <nlohmann/json.hpp>

#include "galaxy/error/Log.hpp"
#include "galaxy/scripting/JSON.hpp"

#include "SceneManager.hpp"

using namespace std::chrono_literals;

namespace galaxy
{
	namespace state
	{
		SceneManager::SceneManager() noexcept
			: m_current {nullptr}
		{
		}

		SceneManager::~SceneManager() noexcept
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

		void SceneManager::save(std::string_view file)
		{
			const auto json = serialize();

			if (!json::save_to_disk(file, json))
			{
				GALAXY_LOG(GALAXY_ERROR, "Failed to save '{0}' to disk.", file);
			}
		}

		std::weak_ptr<Scene> SceneManager::get(const std::string& name) noexcept
		{
			if (m_scenes.contains(name))
			{
				return m_scenes[name];
			}
			else
			{
				GALAXY_LOG(GALAXY_ERROR, "Attempted to retrieve non-existent scene '{0}'.", name);
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

		void SceneManager::clear()
		{
			std::vector<std::string> remove;
			remove.reserve(m_scenes.size());

			for (auto& [name, scene] : m_scenes)
			{
				if (name != m_current->get_name())
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

		Scene& SceneManager::current() noexcept
		{
			return *m_current;
		}

		SceneContainer& SceneManager::all() noexcept
		{
			return m_scenes;
		}

		nlohmann::json SceneManager::serialize()
		{
			nlohmann::json json   = "{\"current_scene\": \"\", \"scenes\": {}}"_json;
			json["current_scene"] = m_current->get_name();

			for (auto& [name, scene] : m_scenes)
			{
				json["scenes"][name] = scene->serialize();
			}

			return json;
		}

		void SceneManager::deserialize(const nlohmann::json& json)
		{
			// Only clear if read from disk was successful.
			clear();

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
	} // namespace state
} // namespace galaxy