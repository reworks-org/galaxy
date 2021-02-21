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
		Tile::Tile() noexcept
		    : m_id {0}, m_image {""}, m_image_height {0}, m_image_width {0}, m_object_group {std::nullopt}, m_probability {-1.0}, m_type {""}
		{
		}

		Tile::Tile(const nlohmann::json& json)
		    : m_id {0}, m_image {""}, m_image_height {0}, m_image_width {0}, m_object_group {std::nullopt}, m_probability {-1.0}, m_type {""}
		{
			parse(json);
		}

		Tile::~Tile() noexcept
		{
			m_animation.clear();
			m_properties.clear();
			m_terrain_indices.clear();
		}

		void Tile::parse(const nlohmann::json& json)
		{
			if (json.count("animation") > 0)
			{
				const auto& anim_array = json.at("animation");
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
				const auto& object_group = json.at("objectgroup");
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
				const auto& prop_array = json.at("properties");
				for (const auto& prop : prop_array)
				{
					m_properties.emplace(prop.at("name"), prop);
				}
			}

			if (json.count("terrain") > 0)
			{
				const auto& terrain_array = json.at("terrain");
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

		const std::vector<Frame>& Tile::get_animations() const noexcept
		{
			return m_animation;
		}

		const int Tile::get_id() const noexcept
		{
			return m_id;
		}

		const std::string& Tile::get_image() const noexcept
		{
			return m_image;
		}

		const int Tile::get_image_height() const noexcept
		{
			return m_image_height;
		}

		const int Tile::get_image_width() const noexcept
		{
			return m_image_width;
		}

		const std::optional<ObjectLayer>& Tile::get_object_group() const noexcept
		{
			return m_object_group;
		}

		const double Tile::get_probability() const noexcept
		{
			return m_probability;
		}

		const std::vector<int>& Tile::get_terrain_indices() const noexcept
		{
			return m_terrain_indices;
		}

		const std::string& Tile::get_type() const noexcept
		{
			return m_type;
		}
	} // namespace map
} // namespace galaxy