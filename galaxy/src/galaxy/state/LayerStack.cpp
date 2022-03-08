///
/// LayerStack.cpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#include <nlohmann/json.hpp>

#include "galaxy/error/Log.hpp"

#include "LayerStack.hpp"

namespace galaxy
{
	namespace state
	{
		LayerStack::LayerStack() noexcept
		{
			m_stack.reserve(1);
		}

		LayerStack::~LayerStack() noexcept
		{
			clear();
		}

		std::shared_ptr<Layer> LayerStack::make(const std::string& name)
		{
			if (!m_layers.contains(name))
			{
				m_layers[name] = std::make_shared<Layer>(name);
				return m_layers[name];
			}
			else
			{
				GALAXY_LOG(GALAXY_ERROR, "Layer '{0}' already exists.", name);
				return nullptr;
			}
		}

		std::shared_ptr<Layer> LayerStack::get(const std::string& name)
		{
			if (m_layers.contains(name))
			{
				return m_layers[name];
			}
			else
			{
				GALAXY_LOG(GALAXY_ERROR, "Layer with name '{0}' does not exist.", name);
				return nullptr;
			}
		}

		std::shared_ptr<Layer> LayerStack::top()
		{
			if (!m_stack.empty())
			{
				return m_stack.back();
			}
			else
			{
				return nullptr;
			}
		}

		void LayerStack::push(const std::string& name)
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

		void LayerStack::pop()
		{
			// Make sure we dont pop an empty stack.
			// Or the only current state.

			if (m_stack.size() > 1)
			{
				m_stack.back()->on_pop();
				m_stack.pop_back();
			}
		}

		void LayerStack::events()
		{
			if (!m_stack.empty())
			{
				m_stack.back()->events();
			}
		}

		void LayerStack::update()
		{
			if (!m_stack.empty())
			{
				m_stack.back()->update();
			}
		}

		void LayerStack::pre_render()
		{
			if (!m_stack.empty())
			{
				m_stack.back()->pre_render();
			}
		}

		void LayerStack::render()
		{
			if (!m_stack.empty())
			{
				m_stack.back()->render();
			}
		}

		void LayerStack::clear()
		{
			while (!m_stack.empty())
			{
				m_stack.back()->on_pop();
				m_stack.pop_back();
			}

			m_layers.clear();
		}

		std::vector<std::shared_ptr<Layer>>& LayerStack::get_stack() noexcept
		{
			return m_stack;
		}

		LayerContainer& LayerStack::get_layers() noexcept
		{
			return m_layers;
		}

		std::vector<std::string> LayerStack::get_layer_keys() noexcept
		{
			std::vector<std::string> keys(m_layers.size());

			std::transform(m_layers.begin(), m_layers.end(), std::back_inserter(keys), [](const auto& pair) {
				return pair.first;
			});

			return keys;
		}

		nlohmann::json LayerStack::serialize()
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

		void LayerStack::deserialize(const nlohmann::json& json)
		{
			clear();

			const auto& layers = json.at("layers");
			for (const auto& [key, value] : layers.items())
			{
				auto ptr = make(key);
				ptr->deserialize(value);
			}

			std::vector<std::string> names(json.at("stack_size"));

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