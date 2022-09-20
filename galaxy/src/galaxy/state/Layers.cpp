///
/// Layers.cpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#include <nlohmann/json.hpp>

#include "Layers.hpp"

namespace galaxy
{
	namespace state
	{
		Layers::Layers(Scene* scene) noexcept
			: m_scene {scene}
		{
			// Reserve some initial memory.
			// In general 2 is ok. I.e. Game + UI.

			m_layers.reserve(2);
			m_stack.reserve(2);
		}

		Layers::~Layers() noexcept
		{
			clear();
		}

		void Layers::push(const std::string& name)
		{
			if (m_layers.contains(name))
			{
				if (!m_stack.empty())
				{
					if (m_stack.back()->get_name() != name)
					{
						m_stack.push_back(m_layers[name]);
						m_stack.back()->on_push();
					}
				}
				else
				{
					m_stack.push_back(m_layers[name]);
					m_stack.back()->on_push();
				}
			}
			else
			{
				GALAXY_LOG(GALAXY_WARNING, "Attempted to push unknown layer '{0}' to stack.", name);
			}
		}

		void Layers::pop()
		{
			if (m_stack.size() > 0)
			{
				m_stack.back()->on_pop();
				m_stack.pop_back();
			}
		}

		void Layers::pop(const std::string& name)
		{
			if (m_layers.contains(name))
			{
				for (auto it = m_stack.begin(); it != m_stack.end(); it++)
				{
					if ((*it)->get_name() == name)
					{
						m_stack.erase(it);
						break;
					}
				}
			}
			else
			{
				GALAXY_LOG(GALAXY_WARNING, "Attempted to pop unknown layer '{0}'.", name);
			}
		}

		void Layers::events()
		{
			for (auto it = m_stack.rbegin(); it != m_stack.rend(); ++it)
			{
				(*it)->events();
			}
		}

		void Layers::update()
		{
			for (auto i = 0; i < m_stack.size(); i++)
			{
				m_stack[i]->update();
			}
		}

		void Layers::render()
		{
			for (auto i = 0; i < m_stack.size(); i++)
			{
				m_stack[i]->render();
			}
		}

		void Layers::clear()
		{
			while (!m_stack.empty())
			{
				m_stack.back()->on_pop();
				m_stack.pop_back();
			}

			m_layers.clear();
		}

		const Layers::LayerStack& Layers::stack() const noexcept
		{
			return m_stack;
		}

		Layers::LayerContainer& Layers::cache() noexcept
		{
			return m_layers;
		}

		nlohmann::json Layers::serialize()
		{
			nlohmann::json json = "{\"layers\": {}, \"layer_stack\": {}, \"stack_size\": \"\"}"_json;

			auto& layers = json.at("layers");
			for (const auto& [key, value] : m_layers)
			{
				layers[key] = value->serialize();
			}

			auto& layer_stack = json.at("layer_stack");
			for (std::size_t i = 0; i < m_stack.size(); i++)
			{
				layer_stack[std::to_string(i)] = m_stack[i]->get_name();
			}

			json["stack_size"] = m_stack.size();

			return json;
		}

		void Layers::deserialize(const nlohmann::json& json)
		{
			clear();

			const auto& layers = json.at("layers");
			for (const auto& [key, value] : layers.items())
			{
				auto layer                 = LayerRegistry::m_registry[value.at("type")](value.at("name"), m_scene);
				m_layers[value.at("name")] = layer;

				layer->deserialize(value);
			}

			std::vector<std::string> names;
			names.resize(json.at("stack_size"));

			const auto& layer_stack = json.at("layer_stack");
			for (const auto& [key, value] : layer_stack.items())
			{
				names[std::stoi(key)] = value;
			}

			for (const auto& name : names)
			{
				push(name);
			}
		}
	} // namespace state
} // namespace galaxy