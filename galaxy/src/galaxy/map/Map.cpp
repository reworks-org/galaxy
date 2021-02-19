///
/// Map.cpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#include <filesystem>
#include <fstream>

#include "galaxy/error/Log.hpp"
#include "galaxy/map/layer/TileLayer.hpp"
#include "galaxy/map/layer/ObjectLayer.hpp"
#include "galaxy/map/layer/ImageLayer.hpp"
#include "galaxy/map/layer/GroupLayer.hpp"
#include "galaxy/scripting/JSONUtils.hpp"

#include "Map.hpp"

namespace galaxy
{
	namespace map
	{
		Map::Map() noexcept
		    : m_loaded {false}, m_root {}, m_bg_colour {"00FFFFFF"}, m_height {0}, m_hex_side_length {0}, m_infinite {false}, m_next_layer_id {0}, m_next_object_id {0}, m_orientation {"orthogonal"}, m_render_order {"right-down"}, m_stagger_axis {""}, m_stagger_index {""}, m_tiled_version {""}, m_tile_height {0}, m_tile_width {0}, m_type {"map"}, m_width {0}, m_compression_level {0}
		{
		}

		Map::~Map() noexcept
		{
			m_root.clear();
			m_properties.clear();
			m_layers.clear();
			m_tile_sets.clear();
		}

		const bool Map::load(std::string_view map)
		{
			const auto json = json::parse_from_disk(map);
			if (json == std::nullopt)
			{
				GALAXY_LOG(GALAXY_ERROR, "Failed to load/parse json tiled map: {0}.", map);
				m_loaded = false;
			}
			else
			{
				m_root   = json.value();
				m_loaded = true;
			}

			return m_loaded;
		}

		const bool Map::load(std::span<char> buffer)
		{
			const auto json = json::parse_from_mem(buffer);
			if (json == std::nullopt)
			{
				GALAXY_LOG(GALAXY_ERROR, "Failed to parse json from memory for tiled map.");
				m_loaded = false;
			}
			else
			{
				m_root   = json.value();
				m_loaded = true;
			}

			return m_loaded;
		}

		const bool Map::parse()
		{
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
					const auto& layer_array = m_root.at("layers");
					for (const auto& layer : layer_array)
					{
						std::string type = layer.at("type");
						if (type == "tilelayer")
						{
							m_layers.push_back(std::make_unique<TileLayer>(layer));
						}
						else if (type == "objectgroup")
						{
							m_layers.push_back(std::make_unique<ObjectLayer>(layer));
						}
						else if (type == "imagelayer ")
						{
							m_layers.push_back(std::make_unique<ImageLayer>(layer));
						}
						else if (type == "group")
						{
							m_layers.push_back(std::make_unique<GroupLayer>(layer));
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
					const auto& property_array = m_root.at("properties");
					for (auto& prop : property_array)
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
					const auto& tileset_array = m_root.at("tilesets");
					for (auto& tileset : tileset_array)
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
				GALAXY_LOG(GALAXY_WARNING, "Tried to parse map that was not loaded.");
				return false;
			}

			return true;
		}

		const std::string& Map::get_bg_colour() const noexcept
		{
			return m_bg_colour;
		}

		const int Map::get_height() const noexcept
		{
			return m_height;
		}

		const int Map::get_hex_side_length() const noexcept
		{
			return m_hex_side_length;
		}

		const bool Map::is_infinite() const noexcept
		{
			return m_infinite;
		}

		std::vector<std::unique_ptr<Layer>>& Map::get_layers() noexcept
		{
			return m_layers;
		}

		const int Map::get_next_layer_id() const noexcept
		{
			return m_next_layer_id;
		}

		const int Map::get_next_object_id() const noexcept
		{
			return m_next_object_id;
		}

		const std::string& Map::get_orientation() const noexcept
		{
			return m_orientation;
		}

		const std::string& Map::get_render_order() const noexcept
		{
			return m_render_order;
		}

		const std::string& Map::get_stagger_axis() const noexcept
		{
			return m_stagger_axis;
		}

		const std::string& Map::get_stagger_index() const noexcept
		{
			return m_stagger_index;
		}

		const std::string& Map::get_tiled_version() const noexcept
		{
			return m_tiled_version;
		}

		const int Map::get_tile_height() const noexcept
		{
			return m_tile_height;
		}

		const auto& Map::get_tile_sets() const noexcept
		{
			return m_tile_sets;
		}

		const int Map::get_tile_width() const noexcept
		{
			return m_tile_width;
		}

		const std::string& Map::get_type() const noexcept
		{
			return m_type;
		}

		const int Map::get_width() const noexcept
		{
			return m_width;
		}

		const int Map::get_compression_level() const noexcept
		{
			return m_compression_level;
		}

		nlohmann::json& Map::get_raw() noexcept
		{
			return m_root;
		}
	} // namespace map
} // namespace galaxy