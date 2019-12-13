///
/// ImageLayer.cpp
/// starmap
///
/// Refer to LICENSE.txt for more details.
///

#include "nlohmann/json.hpp"

#include "ImageLayer.hpp"

///
/// Core namespace.
///
namespace starmap
{
	ImageLayer::ImageLayer(const nlohmann::json& json)
		:Layer(json), m_image(""), m_transparentColour("000000")
	{
		m_image = json.at("image");
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