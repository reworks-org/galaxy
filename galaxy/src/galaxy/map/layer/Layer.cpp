///
/// Layer.cpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#include <nlohmann/json.hpp>

#include "Layer.hpp"

namespace galaxy
{
	namespace map
	{
		Layer::~Layer() noexcept
		{
			m_properties.clear();
		}

		const int Layer::get_height() const noexcept
		{
			return m_height;
		}

		const int Layer::get_id() const noexcept
		{
			return m_id;
		}

		const std::string& Layer::get_name() const noexcept
		{
			return m_name;
		}

		const double Layer::get_offset_x() const noexcept
		{
			return m_offset_x;
		}

		const double Layer::get_offset_y() const noexcept
		{
			return m_offset_y;
		}

		const double Layer::get_opacity() const noexcept
		{
			return m_opacity;
		}

		const int Layer::get_start_x() const noexcept
		{
			return m_start_x;
		}

		const int Layer::get_start_y() const noexcept
		{
			return m_start_y;
		}

		const std::string& Layer::get_tint_colour() const noexcept
		{
			return m_tint_colour;
		}

		const std::string& Layer::get_type() const noexcept
		{
			return m_type;
		}

		const bool Layer::is_visible() const noexcept
		{
			return m_visible;
		}

		const int Layer::get_width() const noexcept
		{
			return m_width;
		}

		const int Layer::get_x() const noexcept
		{
			return m_x;
		}

		const int Layer::get_y() const noexcept
		{
			return m_y;
		}

		Layer::Layer() noexcept
		    : m_height {0}, m_id {0}, m_name {""}, m_offset_x {0.0}, m_offset_y {0.0}, m_opacity {0.0}, m_start_x {0}, m_start_y {0}, m_tint_colour {""}, m_type {""}, m_visible {true}, m_width {0}, m_x {0}, m_y {0}
		{
		}

		Layer::Layer(const nlohmann::json& json)
		    : m_height {0}, m_id {0}, m_name {""}, m_offset_x {0.0}, m_offset_y {0.0}, m_opacity {0.0}, m_start_x {0}, m_start_y {0}, m_tint_colour {""}, m_type {""}, m_visible {true}, m_width {0}, m_x {0}, m_y {0}
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
				m_offset_x = json.at("offsetx");
			}

			if (json.count("offsety") > 0)
			{
				m_offset_y = json.at("offsety");
			}

			if (json.count("opacity") > 0)
			{
				m_opacity = json.at("opacity");
			}

			if (json.count("properties") > 0)
			{
				const auto& prop_array = json.at("properties");
				for (const auto& prop : prop_array)
				{
					m_properties.emplace(prop.at("name"), prop);
				}
			}

			if (json.count("startx") > 0)
			{
				m_start_x = json.at("startx");
			}

			if (json.count("starty") > 0)
			{
				m_start_y = json.at("starty");
			}

			if (json.count("tintcolor") > 0)
			{
				m_tint_colour = json.at("tintcolor");
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
	} // namespace map
} // namespace galaxy