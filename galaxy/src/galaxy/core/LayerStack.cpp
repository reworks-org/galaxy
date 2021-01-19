///
/// LayerStack.cpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#include "LayerStack.hpp"

namespace galaxy
{
	namespace core
	{
		LayerStack::~LayerStack()
		{
			clear();
		}

		void LayerStack::events()
		{
			if (!m_stack.empty())
			{
				m_stack.back()->events();
			}
			else
			{
				GALAXY_LOG(GALAXY_ERROR, "Attempted to access empty layer stack.");
			}
		}

		void LayerStack::update(const double dt)
		{
			if (!m_stack.empty())
			{
				m_stack.back()->update(dt);
			}
			else
			{
				GALAXY_LOG(GALAXY_ERROR, "Attempted to access empty layer stack.");
			}
		}

		void LayerStack::pre_render()
		{
			if (!m_stack.empty())
			{
				m_stack.back()->pre_render();
			}
			else
			{
				GALAXY_LOG(GALAXY_ERROR, "Attempted to access empty layer stack.");
			}
		}

		void LayerStack::render()
		{
			if (!m_stack.empty())
			{
				m_stack.back()->render();
			}
			else
			{
				GALAXY_LOG(GALAXY_ERROR, "Attempted to access empty layer stack.");
			}
		}

		void LayerStack::push(std::string_view name)
		{
			const auto str = static_cast<std::string>(name);
			if (m_layers.contains(str))
			{
				if (!m_stack.empty())
				{
					if (m_stack.back()->get_name() != str)
					{
						m_stack.push_back(m_layers[str]);
						m_stack.back()->on_push();
					}
					else
					{
						GALAXY_LOG(GALAXY_WARNING, "Tried to push state that is already on top.");
					}
				}
				else
				{
					m_stack.push_back(m_layers[str]);
					m_stack.back()->on_push();
				}
			}
			else
			{
				GALAXY_LOG(GALAXY_ERROR, "Tried to push non-existant layer.");
			}
		}

		void LayerStack::pop()
		{
			m_stack.back()->on_pop();
			m_stack.pop_back();

			// Make sure we dont pop an empty stack...
			// Or the only current state.
			if (m_stack.size() > 1)
			{
				m_stack.back()->on_pop();
				m_stack.pop_back();
			}
			else
			{
				GALAXY_LOG(GALAXY_WARNING, "Tried to pop last layer.");
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

		std::vector<std::string> LayerStack::get_layer_keys() noexcept
		{
			std::vector<std::string> keys;
			keys.reserve(m_layers.size());

			std::transform(m_layers.begin(), m_layers.end(), std::back_inserter(keys), [](const auto& pair) {
				return pair.first;
			});

			return keys;
		}
	} // namespace core
} // namespace galaxy