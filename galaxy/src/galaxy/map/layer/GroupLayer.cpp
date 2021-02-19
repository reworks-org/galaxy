///
/// GroupLayer.cpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#include <nlohmann/json.hpp>

#include "galaxy/error/Log.hpp"
#include "galaxy/map/layer/ImageLayer.hpp"
#include "galaxy/map/layer/ObjectLayer.hpp"
#include "galaxy/map/layer/TileLayer.hpp"

#include "GroupLayer.hpp"

namespace galaxy
{
	namespace map
	{
		GroupLayer::GroupLayer()
		{
			GALAXY_LOG(GALAXY_FATAL, "Cannot instantiate a default constructed GroupLayer.");
		}

		GroupLayer::GroupLayer(const nlohmann::json& json)
		    : Layer {json}
		{
		}

		GroupLayer::~GroupLayer() noexcept
		{
			m_layers.clear();
		}

		void GroupLayer::parse(const nlohmann::json& json)
		{
			if (json.count("layers") > 0)
			{
				const auto& layer_array = json.at("layers");
				for (const auto& layer : layer_array)
				{
					if (json.count("type") > 0)
					{
						const std::string type = layer.at("type");
						if (type == "tilelayer")
						{
							m_layers.push_back(std::make_unique<TileLayer>());
						}
						else if (type == "objectgroup")
						{
							m_layers.push_back(std::make_unique<ObjectLayer>());
						}
						else if (type == "imagelayer ")
						{
							m_layers.push_back(std::make_unique<ImageLayer>());
						}
						else if (type == "group")
						{
							m_layers.push_back(std::make_unique<GroupLayer>());
						}
					}
				}
			}
		}

		const std::vector<std::unique_ptr<Layer>>& GroupLayer::get_layers() const noexcept
		{
			return m_layers;
		}
	} // namespace map
} // namespace galaxy