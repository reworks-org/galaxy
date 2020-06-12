///
/// LayerStorage.hpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#ifndef GALAXY_LAYERSTORAGE_HPP_
#define GALAXY_LAYERSTORAGE_HPP_

#include <vector>

#include "galaxy/layer/Layer.hpp"

///
/// Core namespace. 
///
namespace galaxy
{
	///
	/// Storage for layers in a state.
	///
	class LayerStorage final
	{
	public:
		///
		///
		///
		LayerStorage() noexcept;

		///
		///
		///
		~LayerStorage() noexcept;

		///
		///
		///
		template<typename Layer, typename... Args>
		void add(Args&& ...args) noexcept;

		///
		///
		///
		template<typename Layer>
		Layer* get(const std::string& name) noexcept;

		///
		///
		///
		void events() noexcept;

		///
		///
		///
		void update(protostar::ProtectedDouble* deltaTime) noexcept;

		///
		///
		///
		void render() noexcept;

		///
		///
		///
		void pop() noexcept;

		///
		///
		///
		void remove(const std::string& name) noexcept;

	private:
		///
		///
		///
		std::vector<std::unique_ptr<galaxy::Layer>> m_layers;
	};

	template<typename Layer, typename ...Args>
	inline void LayerStorage::add(Args&& ...args) noexcept
	{
		m_layers.emplace_back(std::make_unique<Layer>(std::forward<Args>(args)...));
	}

	template<typename Layer>
	inline Layer* LayerStorage::get(const std::string& name) noexcept
	{
		auto pos = std::find_if(m_layers.begin(), m_layers.end(), [&](std::unique_ptr<Layer> const& layer)
		{
			return layer->getName() == name;
		});

		if (pos != m_layers.end())
		{
			return dynamic_cast<Layer*>(*pos);
		}
		else
		{
			return nullptr;
		}
	}
}

#endif