///
/// ImageLayer.cpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#include <nlohmann/json.hpp>

#include "galaxy/error/Log.hpp"

#include "ImageLayer.hpp"

namespace galaxy
{
	namespace map
	{
		ImageLayer::ImageLayer()
		{
			GALAXY_LOG(GALAXY_FATAL, "Cannot instantiate a default constructed ImageLayer.");
		}

		ImageLayer::ImageLayer(const nlohmann::json& json)
		    : Layer {json}, m_image {""}, m_transparent_colour {"000000"}
		{
			if (json.count("image") > 0)
			{
				m_image = json.at("image");
			}

			if (json.count("transparentcolor") > 0)
			{
				m_transparent_colour = json.at("transparentcolor");
			}
		}

		ImageLayer::~ImageLayer()
		{
		}

		std::string ImageLayer::get_image() const
		{
			return m_image;
		}
	} // namespace map
} // namespace galaxy