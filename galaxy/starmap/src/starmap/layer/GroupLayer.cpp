///
/// GroupLayer.cpp
/// starmap
///
/// Refer to LICENSE.txt for more details.
///

#include <pulsar/Log.hpp>
#include <nlohmann/json.hpp>

#include "starmap/layer/ImageLayer.hpp"
#include "starmap/layer/ObjectLayer.hpp"
#include "starmap/layer/TileLayer.hpp"

#include "GroupLayer.hpp"

///
/// Core namespace.
///
namespace starmap
{
	GroupLayer::GroupLayer() noexcept
	{
		PL_LOG(PL_FATAL, "Cannot instantiate a default constructed GroupLayer!");
		abort();
	}

	GroupLayer::GroupLayer(const nlohmann::json& json) noexcept
		:Layer(json)
	{
	}
	
	GroupLayer::~GroupLayer() noexcept
	{
		m_layers.clear();
	}

	void GroupLayer::parse(const nlohmann::json& json) noexcept
	{
		if (json.count("layers") > 0)
		{
			auto layerArray = json.at("layers");
			std::for_each(layerArray.begin(), layerArray.end(), [&](const nlohmann::json& layer)
			{
				if (json.count("type") > 0)
				{
					std::string type = layer.at("type");
					if (type == "tilelayer")
					{
						m_layers.push_back(std::make_unique<starmap::TileLayer>());
					}
					else if (type == "objectgroup")
					{
						m_layers.push_back(std::make_unique<starmap::ObjectLayer>());
					}
					else if (type == "imagelayer ")
					{
						m_layers.push_back(std::make_unique<starmap::ImageLayer>());
					}
					else if (type == "group")
					{
						m_layers.push_back(std::make_unique<starmap::GroupLayer>());
					}
				}
			});
		}
	}
	
	const auto& GroupLayer::getLayers() const noexcept
	{
		return m_layers;
	}
}