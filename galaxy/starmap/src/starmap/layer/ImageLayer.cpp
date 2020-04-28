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
	ImageLayer::ImageLayer() noexcept
	{
		PL_LOG(PL_FATAL, "Cannot instantiate a default constructed ImageLayer!");
		abort();
	}

	ImageLayer::ImageLayer(const nlohmann::json& json) noexcept
		:Layer(json), m_image(""), m_transparentColour("000000")
	{
		if (json.count("image") > 0)
		{
			m_image = json.at("image");
		}

		if (json.count("transparentcolor") > 0)
		{
			m_transparentColour = json.at("transparentcolor");
		}
	}

	ImageLayer::~ImageLayer() noexcept
	{
	}

	const std::string& ImageLayer::getImage() const noexcept
	{
		return m_image;
	}
}