///
/// SceneManager.cpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#include <nlohmann/json.hpp>

#include "SceneManager.hpp"

namespace galaxy
{
	SceneManager::SceneManager() noexcept
		: StateMachine {}
	{
	}

	SceneManager::SceneManager(SceneManager&& w)
		: StateMachine {std::move(w)}
	{
	}

	SceneManager& SceneManager::operator=(SceneManager&& w)
	{
		if (this != &w)
		{
			StateMachine::operator=(std::move(w));
		}

		return *this;
	}

	SceneManager::~SceneManager()
	{
		clear();
	}

	void SceneManager::on_event(SDL_Event& event)
	{
		if (auto scene = top())
		{
			scene->on_event(event);
		}
	}

	void SceneManager::update()
	{
		if (auto scene = top())
		{
			scene->update(m_entity_manager);
		}
	}

	void SceneManager::render()
	{
		if (auto scene = top())
		{
			scene->render();
		}
	}

	void SceneManager::clear()
	{
		pop_all();

		m_stack.clear();
		m_storage.clear();
	}

	nlohmann::json SceneManager::serialize()
	{
		nlohmann::json json = "{\"scenes\":{}}"_json;

		for (auto& [id, scene] : m_storage)
		{
			json["scenes"][scene->name()] = scene->serialize();
		}

		for (auto i = 0; i < m_stack.size(); i++)
		{
			json["stack"][std::to_string(i)] = m_stack[i]->name();
		}

		/*
		auto& em         = entt::locator<meta::EntityFactory>::value();
		json["entities"] = nlohmann::json::array();
		for (const auto& [entity] : m_registry.m_entt.view<entt::entity>(entt::exclude<flags::NotSerializable>).each())
		{
			json["entities"].push_back(em.serialize_entity(entity, m_registry.m_entt));
		}
		*/

		return json;
	}

	void SceneManager::deserialize(const nlohmann::json& json)
	{
		clear();

		const auto& scenes = json.at("scenes");
		m_stack.reserve(scenes.size());
		for (const auto& [name, data] : scenes.items())
		{
			auto scene = add(name);
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
			m_stack.insert(m_stack.begin() + std::stoi(index), m_storage[hash]);
		}

		/*
		auto& em = entt::locator<meta::EntityFactory>::value();
		const auto& entity_json = json.at("entities");
		for (const auto& data : entity_json)
		{
			const auto entity = em.deserialize_entity(data, m_registry.m_entt);

			if (!m_registry.m_entt.all_of<components::Tag>(entity))
			{
				auto& tag = m_registry.m_entt.emplace<components::Tag>(entity);
				tag.m_tag = "Untagged";
			}
		}*/
	}
} // namespace galaxy

/*
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
*/
