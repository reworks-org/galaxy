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
		ImageLayer::ImageLayer(const nlohmann::json& json, const int zlevel)
			: Layer {json, zlevel}
			, m_transparent_colour {255, 255, 255, 255}
		{
			if (json.count("image") > 0)
			{
				m_image = json.at("image");
			}

			if (json.count("transparentcolor") > 0)
			{
				m_transparent_colour = map::parse_hex_colour(json.at("transparentcolor"));
			}
		}

		const std::string& ImageLayer::get_image() const noexcept
		{
			return m_image;
		}

		const graphics::Colour& ImageLayer::get_transparent_colour() const noexcept
		{
			return m_transparent_colour;
		}
	} // namespace map
} // namespace galaxy