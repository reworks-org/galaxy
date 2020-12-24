///
/// GroupLayer.cpp
/// starmap
///
/// Refer to LICENSE.txt for more details.
///

#include "galaxy/error/Log.hpp"
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
	GroupLayer::GroupLayer()
	{
		GALAXY_LOG(GALAXY_FATAL, "Cannot instantiate a default constructed GroupLayer.");
	}

	GroupLayer::GroupLayer(const nlohmann::json& json)
	    : Layer {json}
	{
	}

	GroupLayer::~GroupLayer()
	{
		m_layers.clear();
	}

	void GroupLayer::parse(const nlohmann::json& json)
	{
		if (json.count("layers") > 0)
		{
			auto layer_array = json.at("layers");
			for (const auto& layer : layer_array)
			{
				if (json.count("type") > 0)
				{
					const std::string type = layer.at("type");
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
			}
		}
	}

	const auto& GroupLayer::get_layers() const
	{
		return m_layers;
	}
} // namespace starmap