///
/// LayerStorage.cpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#include "LayerStorage.hpp"

///
/// Core namespace.
///
namespace galaxy
{
	LayerStorage::~LayerStorage()
	{
		clear();
	}

	void LayerStorage::events()
	{
		for (auto&& layer : m_layers)
		{
			layer->events();
		}
	}

	void LayerStorage::update(const double dt)
	{
		for (auto&& layer : m_layers)
		{
			layer->update(dt);
		}
	}

	void LayerStorage::render(qs::Camera& camera)
	{
		for (auto&& layer : m_layers)
		{
			layer->render(camera);
		}
	}

	void LayerStorage::pop()
	{
		m_layers.pop_back();
	}

	void LayerStorage::remove(std::string_view name)
	{
		std::erase_if(m_layers, [&](const auto& layer) {
			return layer->get_name() == static_cast<std::string>(name);
		});
	}

	void LayerStorage::clear()
	{
		for (auto&& layer : m_layers)
		{
			layer.reset();
		}

		m_layers.clear();
	}
} // namespace galaxy