///
/// Map.cpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#include <filesystem>
#include <fstream>

#include "galaxy/components/Primitive2D.hpp"
#include "galaxy/components/Renderable.hpp"
#include "galaxy/components/ShaderID.hpp"
#include "galaxy/components/Sprite2D.hpp"
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
					unsigned int counter      = 0;
					const auto& tileset_array = m_root.at("tilesets");
					for (const auto& tileset : tileset_array)
					{
						std::string name;
						if (tileset.count("name") > 0)
						{
							name = tileset.at("name");
						}
						else
						{
							name = std::to_string(counter);
						}

						m_tile_sets.emplace(name, tileset_array);
						counter++;
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
				renderable->m_type    = graphics::Renderables::BATCHED;
				renderable->m_z_level = image_layer.get_z_level();

				auto* sprite2d = world.create_component<components::Sprite2D>(entity);
				sprite2d->create(std::filesystem::path(image_layer.get_image()).stem().string(), static_cast<float>(image_layer.get_opacity()));

				auto* transform = world.create_component<components::Transform>(entity);
				transform->set_pos(image_layer.get_offset_x(), image_layer.get_offset_y());

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
					const auto type = object.get_type_enum();

					if (type != Object::Type::POLYLINE && type != Object::Type::TEXT)
					{
						const auto entity     = world.create();
						auto* transform       = world.create_component<components::Transform>(entity);
						auto* shaderid        = world.create_component<components::ShaderID>(entity);
						auto* primitive2d     = world.create_component<components::Primitive2D>(entity);
						auto* renderable      = world.create_component<components::Renderable>(entity);
						renderable->m_z_level = object_layer.get_z_level();

						switch (type)
						{
							case Object::Type::ELLIPSE:
							{
								components::Primitive2D::PrimitiveData data;
								data.m_colour    = object_layer.get_colour();
								data.m_fragments = std::make_optional(40);
								data.m_radii     = std::make_optional<glm::vec2>(object.get_width() / 2.0, object.get_height() / 2.0);

								primitive2d->create<graphics::Primitives::ELLIPSE>(data);

								transform->set_pos(object.get_x(), object.get_y());
								transform->rotate(object.get_rotation());

								renderable->m_type    = graphics::Renderables::LINE_LOOP;
								shaderid->m_shader_id = "line";
							}
							break;

							case Object::Type::POINT:
							{
								components::Primitive2D::PrimitiveData data;
								data.m_colour    = object_layer.get_colour();
								data.m_pointsize = 3;
								primitive2d->create<graphics::Primitives::POINT>(data);

								transform->set_pos(object.get_x(), object.get_y());
								transform->rotate(object.get_rotation());

								renderable->m_type    = graphics::Renderables::POINT;
								shaderid->m_shader_id = "point";
							}
							break;

							case Object::Type::POLYGON:
							{
								components::Primitive2D::PrimitiveData data;
								std::vector<glm::vec2> points;
								for (const auto& point : object.get_points())
								{
									points.emplace_back(point.get_x(), point.get_y());
								}

								data.m_points = std::make_optional(points);
								data.m_colour = object_layer.get_colour();
								primitive2d->create<graphics::Primitives::POLYGON>(data);

								transform->set_pos(object.get_x(), object.get_y());
								transform->rotate(object.get_rotation());

								renderable->m_type    = graphics::Renderables::LINE_LOOP;
								shaderid->m_shader_id = "line";
							}
							break;

							case Object::Type::POLYLINE:
							{
								components::Primitive2D::PrimitiveData data;
								std::vector<glm::vec2> points;
								for (const auto& point : object.get_points())
								{
									points.emplace_back(point.get_x(), point.get_y());
								}

								data.m_points = std::make_optional(points);
								data.m_colour = object_layer.get_colour();
								primitive2d->create<graphics::Primitives::POLYLINE>(data);

								transform->set_pos(object.get_x(), object.get_y());
								transform->rotate(object.get_rotation());

								renderable->m_type    = graphics::Renderables::LINE;
								shaderid->m_shader_id = "line";
							}
							break;

							case Object::Type::RECT:
							{
								components::Primitive2D::PrimitiveData data;
								std::vector<glm::vec2> points;
								points.emplace_back(0.0f, 0.0f);
								points.emplace_back(0.0f + object.get_width(), 0.0f);
								points.emplace_back(0.0f + object.get_width(), 0.0f + object.get_height());
								points.emplace_back(0.0f, 0.0f + object.get_height());

								data.m_points = std::make_optional(points);
								data.m_colour = object_layer.get_colour();
								primitive2d->create<graphics::Primitives::POLYGON>(data);

								transform->set_pos(object.get_x(), object.get_y());
								transform->rotate(object.get_rotation());

								renderable->m_type    = graphics::Renderables::LINE_LOOP;
								shaderid->m_shader_id = "line";
							}
							break;
						}

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
		}

		void Map::generate_map_entities(core::World& world)
		{
			if (!is_infinite())
			{
				for (const auto& tile_layer : m_tile_layers)
				{
					const auto entity = world.create();

					if (tile_layer.is_visible())
					{
						world.enable(entity);
					}
				}
			}
			else
			{
				GALAXY_LOG(GALAXY_ERROR, "Galaxy does not support infinite maps.");
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

		const robin_hood::unordered_flat_map<std::string, Tileset>& Map::get_tile_sets() const noexcept
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