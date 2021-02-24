///
/// Map.cpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#include <filesystem>
#include <fstream>

#include "galaxy/components/Circle.hpp"
#include "galaxy/components/Line.hpp"
#include "galaxy/components/Point.hpp"
#include "galaxy/components/Polygon.hpp"
#include "galaxy/components/ShaderID.hpp"
#include "galaxy/components/Sprite.hpp"
#include "galaxy/components/Renderable.hpp"
#include "galaxy/components/Tag.hpp"
#include "galaxy/components/Transform.hpp"

#include "galaxy/core/World.hpp"
#include "galaxy/scripting/JSONUtils.hpp"

#include "Map.hpp"

namespace galaxy
{
	namespace map
	{
		Map::Map() noexcept
		    : m_loaded {false}, m_root {}, m_bg_colour {255, 255, 255, 255}, m_compression_level {-1}, m_height {0}, m_hex_side_length {0}, m_infinite {false}, m_next_layer_id {0}, m_next_object_id {0}, m_orientation {"orthogonal"}, m_render_order {"right-down"}, m_stagger_axis {""}, m_stagger_index {""}, m_tiled_version {""}, m_tile_height {0}, m_tile_width {0}, m_type {"map"}, m_version {0.0}, m_width {0}
		{
		}

		Map::~Map() noexcept
		{
			m_root.clear();
			m_properties.clear();
			m_tile_layers.clear();
			m_object_layers.clear();
			m_image_layers.clear();
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

		const bool Map::load_mem(std::span<char> buffer)
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
				if (m_root.count("backgroundcolor") > 0)
				{
					m_bg_colour = map::parse_hex_colour(m_root.at("backgroundcolor"));
				}

				if (m_root.count("compressionlevel") > 0)
				{
					m_compression_level = m_root.at("compressionlevel");
				}

				if (m_root.count("height") > 0)
				{
					m_height = m_root.at("height");
				}

				if (m_root.count("hexsidelength") > 0)
				{
					m_hex_side_length = m_root.at("hexsidelength");
				}

				if (m_root.count("infinite") > 0)
				{
					m_infinite = m_root.at("infinite");
				}

				parse_layers(m_root, 0);

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
					const auto& tileset_array = m_root.at("tilesets");
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

				if (m_root.count("version") > 0)
				{
					m_version = m_root.at("version");
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

		void Map::generate_image_entities(core::World& world)
		{
			for (const auto& image_layer : m_image_layers)
			{
				const auto entity = world.create();

				auto* renderable      = world.create_component<components::Renderable>(entity);
				renderable->m_type    = graphics::Renderables::SPRITE;
				renderable->m_z_level = image_layer.get_z_level();

				auto* shader        = world.create_component<components::ShaderID>(entity);
				shader->m_shader_id = "sprite";

				auto* sprite = world.create_component<components::Sprite>(entity);
				sprite->load(image_layer.get_image());
				sprite->create();

				auto* transform = world.create_component<components::Transform>(entity);
				//transform->set_pos(image_layer.get_x(), image_layer.get_y());
				transform->set_rotation_origin(image_layer.get_width() / 2.0, image_layer.get_height() / 2.0);

				if (!image_layer.get_name().empty())
				{
					auto* tag  = world.create_component<components::Tag>(entity);
					tag->m_tag = image_layer.get_name();
				}

				if (image_layer.is_visible())
				{
					world.enable(entity);
				}
			}
		}

		void Map::generate_object_entities(core::World& world)
		{
			for (const auto& object_layer : m_object_layers)
			{
				const auto& objects = object_layer.get_objects();
				for (const auto& object : objects)
				{
					const auto entity = world.create();

					auto* tag  = world.create_component<components::Tag>(entity);
					tag->m_tag = object.get_name();

					auto* transform  = world.create_component<components::Transform>(entity);
					auto* renderable = world.create_component<components::Renderable>(entity);
					auto* shaderid   = world.create_component<components::ShaderID>(entity);

					transform->set_pos(object.get_x(), object.get_y());

					const auto type = object.get_type_enum();
					switch (type)
					{
						case Object::Type::ELLIPSE:
							break;

						case Object::Type::POINT:
							break;

						case Object::Type::POLYGON:
							break;

						case Object::Type::POLYLINE:
							break;

						case Object::Type::TEXT:
							break;
					}

					/*
					if (object.is_ellipse())
					{
						auto* circle = world.create_component<components::Circle>(entity);

						const auto colour = map::convert_colour(object_layer.get_tint_colour());
						circle->create(object.get_width() / 2.0, 50, colour);

						transform->set_rotation_origin(object.get_width() / 2.0, object.get_height() / 2.0);
						transform->rotate(object.get_rotation());

						renderable->m_type    = graphics::Renderables::CIRCLE;
						shaderid->m_shader_id = "line";
					}
					else if (object.is_point())
					{
						auto* point = world.create_component<components::Point>(entity);

						const auto colour = map::convert_colour(object_layer.get_tint_colour());
						point->create(2, colour);

						renderable->m_type    = graphics::Renderables::POINT;
						shaderid->m_shader_id = "point";
					}
					else if (!object.get_points().empty())
					{
						auto* polygon = world.create_component<components::Polygon>(entity);
						for (const auto& point : object.get_points())
						{
							polygon->add_point(point.get_x(), point.get_y());
						}

						const auto colour = map::convert_colour(object_layer.get_tint_colour());
						polygon->create(colour);

						renderable->m_type    = graphics::Renderables::POLYGON;
						shaderid->m_shader_id = "line";
					}
					else
					{
						// Is a rectangle.

						auto* polygon = world.create_component<components::Polygon>(entity);
						polygon->add_point(object.get_x(), object.get_y());
						polygon->add_point(object.get_x() + object.get_width(), object.get_y());
						polygon->add_point(object.get_x() + object.get_width(), object.get_y() + object.get_height());
						polygon->add_point(object.get_x(), object.get_y() + object.get_height());

						const auto colour = map::convert_colour(object_layer.get_tint_colour());
						polygon->create(colour);

						transform->set_rotation_origin(object.get_width() / 2.0, object.get_height() / 2.0);
						transform->rotate(object.get_rotation());

						renderable->m_type    = graphics::Renderables::POLYGON;
						shaderid->m_shader_id = "line";
					}
					*/

					renderable->m_z_level = object_layer.get_z_level();

					if (!object.get_name().empty())
					{
						auto* tag  = world.create_component<components::Tag>(entity);
						tag->m_tag = object.get_name();
					}

					if (object.is_visible())
					{
						world.enable(entity);
					}
				}
			}
		}

		void Map::generate_map_entities(core::World& world)
		{
			for (const auto& tile_layer : m_tile_layers)
			{
				if (!is_infinite())
				{
					const auto entity = world.create();

					if (tile_layer.is_visible())
					{
						world.enable(entity);
					}
				}
				else
				{
					GALAXY_LOG(GALAXY_FATAL, "Galaxy does not support infinite maps.");
				}
			}
		}

		const graphics::Colour& Map::get_bg_colour() const noexcept
		{
			return m_bg_colour;
		}

		const int Map::get_compression_level() const noexcept
		{
			return m_compression_level;
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

		const std::vector<TileLayer>& Map::tile_layers() const noexcept
		{
			return m_tile_layers;
		}

		const std::vector<ObjectLayer>& Map::object_layers() const noexcept
		{
			return m_object_layers;
		}

		const std::vector<ImageLayer>& Map::image_layers() const noexcept
		{
			return m_image_layers;
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

		const double Map::get_version() const noexcept
		{
			return m_version;
		}

		const int Map::get_width() const noexcept
		{
			return m_width;
		}

		const nlohmann::json& Map::get_raw() const noexcept
		{
			return m_root;
		}

		const int Map::parse_layers(const nlohmann::json& json, int level)
		{
			if (json.count("layers") > 0)
			{
				const auto& layer_array = json.at("layers");
				for (const auto& layer : layer_array)
				{
					if (json.count("type") > 0)
					{
						const auto& type = layer.at("type");
						if (type == "tilelayer")
						{
							m_tile_layers.emplace_back(layer, level);
							level++;
						}
						else if (type == "objectgroup")
						{
							m_object_layers.emplace_back(layer, level);
							level++;
						}
						else if (type == "imagelayer")
						{
							m_image_layers.emplace_back(layer, level);
							level++;
						}
						else if (type == "group")
						{
							level = parse_layers(layer, level);
						}
					}
				}
			}

			return level;
		}
	} // namespace map
} // namespace galaxy