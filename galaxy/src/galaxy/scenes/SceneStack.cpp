///
/// SceneStack.cpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#include "SceneStack.hpp"

namespace galaxy
{
	namespace scenes
	{
		SceneStack::SceneStack() noexcept
		    : Serializable {this}
		{
		}

		SceneStack::~SceneStack()
		{
			clear();
		}

		std::shared_ptr<scenes::Scene> SceneStack::top()
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
					if (m_stack.back()->get_name() != str)
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
			nlohmann::json json = "{\"scenes\":{}}"_json;

			auto& scenes = json.at("scenes");
			for (const auto& [key, value] : m_scenes)
			{
				scenes[key] = value->serialize();

				if (key == top()->get_name())
				{
					json["top"] = top()->get_name();
				}
			}

			return json;
		}

		void SceneStack::deserialize(const nlohmann::json& json)
		{
			m_scenes.clear();

			const auto& scenes = json.at("scenes");
			for (const auto& [key, value] : scenes.items())
			{
				if (value.at("type") == "WORLD")
				{
					create<scenes::WorldScene>(key);
				}
				else if (value.at("type") == "GUI")
				{
					create<scenes::GUIScene>(key);
				}

				m_scenes[key]->deserialize(value);
			}

			push(json.at("top"));
		}
	} // namespace scenes
} // namespace galaxy