///
/// Object.cpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#include <nlohmann/json.hpp>

#include "Object.hpp"

namespace galaxy
{
	namespace map
	{
		Object::Object()
		    : m_ellipse {false}, m_gid {0}, m_height {0.0}, m_id {0}, m_name {""}, m_point {false}, m_rotation {0.0}, m_template {""}, m_type {""}, m_visible {true}, m_width {0.0}, m_x {0.0}, m_y {0.0}
		{
		}

		Object::Object(const nlohmann::json& json)
		    : m_ellipse {false}, m_gid {0}, m_height {0.0}, m_id {0}, m_name {""}, m_point {false}, m_rotation {0.0}, m_template {""}, m_type {""}, m_visible {true}, m_width {0.0}, m_x {0.0}, m_y {0.0}
		{
			parse(json);
		}

		Object::~Object()
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
				auto point_array = json.at("polygon");
				for (const auto& point : point_array)
				{
					m_points.emplace_back(point);
				}
			}

			if (json.count("polyline") > 0)
			{
				auto point_array = json.at("polyline");
				for (const auto& point : point_array)
				{
					m_points.emplace_back(point);
				}
			}

			if (json.count("properties") > 0)
			{
				auto prop_array = json.at("properties");
				for (const auto& prop : prop_array)
				{
					m_properties.emplace(prop.at("name"), prop);
				}
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

		const bool Object::is_ellipse() const
		{
			return m_ellipse;
		}

		const int Object::get_gid() const
		{
			return m_gid;
		}

		std::string Object::get_name() const
		{
			return m_name;
		}

		const bool Object::is_point() const
		{
			return m_point;
		}

		const auto& Object::get_points() const
		{
			return m_points;
		}

		const double Object::get_rotation() const
		{
			return m_rotation;
		}

		std::string Object::get_template() const
		{
			return m_template;
		}

		const galaxy::Text& Object::get_text() const
		{
			return m_text;
		}

		std::string Object::get_type() const
		{
			return m_type;
		}

		const bool Object::is_visible() const
		{
			return m_visible;
		}

		const double Object::get_width() const
		{
			return m_width;
		}

		const double Object::get_x() const
		{
			return m_x;
		}

		const double Object::get_y() const
		{
			return m_y;
		}
	} // namespace map
} // namespace galaxy