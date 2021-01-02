///
/// LayerStorage.cpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#include "LayerStorage.hpp"

namespace galaxy
{
	namespace core
	{
		LayerStorage::~LayerStorage()
		{
			clear();
		}

		void LayerStorage::events()
		{
			for (const auto& layer : m_stack)
			{
				layer->events();
			}
		}

		void LayerStorage::update(const double dt)
		{
			for (const auto& layer : m_stack)
			{
				layer->update(dt);
			}
		}

		void LayerStorage::pre_render()
		{
			for (const auto& layer : m_stack)
			{
				layer->pre_render();
			}
		}

		void LayerStorage::render()
		{
			for (const auto& layer : m_stack)
			{
				layer->render();
			}
		}

		void LayerStorage::push(std::string_view name)
		{
			const auto str = static_cast<std::string>(name);
			if (m_layers.contains(str))
			{
				m_stack.push_back(m_layers[str]);
			}
		}

		void LayerStorage::pop()
		{
			m_stack.pop_back();
		}

		void LayerStorage::clear()
		{
			m_stack.clear();
			m_layers.clear();
		}

		std::vector<std::string> LayerStorage::get_layer_keys()
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