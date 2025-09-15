///
/// World.cpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#include "galaxy/math/FNV1a.hpp"
#include "galaxy/logging/Log.hpp"

#include "World.hpp"
#include "Scene.hpp"

namespace galaxy
{
	World::World() noexcept
	{
	}

	World::~World() noexcept
	{
		clear();
	}

	std::shared_ptr<Scene> World::add(const std::string& name)
	{
		const auto hash = math::fnv1a(name.c_str());

		if (!m_scenes.contains(hash))
		{
			m_scenes[hash] = std::make_shared<Scene>(name);
			return m_scenes[hash];
		}
		else
		{
			GALAXY_LOG(GALAXY_WARN, "Tried to create a scene with a duplicate name of '{0}'.", name);
			return nullptr;
		}
	}

	std::shared_ptr<Scene> World::get(const std::string& name) noexcept
	{
		const auto hash = math::fnv1a(name.c_str());

		if (m_scenes.contains(hash))
		{
			return m_scenes[hash];
		}
		else
		{
			GALAXY_LOG(GALAXY_WARN, "Scene '{0}' does not exist.", name);
			return nullptr;
		}
	}

	void World::remove(const std::string& name)
	{
		const auto hash = math::fnv1a(name.c_str());
		m_scenes.erase(hash);
	}

	bool World::has(const std::string& name) noexcept
	{
		const auto hash = math::fnv1a(name.c_str());
		return m_scenes.contains(hash);
	}

	void World::push(const std::string& name) noexcept
	{
		if (auto scene = get(name))
		{
			m_scene_stack.push_back(scene);
			m_scene_stack.back()->load();
		}
		else
		{
			GALAXY_LOG(GALAXY_WARN, "Tried to push non-existent scene '{0}'.", name);
		}
	}

	void World::pop() noexcept
	{
		if (m_scene_stack.size() > 0)
		{
			m_scene_stack.back()->unload();
			m_scene_stack.pop_back();
		}
	}

	void World::pop_all() noexcept
	{
		while (m_scene_stack.size() > 0)
		{
			pop();
		}
	}

	std::shared_ptr<Scene> World::top() const noexcept
	{
		if (m_scene_stack.size() > 0)
		{
			return m_scene_stack.back();
		}

		return nullptr;
	}

	void World::update()
	{
		if (m_scene_stack.size() > 0)
		{
			return m_scene_stack.back()->update(m_registry);
		}
	}

	void World::render()
	{
		if (m_scene_stack.size() > 0)
		{
			return m_scene_stack.back()->render();
		}
	}

	void World::clear()
	{
		m_scene_stack.clear();
		m_scenes.clear();
	}

	World::SceneMap& World::get_scenes() noexcept
	{
		return m_scenes;
	}

	World::SceneStack& World::get_scene_stack() noexcept
	{
		return m_scene_stack;
	}
} // namespace galaxy

