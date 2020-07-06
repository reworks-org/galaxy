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
	LayerStorage::LayerStorage() noexcept
	{
	}

	LayerStorage::~LayerStorage() noexcept
	{
		clear();
	}

	void LayerStorage::events() noexcept
	{
		for (auto&& layer : m_layers)
		{
			layer->events();
		}
	}

	void LayerStorage::update(protostar::ProtectedDouble* deltaTime) noexcept
	{
		for (auto&& layer : m_layers)
		{
			layer->update(deltaTime);
		}
	}

	void LayerStorage::render(qs::Camera& camera) noexcept
	{
		for (auto&& layer : m_layers)
		{
			layer->render(camera);
		}
	}

	void LayerStorage::pop() noexcept
	{
		m_layers.pop_back();
	}

	void LayerStorage::remove(const std::string& name) noexcept
	{
		m_layers.erase(std::find_if(m_layers.begin(), m_layers.end(), [&](std::unique_ptr<Layer> const& layer) noexcept
		{
			return layer->getName() == name;
		}));
	}

	void LayerStorage::clear() noexcept
	{
		for (auto&& layer : m_layers)
		{
			layer.reset();
		}

		m_layers.clear();
	}
}