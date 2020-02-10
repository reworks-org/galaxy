///
/// Layer.cpp
/// starmap
///
/// Refer to LICENSE.txt for more details.
///

#include <nlohmann/json.hpp>

#include "Layer.hpp"

///
/// Core namespace.
///
namespace starmap
{
	Layer::~Layer() noexcept
	{
		m_properties.clear();
	}

	const int Layer::getHeight() const noexcept
	{
		return m_height;
	}

	const int Layer::getID() const noexcept
	{
		return m_id;
	}

	const std::string& Layer::getName() const noexcept
	{
		return m_name;
	}

	const double Layer::getOffsetX() const noexcept
	{
		return m_offsetX;
	}

	const double Layer::getOffsetY() const noexcept
	{
		return m_offsetY;
	}

	const double Layer::getOpacity() const noexcept
	{
		return m_opacity;
	}

	const int Layer::getStartX() const noexcept
	{
		return m_startx;
	}

	const int Layer::getStartY() const noexcept
	{
		return m_starty;
	}

	const std::string& Layer::getType() const noexcept
	{
		return m_type;
	}

	const bool Layer::isVisible() const noexcept
	{
		return m_visible;
	}

	const int Layer::getWidth() const noexcept
	{
		return m_width;
	}

	const int Layer::getX() const noexcept
	{
		return m_x;
	}

	const int Layer::getY() const noexcept
	{
		return m_y;
	}

	Layer::Layer() noexcept
		:m_height(0), m_id(0), m_name(""), m_offsetX(0.0), m_offsetY(0.0), m_opacity(0.0), m_startx(0), m_starty(0), m_type(""), m_visible(true), m_width(0), m_x(0), m_y(0)
	{
	}

	Layer::Layer(const nlohmann::json& json)
		:m_height(0), m_id(0), m_name(""), m_offsetX(0.0), m_offsetY(0.0), m_opacity(0.0), m_startx(0), m_starty(0), m_type(""), m_visible(true), m_width(0), m_x(0), m_y(0)
	{
		if (json.count("height") > 0)
		{
			m_height = json.at("height");
		}
		
		if (json.count("id") > 0)
		{
			m_id = json.at("id");
		}
		
		if (json.count("name") > 0)
		{
			m_name = json.at("name");
		}
		
		if (json.count("offsetx") > 0)
		{
			m_offsetX = json.at("offsetx");
		}
		
		if (json.count("offsety") > 0)
		{
			m_offsetY = json.at("offsety");
		}
		
		if (json.count("opacity") > 0)
		{
			m_opacity = json.at("opacity");
		}

		if (json.count("properties") > 0)
		{
			auto propArray = json.at("properties");
			std::for_each(propArray.begin(), propArray.end(), [&](const nlohmann::json& property)
			{
				m_properties.emplace(property.at("name"), property);
			});
		}

		if (json.count("startx") > 0)
		{
			m_startx = json.at("startx");
		}

		if (json.count("starty") > 0)
		{
			m_starty = json.at("starty");
		}

		m_type = json.at("type");
		
		if (json.count("visible") > 0)
		{
			m_visible = json.at("visible");
		}

		m_width = json.at("width");
		m_x = json.at("x");
		m_y = json.at("y");
	}
}