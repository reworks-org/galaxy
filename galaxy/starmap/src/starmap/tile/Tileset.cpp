///
/// Tileset.cpp
/// starmap
///
/// Refer to LICENSE.txt for more details.
///

#include <nlohmann/json.hpp>

#include "Tileset.hpp"

///
/// Core namespace.
///
namespace starmap
{
	Tileset::Tileset() noexcept
	    : m_bg_colour {"00FFFFFF"}, m_columns {0}, m_first_gid {0}, m_grid {std::nullopt}, m_image {""}, m_image_height {0}, m_image_width {0}, m_margin {0}, m_name {""}, m_object_alignment {""}, m_source {""}, m_spacing {0}, m_tile_count {0}, m_tiled_version {""}, m_tile_height {0}, m_tile_offset {std::nullopt}, m_tile_width {0}, m_transparent_colour {"FFFFFF"}, m_type {"tileset"}
	{
	}

	Tileset::Tileset(const nlohmann::json& json)
	    : m_bg_colour {"00FFFFFF"}, m_columns {0}, m_first_gid {0}, m_grid {std::nullopt}, m_image {""}, m_image_height {0}, m_image_width {0}, m_margin {0}, m_name {""}, m_object_alignment {""}, m_source {""}, m_spacing {0}, m_tile_count {0}, m_tiled_version {""}, m_tile_height {0}, m_tile_offset {std::nullopt}, m_tile_width {0}, m_transparent_colour {"FFFFFF"}, m_type {"tileset"}
	{
		parse(json);
	}

	Tileset::~Tileset() noexcept
	{
		m_grid.reset();
		m_properties.clear();
		m_terrain.clear();
		m_tiles.clear();
		m_wang_sets.clear();
	}

	void Tileset::parse(const nlohmann::json& json)
	{
		if (json.count("backgroundcolor") > 0)
		{
			m_bg_colour = json.at("backgroundcolor");
		}

		if (json.count("columns") > 0)
		{
			m_columns = json.at("columns");
		}

		if (json.count("firstgid") > 0)
		{
			m_first_gid = json.at("firstgid");
		}

		if (json.count("grid") > 0)
		{
			auto grid_array = json.at("grid");
			m_grid.emplace(grid_array);
		}
		else
		{
			m_grid = std::nullopt;
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

		if (json.count("margin") > 0)
		{
			m_margin = json.at("margin");
		}

		if (json.count("name") > 0)
		{
			m_name = json.at("name");
		}

		if (json.count("objectalignment") > 0)
		{
			m_object_alignment = json.at("objectalignment");
		}

		if (json.count("properties") > 0)
		{
			auto prop_array = json.at("properties");
			for (const auto& prop : prop_array)
			{
				m_properties.emplace(prop.at("name"), prop);
			}
		}

		if (json.count("source") > 0)
		{
			m_source = json.at("source");
		}

		if (json.count("spacing") > 0)
		{
			m_spacing = json.at("spacing");
		}

		if (json.count("terrains") > 0)
		{
			auto terrain_array = json.at("terrains");
			for (const auto& terrain : terrain_array)
			{
				m_terrain.emplace_back(terrain);
			}
		}

		if (json.count("tilecount") > 0)
		{
			m_tile_count = json.at("tilecount");
		}

		if (json.count("tiledversion") > 0)
		{
			m_tiled_version = json.at("tiledversion");
		}

		if (json.count("tileheight") > 0)
		{
			m_tile_height = json.at("tileheight");
		}

		if (json.count("tileoffset") > 0)
		{
			auto tile_offset = json.at("tileoffset");
			m_tile_offset.emplace(tile_offset);
		}
		else
		{
			m_grid = std::nullopt;
		}

		if (json.count("tiles") > 0)
		{
			auto tile_array = json.at("tiles");
			for (const auto& tile : tile_array)
			{
				m_tiles.emplace_back(tile);
			}
		}

		if (json.count("tilewidth") > 0)
		{
			m_tile_width = json.at("tilewidth");
		}

		if (json.count("transparentcolor") > 0)
		{
			m_transparent_colour = json.at("transparentcolor");
		}

		if (json.count("type") > 0)
		{
			m_type = json.at("type");
		}

		if (json.count("wangsets") > 0)
		{
			auto wang_sets = json.at("wangsets");
			for (const auto& set : wang_sets)
			{
				m_wang_sets.emplace_back(set);
			}
		}
	}

	std::string Tileset::get_bg_colour() const noexcept
	{
		return m_bg_colour;
	}

	const int Tileset::get_columns() const noexcept
	{
		return m_columns;
	}

	const int Tileset::get_first_gid() const noexcept
	{
		return m_first_gid;
	}

	const auto& Tileset::get_grid() const noexcept
	{
		return m_grid;
	}

	std::string Tileset::get_image() const noexcept
	{
		return m_image;
	}

	const int Tileset::get_image_height() const noexcept
	{
		return m_image_height;
	}

	const int Tileset::get_image_width() const noexcept
	{
		return m_image_width;
	}

	const int Tileset::get_margin() const noexcept
	{
		return m_margin;
	}

	std::string Tileset::get_name() const noexcept
	{
		return m_name;
	}

	std::string Tileset::get_object_alignment() const noexcept
	{
		return m_object_alignment;
	}

	const auto& Tileset::get_terrain() const noexcept
	{
		return m_terrain;
	}

	const int Tileset::get_tile_count() const noexcept
	{
		return m_tile_count;
	}

	std::string Tileset::get_tiled_version() const noexcept
	{
		return m_tiled_version;
	}

	const int Tileset::get_tile_height() const noexcept
	{
		return m_tile_height;
	}

	const auto& Tileset::get_tile_offset() const noexcept
	{
		return m_tile_offset;
	}

	const auto& Tileset::get_tiles() const noexcept
	{
		return m_tiles;
	}

	const int Tileset::get_tile_width() const noexcept
	{
		return m_tile_width;
	}

	std::string Tileset::get_transparent_colour() const noexcept
	{
		return m_transparent_colour;
	}

	std::string Tileset::get_type() const noexcept
	{
		return m_type;
	}

	const auto& Tileset::get_wang_sets() const noexcept
	{
		return m_wang_sets;
	}
} // namespace starmap