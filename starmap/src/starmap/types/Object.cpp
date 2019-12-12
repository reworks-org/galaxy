///
/// Object.cpp
/// starmap
///
/// Refer to LICENSE.txt for more details.
///

#include "nlohmann/json.hpp"

#include "Object.hpp"

///
/// Core namespace.
///
namespace starmap
{
	Object::Object() noexcept
		:m_ellipse(false), m_gid(0), m_height(0.0), m_id(0), m_name(""), m_point(false), m_rotation(0.0), m_template("")
	{
	}

	Object::~Object() noexcept
	{
		m_points.clear();
		m_properties.clear();
	}

	void Object::parse(const nlohmann::json& json)
	{
		if (json.count("ellipse") > 0)
		{
			m_ellipse = json.at("ellipse");
		}

		m_gid = json.at("gid");
		m_height = json.at("height");
		m_id = json.at("id");

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
				m_points.emplace_back(point.at("x"), point.at("y"));
			});
		}

		if (json.count("polyline") > 0)
		{
			auto points = json.at("polyline");
			std::for_each(points.begin(), points.end(), [&](const nlohmann::json& point)
			{
				m_points.emplace_back(point.at("x"), point.at("y"));
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

		m_rotation = json.at("rotation");
		
		if (json.count("template") > 0)
		{
			m_template = json.at("template");
		}

		if (json.count("text") > 0)
		{

		}
	}

	const bool Object::getEllipse() const noexcept
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

	const bool Object::getPoint() const noexcept
	{
		return m_point;
	}

	const auto& Object::getPoints() const noexcept
	{
		return m_points;
	}
}