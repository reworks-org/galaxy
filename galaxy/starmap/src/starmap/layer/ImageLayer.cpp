///
/// ImageLayer.cpp
/// starmap
///
/// Refer to LICENSE.txt for more details.
///

#include <pulsar/Log.hpp>
#include <nlohmann/json.hpp>

#include "ImageLayer.hpp"

///
/// Core namespace.
///
namespace starmap
{
	ImageLayer::ImageLayer()
	{
		PL_LOG(PL_FATAL, "Cannot instantiate a default constructed ImageLayer.");
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

	ImageLayer::~ImageLayer() noexcept
	{
	}

	std::string ImageLayer::get_image() const noexcept
	{
		return m_image;
	}
} // namespace starmap