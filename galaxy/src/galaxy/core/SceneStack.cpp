///
/// SceneStack.cpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#include "SceneStack.hpp"

namespace galaxy
{
	namespace core
	{
		SceneStack::SceneStack() noexcept
			: Serializable {this}
		{
		}

		SceneStack::~SceneStack()
		{
			clear();
		}

		Scene2D* SceneStack::create(std::string_view name)
		{
			const auto str = static_cast<std::string>(name);

			if (m_scenes.contains(str))
			{
				GALAXY_LOG(GALAXY_WARNING, "Scene already exists.");
				return nullptr;
			}
			else
			{
				m_scenes[str] = std::make_unique<Scene2D>(str);
				return m_scenes[str].get();
			}
		}

		Scene2D* SceneStack::top()
		{
			if (!m_stack.empty())
			{
				return m_stack.back();
			}

			return nullptr;
		}

		void SceneStack::push(std::string_view name)
		{
			const auto str = static_cast<std::string>(name);

			if (m_scenes.contains(str))
			{
				if (!m_stack.empty())
				{
					if (m_stack.back()->m_name != str)
					{
						m_stack.push_back(m_scenes[str].get());
						m_stack.back()->on_push();
					}
				}
				else
				{
					m_stack.push_back(m_scenes[str].get());
					m_stack.back()->on_push();
				}
			}
			else
			{
				GALAXY_LOG(GALAXY_WARNING, "Attempted to push unknown scene to stack.");
			}
		}

		void SceneStack::pop()
		{
			// Make sure we dont pop an empty stack...
			// Or the only current state.
			if (m_stack.size() > 1)
			{
				m_stack.back()->on_pop();
				m_stack.pop_back();
			}
		}

		void SceneStack::clear()
		{
			while (!m_stack.empty())
			{
				m_stack.back()->on_pop();
				m_stack.pop_back();
			}

			m_scenes.clear();
		}

		void SceneStack::events()
		{
			if (!m_stack.empty())
			{
				m_stack.back()->events();
			}
		}

		void SceneStack::update()
		{
			if (!m_stack.empty())
			{
				m_stack.back()->update();
			}
		}

		void SceneStack::pre_render()
		{
			if (!m_stack.empty())
			{
				m_stack.back()->pre_render();
			}
		}

		void SceneStack::render()
		{
			if (!m_stack.empty())
			{
				m_stack.back()->render();
			}
		}

		Scene2D* SceneStack::get(std::string_view name)
		{
			const auto str = static_cast<std::string>(name);

			if (m_scenes.contains(str))
			{
				return m_scenes[str].get();
			}
			else
			{
				GALAXY_LOG(GALAXY_ERROR, "Scene with name: {0} does not exist.", name);
				return nullptr;
			}
		}

		robin_hood::unordered_node_map<std::string, std::unique_ptr<Scene2D>>& SceneStack::get_scenes() noexcept
		{
			return m_scenes;
		}

		std::vector<std::string> SceneStack::get_scene_keys() noexcept
		{
			std::vector<std::string> keys;
			keys.reserve(m_scenes.size());

			// clang-format off
			std::transform(m_scenes.begin(), m_scenes.end(), std::back_inserter(keys), [](const auto& pair) {
				return pair.first;
			});
			// clang-format on

			return keys;
		}

		std::vector<Scene2D*>& SceneStack::get_stack() noexcept
		{
			return m_stack;
		}

		nlohmann::json SceneStack::serialize()
		{
			nlohmann::json json = "{\"scenes\":{},\"scene-stack\":{}}"_json;

			auto& scenes = json.at("scenes");
			for (const auto& [key, value] : m_scenes)
			{
				scenes[key] = value->serialize();
			}

			auto& scene_stack = json.at("scene-stack");
			for (std::size_t i = 0; i < m_stack.size(); i++)
			{
				scene_stack[std::to_string(i)] = m_stack[i]->m_name;
			}

			json["stack-size"] = m_stack.size();

			return json;
		}

		void SceneStack::deserialize(const nlohmann::json& json)
		{
			m_scenes.clear();

			const auto& scenes = json.at("scenes");
			for (const auto& [key, value] : scenes.items())
			{
				create(key);
				m_scenes[key]->deserialize(value);
			}

			std::vector<std::string> names;
			names.resize(json.at("stack-size"));

			const auto& scene_stack = json.at("scene-stack");
			for (const auto& [key, value] : scene_stack.items())
			{
				names[std::stoi(key)] = value;
			}

			for (const auto& name : names)
			{
				push(name);
			}
		}
	} // namespace core
} // namespace galaxy