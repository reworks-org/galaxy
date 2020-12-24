///
/// Map.cpp
/// starmap
///
/// Refer to LICENSE.txt for more details.
///

#include <fstream>
#include <filesystem>

#include <pulsar/Log.hpp>

#include "starmap/layer/TileLayer.hpp"
#include "starmap/layer/ObjectLayer.hpp"
#include "starmap/layer/ImageLayer.hpp"
#include "starmap/layer/GroupLayer.hpp"

#include "Map.hpp"

///
/// Core namespace.
///
namespace starmap
{
	Map::Map()
	    : m_loaded {false}, m_root {}, m_bg_colour {"00FFFFFF"}, m_height {0}, m_hex_side_length {0}, m_infinite {false}, m_next_layer_id {0}, m_next_object_id {0}, m_orientation {"orthogonal"}, m_render_order {"right-down"}, m_stagger_axis {""}, m_stagger_index {""}, m_tiled_version {""}, m_tile_height {0}, m_tile_width {0}, m_type {"map"}, m_width {0}, m_compression_level {0}
	{
	}

	Map::~Map()
	{
		m_root.clear();
		m_properties.clear();
		m_layers.clear();
		m_tile_sets.clear();
	}

	bool Map::load(std::string_view map)
	{
		bool result = true;

		// Makes sure the filepath is correct for the current platform.
		auto path = std::filesystem::path {map};
		std::ifstream input;
		input.open(path.string(), std::ifstream::in);

		if (!input.good())
		{
			PL_LOG(PL_ERROR, "Failed to open: {0}.", path.string());
			result = false;
		}
		else
		{
			// Use JSON stream to deserialize data and parse.
			input >> m_root;
			m_loaded = true;
		}

		input.close();
		return result;
	}

	bool Map::load(std::span<char> buffer)
	{
		bool result = true;

		if (buffer.empty())
		{
			PL_LOG(PL_ERROR, "Passed empty buffer to Map::load()");
			result = false;
		}
		else
		{
			// This is the string parser.
			m_root   = nlohmann::json::parse(buffer);
			m_loaded = true;
		}

		return result;
	}

	bool Map::move(nlohmann::json&& json)
	{
		m_root   = std::move(json);
		m_loaded = true;

		return true;
	}

	bool Map::parse()
	{
		bool result = true;

		// Make sure json is loaded to avoid error.
		if (m_loaded)
		{
			// Optional attribute.
			if (m_root.count("backgroundcolor") > 0)
			{
				m_bg_colour = m_root.at("backgroundcolor");
			}

			if (m_root.count("height") > 0)
			{
				m_height = m_root.at("height");
			}

			// Only present on hexagonal maps.
			if (m_root.count("hexsidelength") > 0)
			{
				m_hex_side_length = m_root.at("hexsidelength");
			}

			if (m_root.count("infinite") > 0)
			{
				m_infinite = m_root.at("infinite");
			}

			if (m_root.count("compressionlevel") > 0)
			{
				m_compression_level = m_root.at("compressionlevel");
			}

			if (m_root.count("layers") > 0)
			{
				auto layer_array = m_root.at("layers");
				for (const auto& layer : layer_array)
				{
					std::string type = layer.at("type");
					if (type == "tilelayer")
					{
						m_layers.push_back(std::make_unique<starmap::TileLayer>(layer));
					}
					else if (type == "objectgroup")
					{
						m_layers.push_back(std::make_unique<starmap::ObjectLayer>(layer));
					}
					else if (type == "imagelayer ")
					{
						m_layers.push_back(std::make_unique<starmap::ImageLayer>(layer));
					}
					else if (type == "group")
					{
						m_layers.push_back(std::make_unique<starmap::GroupLayer>(layer));
					}
				}
			}

			if (m_root.count("nextlayerid") > 0)
			{
				m_next_layer_id = m_root.at("nextlayerid");
			}

			if (m_root.count("nextobjectid") > 0)
			{
				m_next_object_id = m_root.at("nextobjectid");
			}

			if (m_root.count("orientation") > 0)
			{
				m_orientation = m_root.at("orientation");
			}

			if (m_root.count("properties") > 0)
			{
				auto property_array = m_root.at("properties");
				for (const auto& prop : property_array)
				{
					m_properties.emplace(prop.at("name"), prop);
				}
			}

			if (m_root.count("renderorder") > 0)
			{
				m_render_order = m_root.at("renderorder");
			}

			if (m_root.count("staggeraxis") > 0)
			{
				m_stagger_axis = m_root.at("staggeraxis");
			}

			if (m_root.count("staggerindex") > 0)
			{
				m_stagger_index = m_root.at("staggerindex");
			}

			if (m_root.count("tiledversion") > 0)
			{
				m_tiled_version = m_root.at("tiledversion");
			}

			if (m_root.count("tileheight") > 0)
			{
				m_tile_height = m_root.at("tileheight");
			}

			if (m_root.count("tilesets") > 0)
			{
				auto tileset_array = m_root.at("tilesets");
				for (const auto& tileset : tileset_array)
				{
					m_tile_sets.emplace_back(tileset);
				}
			}

			if (m_root.count("tilewidth") > 0)
			{
				m_tile_width = m_root.at("tilewidth");
			}

			if (m_root.count("type") > 0)
			{
				m_type = m_root.at("type");
			}

			if (m_root.count("width") > 0)
			{
				m_width = m_root.at("width");
			}
		}
		else
		{
			PL_LOG(PL_WARNING, "Tried to parse map that was not loaded.");
			result = false;
		}

		return result;
	}

	std::string Map::get_bg_colour() const
	{
		return m_bg_colour;
	}

	const int Map::get_height() const
	{
		return m_height;
	}

	const int Map::get_hex_side_length() const
	{
		return m_hex_side_length;
	}

	const bool Map::is_infinite() const
	{
		return m_infinite;
	}

	const auto& Map::get_layers() const
	{
		return m_layers;
	}

	const int Map::get_next_layer_id() const
	{
		return m_next_layer_id;
	}

	const int Map::get_next_object_id() const
	{
		return m_next_object_id;
	}

	std::string Map::get_orientation() const
	{
		return m_orientation;
	}

	std::string Map::get_render_order() const
	{
		return m_render_order;
	}

	std::string Map::get_stagger_axis() const
	{
		return m_stagger_axis;
	}

	std::string Map::get_stagger_index() const
	{
		return m_stagger_index;
	}

	std::string Map::get_tiled_version() const
	{
		return m_tiled_version;
	}

	const int Map::get_tile_height() const
	{
		return m_tile_height;
	}

	const auto& Map::get_tile_sets() const
	{
		return m_tile_sets;
	}

	const int Map::get_tile_width() const
	{
		return m_tile_width;
	}

	std::string Map::get_type() const
	{
		return m_type;
	}

	const int Map::get_width() const
	{
		return m_width;
	}

	const int Map::get_compression_level() const
	{
		return m_compression_level;
	}

	nlohmann::json& Map::raw_json()
	{
		return m_root;
	}
} // namespace starmap