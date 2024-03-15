///
/// SceneManager.cpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#include <nlohmann/json.hpp>

#include "galaxy/core/ServiceLocator.hpp"
#include "galaxy/fs/VirtualFileSystem.hpp"
#include "galaxy/graphics/Renderer.hpp"
#include "galaxy/math/Base64.hpp"
#include "galaxy/math/ZLib.hpp"
#include "galaxy/scripting/JSON.hpp"

#include "SceneManager.hpp"

void move(galaxy::meta::vector<std::shared_ptr<galaxy::scene::Scene>>& list, std::size_t old, size_t new_)
{
	if (old > new_)
	{
		std::rotate(list.rend() - old - 1, list.rend() - old, list.rend() - new_);
	}
	else
	{
		std::rotate(list.begin() + old, list.begin() + old + 1, list.begin() + new_ + 1);
	}
}

namespace galaxy
{
	namespace scene
	{
		SceneManager::SceneManager()
		{
		}

		SceneManager::~SceneManager()
		{
			m_order.clear();
			m_scenes.clear();
		}

		std::shared_ptr<Scene> SceneManager::add(const std::string& name)
		{
			const auto hash = math::fnv1a_64(name.c_str());

			if (!m_scenes.contains(hash))
			{
				m_scenes[hash] = std::make_shared<Scene>(name);
				m_order.push_back(m_scenes[hash]);

				return m_scenes[hash];
			}
			else
			{
				GALAXY_LOG(GALAXY_WARNING, "Tried to add a duplicate scene '{0}'.", name);
				return m_scenes[hash];
			}
		}

		std::shared_ptr<Scene> SceneManager::get(const std::string& name)
		{
			const auto hash = math::fnv1a_64(name.c_str());

			if (m_scenes.contains(hash))
			{
				return m_scenes[hash];
			}
			else
			{
				GALAXY_LOG(GALAXY_ERROR, "Failed to find '{0}'.", name);
				return nullptr;
			}
		}

		void SceneManager::remove(const std::string& name)
		{
			// clang-format off
			m_order.erase(std::remove_if(m_order.begin(), m_order.end(),
				[&](auto&& scene) -> bool {
					return scene->m_name == name;
				}), 
				m_order.end()
			);
			// clang-format on

			m_scenes.erase(math::fnv1a_64(name.c_str()));
		}

		bool SceneManager::has(const std::string& name)
		{
			return m_scenes.contains(math::fnv1a_64(name.c_str()));
		}

		void SceneManager::load_app(const std::string& appdata_file)
		{
			const auto data = core::ServiceLocator<fs::VirtualFileSystem>::ref().read(appdata_file);
			if (!data.empty())
			{
				const auto decoded_zlib = math::decode_zlib(data);
				if (!decoded_zlib.empty())

				{
					const auto decoded_base64 = math::decode_base64(decoded_zlib);
					if (!decoded_base64.empty())
					{
						const auto parsed = nlohmann::json::parse(decoded_base64);

						if (!parsed.empty())
						{
							deserialize(parsed);
						}
						else
						{
							GALAXY_LOG(GALAXY_ERROR, "Failed to parse scenemanger JSON data from memory.");
						}
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

		void SceneManager::save_app(const std::string& file)
		{
			const auto json = serialize();

			if (!json::write(file, json))
			{
				GALAXY_LOG(GALAXY_ERROR, "Failed to save '{0}' to disk.", file);
			}
		}

		void SceneManager::update()
		{
			for (auto&& scene : m_order)
			{
				if (scene->enabled())
				{
					scene->update();
				}
			}
		}

		void SceneManager::render()
		{
			graphics::Renderer::ref().begin_post();

			for (auto&& scene : m_order)
			{
				if (scene->enabled())
				{
					scene->render();
				}
			}

			// graphics::Renderer::ref().end_post();
			// graphics::Renderer::ref().begin_default();
			// graphics::Renderer::ref().render_post();
			graphics::Renderer::ref().end_default();
		}

		void SceneManager::update_ui()
		{
			for (auto&& scene : m_order)
			{
				if (scene->enabled())
				{
					scene->update_ui();
				}
			}
		}

		void SceneManager::update_rendering()
		{
			for (auto&& scene : m_order)
			{
				if (scene->enabled())
				{
					scene->update_rendering();
				}
			}
		}

		void SceneManager::clear()
		{
			m_order.clear();
			m_scenes.clear();
		}

		const SceneManager::Map& SceneManager::map() const
		{
			return m_scenes;
		}

		const SceneManager::List& SceneManager::list() const
		{
			return m_order;
		}

		std::size_t SceneManager::size() const
		{
			return m_scenes.size();
		}

		bool SceneManager::empty() const
		{
			return m_scenes.size() == 0;
		}

		nlohmann::json SceneManager::serialize()
		{
			nlohmann::json json = "{\"scenes\":{},\"order\":{}}"_json;

			for (auto& [name, scene] : m_scenes)
			{
				json["scenes"][scene->m_name] = scene->serialize();
			}

			for (int i = 0; i < m_order.size(); i++)
			{
				json["order"][std::to_string(i)] = m_order[i]->m_name;
			}

			return json;
		}

		void SceneManager::deserialize(const nlohmann::json& json)
		{
			m_order.clear();
			m_scenes.clear();

			const auto& scenes = json.at("scenes");
			const auto& order  = json.at("order");

			m_scenes.reserve(scenes.size());
			for (const auto& [name, data] : scenes.items())
			{
				auto scene = add(name);
				if (scene)
				{
					scene->deserialize(data);
				}
			}

			m_order.reserve(order.size());
			for (const auto& [index, name] : order.items())
			{
				const auto name_str = name.get<std::string>();
				m_order.insert(m_order.begin() + std::stoi(index), m_scenes[math::fnv1a_64(name_str.c_str())]);
			}
		}
	} // namespace scene
} // namespace galaxy
