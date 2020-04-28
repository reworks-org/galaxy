///
/// Object.cpp
/// starmap
///
/// Refer to LICENSE.txt for more details.
///

#include <nlohmann/json.hpp>

#include "Object.hpp"

///
/// Core namespace.
///
namespace starmap
{
	Object::Object() noexcept
		:m_ellipse(false), m_gid(0), m_height(0.0), m_id(0), m_name(""), m_point(false), m_rotation(0.0), m_template(""), m_type(""), m_visible(true), m_width(0.0), m_x(0.0), m_y(0.0)
	{
	}

	Object::Object(const nlohmann::json& json) noexcept
		:m_ellipse(false), m_gid(0), m_height(0.0), m_id(0), m_name(""), m_point(false), m_rotation(0.0), m_template(""), m_type(""), m_visible(true), m_width(0.0), m_x(0.0), m_y(0.0)
	{
		parse(json);
	}

	Object::~Object() noexcept
	{
		m_points.clear();
		m_properties.clear();
	}

	void Object::parse(const nlohmann::json& json) noexcept
	{
		if (json.count("ellipse") > 0)
		{
			m_ellipse = json.at("ellipse");
		}

		if (json.count("gid") > 0)
		{
			m_gid = json.at("gid");
		}

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

		if (json.count("point") > 0)
		{
			m_point = json.at("point");
		}
		
		if (json.count("polygon") > 0)
		{
			auto points = json.at("polygon");
			std::for_each(points.begin(), points.end(), [&](const nlohmann::json& point)
			{
				m_points.emplace_back(point);
			});
		}

		if (json.count("polyline") > 0)
		{
			auto points = json.at("polyline");
			std::for_each(points.begin(), points.end(), [&](const nlohmann::json& point)
			{
				m_points.emplace_back(point);
			});
		}

		if (json.count("properties") > 0)
		{
			auto propArray = json.at("properties");
			std::for_each(propArray.begin(), propArray.end(), [&](const nlohmann::json& property)
			{
				m_properties.emplace(property.at("name"), property);
			});
		}

		if (json.count("rotation") > 0)
		{
			m_rotation = json.at("rotation");
		}
		
		if (json.count("template") > 0)
		{
			m_template = json.at("template");
		}

		if (json.count("text") > 0)
		{
			m_text.parse(json.at("text"));
		}
		
		if (json.count("type") > 0)
		{
			m_type = json.at("type");
		}

		if (json.count("visible") > 0)
		{
			m_visible = json.at("visible");
		}

		if (json.count("width") > 0)
		{
			m_width = json.at("width");
		}

		if (json.count("x") > 0)
		{
			m_x = json.at("x");
		}

		if (json.count("y") > 0)
		{
			m_y = json.at("y");
		}
	}

	const bool Object::isEllipse() const noexcept
	{
		return m_ellipse;
	}

	const int Object::getGID() const noexcept
	{
		return m_gid;
	}

	const std::string& Object::getName() const noexcept
	{
		return m_name;
	}

	const bool Object::isPoint() const noexcept
	{
		return m_point;
	}

	const auto& Object::getPoints() const noexcept
	{
		return m_points;
	}

	const double Object::getRotation() const noexcept
	{
		return m_rotation;
	}

	const std::string& Object::getTemplate() const noexcept
	{
		return m_template;
	}

	const starmap::Text& Object::getText() const noexcept
	{
		return m_text;
	}

	const std::string& Object::getType() const noexcept
	{
		return m_type;
	}

	const bool Object::isVisible() const noexcept
	{
		return m_visible;
	}

	const double Object::getWidth() const noexcept
	{
		return m_width;
	}

	const double Object::getX() const noexcept
	{
		return m_x;
	}

	const double Object::getY() const noexcept
	{
		return m_y;
	}
}