/*
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
			for (auto&& system : m_systems)
			{
				system->update(m_current->m_registry.m_entt);

				if (m_current->m_world.get_active())
				{
					system->update(m_current->m_world.get_active()->m_registry.m_entt);
				}
			}
		}

		void SceneManager::only_update_rendering()
		{
			if ((m_rendersystem_index >= 0 && m_rendersystem_index < m_systems.size()) && m_current != nullptr)
			{
				m_systems[m_rendersystem_index]->update(m_current->m_registry.m_entt);

				if (m_current->m_world.get_active())
				{
					m_systems[m_rendersystem_index]->update(m_current->m_world.get_active()->m_registry.m_entt);
				}
			}
		}

		nlohmann::json SceneManager::serialize()
		{
			nlohmann::json json = "{\"scenes\":{}}"_json;

			for (auto& [name, scene] : m_scenes)
			{
				json["scenes"][scene->m_name] = scene->serialize();
			}

			if (m_current)
			{
				json["current"] = m_current->m_name;
			}

			return json;
		}

		void SceneManager::deserialize(const nlohmann::json& json)
		{
			clear();

			const auto& scenes = json.at("scenes");

			m_scenes.reserve(scenes.size());
			for (const auto& [name, data] : scenes.items())
			{
				auto scene = add(name);
				if (scene)
				{
					scene->deserialize(data);
				}
			}

			if (json.contains("current"))
			{
				set_scene(json.at("current"));
			}
		}

		void SceneManager::load_app(const std::string& appdata_file)
		{
			std::ifstream in(appdata_file, std::ifstream::in);

			std::stringstream buffer;
			buffer << in.rdbuf();

			auto data = buffer.str();
			if (!data.empty())
			{
				if constexpr (!GALAXY_DEBUG_BUILD)
				{
					data = math::decode_zlib(data);
					if (!data.empty())
					{
						data = math::decode_base64(data);
						if (data.empty())
						{
							GALAXY_LOG(GALAXY_ERROR, "Failed to decode base64 appdata '{0}'.", appdata_file);
						}
					}
					else
					{
						GALAXY_LOG(GALAXY_ERROR, "Failed to decode zlib appdata '{0}'.", appdata_file);
					}

					if (data.empty())
					{
						return;
					}
				}

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
			else
			{
				GALAXY_LOG(GALAXY_ERROR, "Failed to load appdata '{0}'.", appdata_file);
			}
		}

		void SceneManager::save_app(const std::string& file)
		{
			const auto json = serialize();
			auto       data = json::dump(json);

			if constexpr (!GALAXY_DEBUG_BUILD)
			{
				data = math::encode_base64(data);
				if (!data.empty())
				{
					data = math::encode_zlib(data);
					if (data.empty())
					{
						GALAXY_LOG(GALAXY_ERROR, "Failed to encode scenemanager to zlib '{0}'.", file);
					}
				}
				else
				{
					GALAXY_LOG(GALAXY_ERROR, "Failed to encode scenemanager to base64 '{0}'.", file);
				}

				if (data.empty())
				{
					return;
				}
			}

			auto& fs = entt::locator<fs::VirtualFileSystem>::value();
			if (!fs.write(data, file))
			{
				GALAXY_LOG(GALAXY_ERROR, "Failed to save '{0}' to disk.", file);
			}
		}

		nlohmann::json SceneManager::serialize()
		{
			auto& em = entt::locator<meta::EntityFactory>::value();

			nlohmann::json json = "{\"scenes\":{}}"_json;

			for (auto& [id, scene] : m_scenes)
			{
				json["scenes"][scene->name()] = scene->serialize();
			}

			for (auto i = 0; i < m_stack.size(); i++)
			{
				json["stack"][std::to_string(i)] = m_stack[i]->name();
			}

			json["entities"] = nlohmann::json::array();
			for (const auto& [entity] : m_registry.m_entt.view<entt::entity>(entt::exclude<flags::NotSerializable>).each())
			{
				json["entities"].push_back(em.serialize_entity(entity, m_registry.m_entt));
			}

			return json;
		}

		void SceneManager::deserialize(const nlohmann::json& json)
		{
			auto& em = entt::locator<meta::EntityFactory>::value();

			pop_all();
			clear();

			const auto& scenes = json.at("scenes");
			m_scenes.reserve(scenes.size());
			for (const auto& [name, data] : scenes.items())
			{
				auto scene = create(name);
				if (scene)
				{
					scene->deserialize(data);
				}
			}

			const auto& stack = json.at("stack");
			m_stack.reserve(stack.size());
			for (const auto& [index, name] : stack.items())
			{
				const auto hash = math::fnv1a(name.get<std::string>().c_str());
				m_stack.insert(m_stack.begin() + std::stoi(index), m_scenes[hash]);
			}

			const auto& entity_json = json.at("entities");
			for (const auto& data : entity_json)
			{
				const auto entity = em.deserialize_entity(data, m_registry.m_entt);

				if (!m_registry.m_entt.all_of<components::Tag>(entity))
				{
					auto& tag = m_registry.m_entt.emplace<components::Tag>(entity);
					tag.m_tag = "Untagged";
				}
			}
		}
*/
