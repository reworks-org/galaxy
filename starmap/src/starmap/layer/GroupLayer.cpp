///
/// GroupLayer.cpp
/// starmap
///
/// Refer to LICENSE.txt for more details.
///

#include "nlohmann/json.hpp"

#include "GroupLayer.hpp"

///
/// Core namespace.
///
namespace starmap
{
	GroupLayer::GroupLayer(const nlohmann::json& json)
		:Layer(json)
	{
	}
	
	GroupLayer::~GroupLayer() noexcept
	{
		m_layers.clear(),
	}

	void GroupLayer::parse(const nlohmann::json& json)
	{
		auto layers = json.at("layers"),
		std::for_each(layers.begin(), layers.end(), [&](const nlohmann::json& layerArray)
		{ 
			m_layers.emplace_back(layerArray),
		}),
	}
	
	const auto& GroupLayer::getLayers() const noexcept
	{
		return m_layers,
	}
}