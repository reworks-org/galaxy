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
		Object::Object() noexcept
			: m_type_enum {Type::RECT}
			, m_gid {0}
			, m_height {0.0}
			, m_id {0}
			, m_name {""}
			, m_rotation {0.0}
			, m_template {""}
			, m_type {""}
			, m_visible {true}
			, m_width {0.0}
			, m_x {0.0}
			, m_y {0.0}
		{
		}

		Object::Object(const nlohmann::json& json)
			: m_type_enum {Type::RECT}
			, m_gid {0}
			, m_height {0.0}
			, m_id {0}
			, m_name {""}
			, m_rotation {0.0}
			, m_template {""}
			, m_type {""}
			, m_visible {true}
			, m_width {0.0}
			, m_x {0.0}
			, m_y {0.0}
		{
			parse(json);
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
				const bool is_ellipse = json.at("ellipse");
				if (is_ellipse)
				{
					m_type_enum = Type::ELLIPSE;
				}
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
				const bool is_point = json.at("point");
				if (is_point)
				{
					m_type_enum = Type::POINT;
				}
			}

			if (json.count("polygon") > 0)
			{
				const auto& point_array = json.at("polygon");
				for (const auto& point : point_array)
				{
					m_points.emplace_back(point);
				}

				if (!m_points.empty())
				{
					m_type_enum = Type::POLYGON;
				}
			}

			if (json.count("polyline") > 0)
			{
				const auto& point_array = json.at("polyline");
				for (const auto& point : point_array)
				{
					m_points.emplace_back(point);
				}

				if (!m_points.empty())
				{
					m_type_enum = Type::POLYLINE;
				}
			}

			if (json.count("properties") > 0)
			{
				const auto& prop_array = json.at("properties");
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
				m_type_enum = Type::TEXT;
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

		const Object::Type Object::get_type_enum() const noexcept
		{
			return m_type_enum;
		}

		const int Object::get_gid() const noexcept
		{
			return m_gid;
		}

		const double Object::get_height() const noexcept
		{
			return m_height;
		}

		const int Object::get_id() const noexcept
		{
			return m_id;
		}

		const std::string& Object::get_name() const noexcept
		{
			return m_name;
		}

		const std::vector<Point>& Object::get_points() const noexcept
		{
			return m_points;
		}

		const double Object::get_rotation() const noexcept
		{
			return m_rotation;
		}

		const std::string& Object::get_template() const noexcept
		{
			return m_template;
		}

		const Text& Object::get_text() const noexcept
		{
			return m_text;
		}

		const std::string& Object::get_type() const noexcept
		{
			return m_type;
		}

		const bool Object::is_visible() const noexcept
		{
			return m_visible;
		}

		const double Object::get_width() const noexcept
		{
			return m_width;
		}

		const double Object::get_x() const noexcept
		{
			return m_x;
		}

		const double Object::get_y() const noexcept
		{
			return m_y;
		}
	} // namespace map
} // namespace galaxy