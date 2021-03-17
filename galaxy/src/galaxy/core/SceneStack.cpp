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

		std::shared_ptr<Scene> SceneStack::create(std::string_view name)
		{
			const auto str = static_cast<std::string>(name);
			if (m_scenes.contains(str))
			{
				GALAXY_LOG(GALAXY_WARNING, "Attempted to create a scene that already exists!");
			}
			else
			{
				m_scenes[str] = std::make_shared<Scene>(str);
				return m_scenes[str];
			}
		}

		std::shared_ptr<Scene> SceneStack::get(std::string_view name)
		{
			const auto str = static_cast<std::string>(name);
			if (m_scenes.contains(str))
			{
				return m_scenes[str];
			}
			else
			{
				GALAXY_LOG(GALAXY_ERROR, "Attempted to retrieve non-existant scene: {0}.", name);
				return nullptr;
			}
		}

		std::shared_ptr<core::Scene> SceneStack::top()
		{
			if (!m_stack.empty())
			{
				return m_stack.back();
			}
			else
			{
				GALAXY_LOG(GALAXY_ERROR, "No scenes in stack!");
				return nullptr;
			}
		}

		void SceneStack::events()
		{
			if (!m_stack.empty())
			{
				m_stack.back()->events();
			}
			else
			{
				GALAXY_LOG(GALAXY_ERROR, "Attempted to access empty scene stack.");
			}
		}

		void SceneStack::update(const double dt)
		{
			if (!m_stack.empty())
			{
				m_stack.back()->update(dt);
			}
			else
			{
				GALAXY_LOG(GALAXY_ERROR, "Attempted to access empty scene stack.");
			}
		}

		void SceneStack::pre_render()
		{
			if (!m_stack.empty())
			{
				m_stack.back()->pre_render();
			}
			else
			{
				GALAXY_LOG(GALAXY_ERROR, "Attempted to access empty scene stack.");
			}
		}

		void SceneStack::render()
		{
			if (!m_stack.empty())
			{
				m_stack.back()->render();
			}
			else
			{
				GALAXY_LOG(GALAXY_ERROR, "Attempted to access empty scene stack.");
			}
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
						m_stack.push_back(m_scenes[str]);
						m_stack.back()->on_push();
					}
					else
					{
						GALAXY_LOG(GALAXY_WARNING, "Tried to push state that is already on top.");
					}
				}
				else
				{
					m_stack.push_back(m_scenes[str]);
					m_stack.back()->on_push();
				}
			}
			else
			{
				GALAXY_LOG(GALAXY_ERROR, "Tried to push non-existant scene.");
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
			else
			{
				GALAXY_LOG(GALAXY_WARNING, "Tried to pop last scene.");
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

		SceneStorage& SceneStack::get_scenes() noexcept
		{
			return m_scenes;
		}

		std::vector<std::string> SceneStack::get_scene_keys() noexcept
		{
			std::vector<std::string> keys;
			keys.reserve(m_scenes.size());

			std::transform(m_scenes.begin(), m_scenes.end(), std::back_inserter(keys), [](const auto& pair) {
				return pair.first;
			});

			return keys;
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