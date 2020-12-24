///
/// ImageLayer.cpp
/// starmap
///
/// Refer to LICENSE.txt for more details.
///

#include "galaxy/error/Log.hpp"
#include <nlohmann/json.hpp>

#include "ImageLayer.hpp"

///
/// Core namespace.
///
namespace starmap
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
} // namespace starmap