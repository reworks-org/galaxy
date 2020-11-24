///
/// Tileset.cpp
/// starmap
///
/// Refer to LICENSE.txt for more details.
///

#include <filesystem>
#include <fstream>

#include <nlohmann/json.hpp>
#include <pulsar/Log.hpp>

#include "Tileset.hpp"

///
/// Core namespace.
///
namespace starmap
{
	Tileset::Tileset()
	    : m_bg_colour {"00FFFFFF"}, m_columns {0}, m_first_gid {0}, m_grid {std::nullopt}, m_image {""}, m_image_height {0}, m_image_width {0}, m_margin {0}, m_name {""}, m_object_alignment {""}, m_source {""}, m_spacing {0}, m_tile_count {0}, m_tiled_version {""}, m_tile_height {0}, m_tile_offset {std::nullopt}, m_tile_width {0}, m_transparent_colour {"FFFFFF"}, m_type {"tileset"}
	{
	}

	Tileset::Tileset(const nlohmann::json& json)
	    : m_bg_colour {"00FFFFFF"}, m_columns {0}, m_first_gid {0}, m_grid {std::nullopt}, m_image {""}, m_image_height {0}, m_image_width {0}, m_margin {0}, m_name {""}, m_object_alignment {""}, m_source {""}, m_spacing {0}, m_tile_count {0}, m_tiled_version {""}, m_tile_height {0}, m_tile_offset {std::nullopt}, m_tile_width {0}, m_transparent_colour {"FFFFFF"}, m_type {"tileset"}
	{
		parse(json);
	}

	Tileset::~Tileset()
	{
		m_grid.reset();
		m_properties.clear();
		m_terrain.clear();
		m_tiles.clear();
		m_wang_sets.clear();
	}

	void Tileset::parse(const nlohmann::json& json)
	{
		if (json.count("firstgid") > 0)
		{
			m_first_gid = json.at("firstgid");
		}

		if (json.count("source") > 0)
		{
			m_source = json.at("source");
		}

		nlohmann::json src_json;
		if (!m_source.empty())
		{
			// Makes sure the filepath is correct for the current platform.
			auto path = std::filesystem::path {m_source};
			std::ifstream input;
			input.open(path.string(), std::ifstream::in);

			if (!input.good())
			{
				input.close();
				PL_LOG(PL_FATAL, "Failed to open: {0}.", path.string());
			}
			else
			{
				// Use JSON stream to deserialize data and parse.
				input >> src_json;
				input.close();
			}
		}
		else
		{
			src_json = json;
		}

		if (src_json.count("backgroundcolor") > 0)
		{
			m_bg_colour = src_json.at("backgroundcolor");
		}

		if (src_json.count("columns") > 0)
		{
			m_columns = src_json.at("columns");
		}

		if (src_json.count("grid") > 0)
		{
			auto grid_array = src_json.at("grid");
			m_grid.emplace(grid_array);
		}
		else
		{
			m_grid = std::nullopt;
		}

		if (src_json.count("image") > 0)
		{
			m_image = src_json.at("image");
		}

		if (src_json.count("imageheight") > 0)
		{
			m_image_height = src_json.at("imageheight");
		}

		if (src_json.count("imagewidth") > 0)
		{
			m_image_width = src_json.at("imagewidth");
		}

		if (src_json.count("margin") > 0)
		{
			m_margin = src_json.at("margin");
		}

		if (src_json.count("name") > 0)
		{
			m_name = src_json.at("name");
		}

		if (src_json.count("objectalignment") > 0)
		{
			m_object_alignment = src_json.at("objectalignment");
		}

		if (src_json.count("properties") > 0)
		{
			auto prop_array = src_json.at("properties");
			for (const auto& prop : prop_array)
			{
				m_properties.emplace(prop.at("name"), prop);
			}
		}

		if (src_json.count("spacing") > 0)
		{
			m_spacing = src_json.at("spacing");
		}

		if (src_json.count("terrains") > 0)
		{
			auto terrain_array = src_json.at("terrains");
			for (const auto& terrain : terrain_array)
			{
				m_terrain.emplace_back(terrain);
			}
		}

		if (src_json.count("tilecount") > 0)
		{
			m_tile_count = src_json.at("tilecount");
		}

		if (src_json.count("tiledversion") > 0)
		{
			m_tiled_version = src_json.at("tiledversion");
		}

		if (src_json.count("tileheight") > 0)
		{
			m_tile_height = src_json.at("tileheight");
		}

		if (src_json.count("tileoffset") > 0)
		{
			auto tile_offset = src_json.at("tileoffset");
			m_tile_offset.emplace(tile_offset);
		}
		else
		{
			m_grid = std::nullopt;
		}

		if (src_json.count("tiles") > 0)
		{
			auto tile_array = src_json.at("tiles");
			for (const auto& tile : tile_array)
			{
				m_tiles.emplace_back(tile);
			}
		}

		if (src_json.count("tilewidth") > 0)
		{
			m_tile_width = src_json.at("tilewidth");
		}

		if (src_json.count("transparentcolor") > 0)
		{
			m_transparent_colour = src_json.at("transparentcolor");
		}

		if (src_json.count("type") > 0)
		{
			m_type = src_json.at("type");
		}

		if (src_json.count("wangsets") > 0)
		{
			auto wang_sets = src_json.at("wangsets");
			for (const auto& set : wang_sets)
			{
				m_wang_sets.emplace_back(set);
			}
		}
	}

	std::string Tileset::get_bg_colour() const
	{
		return m_bg_colour;
	}

	const int Tileset::get_columns() const
	{
		return m_columns;
	}

	const int Tileset::get_first_gid() const
	{
		return m_first_gid;
	}

	const auto& Tileset::get_grid() const
	{
		return m_grid;
	}

	std::string Tileset::get_image() const
	{
		return m_image;
	}

	const int Tileset::get_image_height() const
	{
		return m_image_height;
	}

	const int Tileset::get_image_width() const
	{
		return m_image_width;
	}

	const int Tileset::get_margin() const
	{
		return m_margin;
	}

	std::string Tileset::get_name() const
	{
		return m_name;
	}

	std::string Tileset::get_object_alignment() const
	{
		return m_object_alignment;
	}

	const auto& Tileset::get_terrain() const
	{
		return m_terrain;
	}

	const int Tileset::get_tile_count() const
	{
		return m_tile_count;
	}

	std::string Tileset::get_tiled_version() const
	{
		return m_tiled_version;
	}

	const int Tileset::get_tile_height() const
	{
		return m_tile_height;
	}

	const auto& Tileset::get_tile_offset() const
	{
		return m_tile_offset;
	}

	const auto& Tileset::get_tiles() const
	{
		return m_tiles;
	}

	const int Tileset::get_tile_width() const
	{
		return m_tile_width;
	}

	std::string Tileset::get_transparent_colour() const
	{
		return m_transparent_colour;
	}

	std::string Tileset::get_type() const
	{
		return m_type;
	}

	const auto& Tileset::get_wang_sets() const
	{
		return m_wang_sets;
	}
} // namespace starmap