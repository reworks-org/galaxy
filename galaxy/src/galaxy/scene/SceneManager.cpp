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
#include "galaxy/systems/AnimationSystem.hpp"
#include "galaxy/systems/PhysicsSystem.hpp"
#include "galaxy/systems/ScriptSystem.hpp"

#include "SceneManager.hpp"

namespace galaxy
{
	namespace scene
	{
		SceneManager::SceneManager()
			: m_rendersystem_index {0}
		{
			create_system<systems::ScriptSystem>();
			create_system<systems::PhysicsSystem>();
			create_system<systems::AnimationSystem>();
			create_system<systems::RenderSystem>();
		}

		SceneManager::~SceneManager()
		{
			m_scenes.clear();
		}

		Scene* SceneManager::add(const std::string& name)
		{
			const auto hash = math::fnv1a_64(name.c_str());

			if (!m_scenes.contains(hash))
			{
				m_scenes[hash] = std::make_unique<Scene>(name);
			}
			else
			{
				GALAXY_LOG(GALAXY_WARNING, "Tried to add a duplicate scene '{0}'.", name);
			}

			return m_scenes[hash].get();
		}

		Scene* SceneManager::get(const std::string& name)
		{
			const auto hash = math::fnv1a_64(name.c_str());

			if (m_scenes.contains(hash))
			{
				return m_scenes[hash].get();
			}
			else
			{
				GALAXY_LOG(GALAXY_ERROR, "Failed to find '{0}'.", name);
				return nullptr;
			}
		}

		void SceneManager::remove(const std::string& name)
		{
			m_scenes.erase(math::fnv1a_64(name.c_str()));
		}

		bool SceneManager::has(const std::string& name)
		{
			return m_scenes.contains(math::fnv1a_64(name.c_str()));
		}

		void SceneManager::set_scene(const std::string& name)
		{
			const auto hash = math::fnv1a_64(name.c_str());
			if (m_scenes.contains(hash))
			{
				m_current = m_scenes[hash].get();
			}
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
			if (m_current)
			{
				m_current->update();

				graphics::Renderer::ref().flush();

				for (auto&& system : m_systems)
				{
					system->update(m_current->m_registry.m_entt);

					if (m_current->m_world.get_active())
					{
						system->update(m_current->m_world.get_active()->m_registry.m_entt);
					}
				}
			}
		}

		void SceneManager::only_update_ui()
		{
			if (m_current)
			{
				m_current->update_ui();
			}
		}

		void SceneManager::only_update_rendering()
		{
			if ((m_rendersystem_index >= 0 && m_rendersystem_index < m_systems.size()) && m_current != nullptr)
			{
				graphics::Renderer::ref().flush();
				m_systems[m_rendersystem_index]->update(m_current->m_registry.m_entt);

				if (m_current->m_world.get_active())
				{
					m_systems[m_rendersystem_index]->update(m_current->m_world.get_active()->m_registry.m_entt);
				}
			}
		}

		void SceneManager::render()
		{
			if (m_current)
			{
				m_current->render();
			}
		}

		void SceneManager::clear()
		{
			m_current = nullptr;
			m_scenes.clear();
		}

		scene::Scene* SceneManager::current() const
		{
			return m_current;
		}

		const SceneManager::Map& SceneManager::map() const
		{
			return m_scenes;
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

			return json;
		}

		void SceneManager::deserialize(const nlohmann::json& json)
		{
			clear();

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
		}
	} // namespace scene
} // namespace galaxy
