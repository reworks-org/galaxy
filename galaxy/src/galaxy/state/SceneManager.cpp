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

		std::shared_ptr<Scene> SceneManager::make(const std::string& name)
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
				return nullptr;
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

		std::shared_ptr<Scene> SceneManager::current() noexcept
		{
			return m_current;
		}

		std::shared_ptr<Scene> SceneManager::get(const std::string& name) noexcept
		{
			if (m_scenes.contains(name))
			{
				return m_scenes[name];
			}
			else
			{
				GALAXY_LOG(GALAXY_ERROR, "Attempted to retrieve non-existent scene '{0}'.", name);
				return nullptr;
			}
		}

		void SceneManager::save(std::string_view file)
		{
			nlohmann::json json   = "{\"current_scene\": \"\", \"scenes\": {}}"_json;
			json["current_scene"] = m_current->get_name();

			for (auto& [name, scene] : m_scenes)
			{
				json["scenes"][name] = scene->serialize();
			}

			if (!json::save_to_disk(file, json))
			{
				GALAXY_LOG(GALAXY_ERROR, "Failed to save '{0}' to disk.", file);
			}
		}

		void SceneManager::load(std::string_view file)
		{
			const auto load_opt = json::parse_from_disk(file);

			if (load_opt.has_value())
			{
				// Only clear if read from disk was successful.
				clear();

				const auto& load   = load_opt.value();
				const auto& scenes = load.at("scenes");

				for (const auto& [key, value] : scenes.items())
				{
					auto scene = make(key);
					scene->deserialize(value);
				}

				change(load.at("current_scene"));
			}
			else
			{
				GALAXY_LOG(GALAXY_ERROR, "Failed to load save file '{0}' from disk.", file);
			}
		}

		void SceneManager::clear()
		{
			for (auto& [name, scene] : m_scenes)
			{
				scene.reset();
			}

			m_scenes.clear();
			m_current = nullptr;
		}
	} // namespace state
} // namespace galaxy