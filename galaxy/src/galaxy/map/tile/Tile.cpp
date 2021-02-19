///
/// Tile.cpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#include <nlohmann/json.hpp>

#include "Tile.hpp"

namespace galaxy
{
	namespace map
	{
		Tile::Tile()
		    : m_id {0}, m_image {""}, m_image_height {0}, m_image_width {0}, m_object_group {std::nullopt}, m_probability {-1.0}, m_type {""}
		{
		}

		Tile::Tile(const nlohmann::json& json)
		    : m_id {0}, m_image {""}, m_image_height {0}, m_image_width {0}, m_object_group {std::nullopt}, m_probability {-1.0}, m_type {""}
		{
			parse(json);
		}

		Tile::~Tile()
		{
			m_animation.clear();
			m_properties.clear();
			m_terrain_indices.clear();
		}

		void Tile::parse(const nlohmann::json& json)
		{
			if (json.count("animation") > 0)
			{
				auto anim_array = json.at("animation");
				for (const auto& anim : anim_array)
				{
					m_animation.emplace_back(anim);
				}
			}

			if (json.count("id") > 0)
			{
				m_id = json.at("id");
			}

			if (json.count("image") > 0)
			{
				m_image = json.at("image");
			}

			if (json.count("imageheight") > 0)
			{
				m_image_height = json.at("imageheight");
			}

			if (json.count("imagewidth") > 0)
			{
				m_image_width = json.at("imagewidth");
			}

			if (json.count("objectgroup") > 0)
			{
				auto object_group = json.at("objectgroup");
				m_object_group.emplace(object_group);
			}
			else
			{
				m_object_group = std::nullopt;
			}

			if (json.count("probability") > 0)
			{
				m_probability = json.at("probability");
			}

			if (json.count("properties") > 0)
			{
				auto prop_array = json.at("properties");
				for (const auto& prop : prop_array)
				{
					m_properties.emplace(prop.at("name"), prop);
				}
			}

			if (json.count("terrain") > 0)
			{
				auto terrain_array = json.at("terrain");
				for (const auto& terrain : terrain_array)
				{
					m_terrain_indices.emplace_back(terrain);
				}
			}

			if (json.count("type") > 0)
			{
				m_type = json.at("type");
			}
		}

		const auto& Tile::get_animations() const
		{
			return m_animation;
		}

		const int Tile::get_id() const
		{
			return m_id;
		}

		std::string Tile::get_image() const
		{
			return m_image;
		}

		const int Tile::get_image_height() const
		{
			return m_image_height;
		}

		const int Tile::get_image_width() const
		{
			return m_image_width;
		}

		const auto& Tile::get_object_group() const
		{
			return m_object_group;
		}

		const double Tile::get_probability() const
		{
			return m_probability;
		}

		const auto& Tile::get_terrain_indices() const
		{
			return m_terrain_indices;
		}

		std::string Tile::get_type() const
		{
			return m_type;
		}
	} // namespace map
} // namespace galaxy