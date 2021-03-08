///
/// Map.cpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#include "galaxy/components/Animated.hpp"
#include "galaxy/components/Primitive2D.hpp"
#include "galaxy/components/Renderable.hpp"
#include "galaxy/components/ShaderID.hpp"
#include "galaxy/components/Tag.hpp"
#include "galaxy/core/ServiceLocator.hpp"
#include "galaxy/core/World.hpp"
#include "galaxy/error/Log.hpp"
#include "galaxy/fs/FileSystem.hpp"
#include "galaxy/graphics/Renderer2D.hpp"
#include "galaxy/res/TextureAtlas.hpp"
#include "galaxy/scripting/JSONUtils.hpp"

#include "Map.hpp"

namespace galaxy
{
	namespace map
	{
		// clang-format off
		Map::Map() noexcept
			: m_loaded{ false }, m_root{}, m_bg_colour{ 255, 255, 255, 255 },
			m_compression_level{ -1 }, m_height{ 0 }, m_hex_side_length{ 0 },
			m_infinite{ false }, m_next_layer_id{ 0 }, m_next_object_id{ 0 },
			m_orientation{ "orthogonal" }, m_render_order{ "right-down" },
			m_stagger_axis{ "" }, m_stagger_index{ "" }, m_tiled_version{ "" },
			m_tile_height{ 0 }, m_tile_width{ 0 }, m_type{ "map" }, m_version{ 0.0 },
			m_width{ 0 }
		{
		}
		
		Map::Map(std::string_view map) noexcept
			: m_loaded{ false }, m_root{}, m_bg_colour{ 255, 255, 255, 255 },
			m_compression_level{ -1 }, m_height{ 0 }, m_hex_side_length{ 0 },
			m_infinite{ false }, m_next_layer_id{ 0 }, m_next_object_id{ 0 },
			m_orientation{ "orthogonal" }, m_render_order{ "right-down" },
			m_stagger_axis{ "" }, m_stagger_index{ "" }, m_tiled_version{ "" },
			m_tile_height{ 0 }, m_tile_width{ 0 }, m_type{ "map" }, m_version{ 0.0 },
			m_width{ 0 }
		{
			if (!load(map))
			{
				GALAXY_LOG(GALAXY_ERROR, "Failed to load map from constructor: {0}.", map);
			}
		}
		// clang-format on

		Map::Map(Map&& m) noexcept
		{
		}

		Map& Map::operator=(Map&& m) noexcept
		{
			if (this != &m)
			{
			}

			return *this;
		}

		Map::~Map() noexcept
		{
			for (auto& data : m_data)
			{
				data->m_batch->clear();
				data->m_batch_data.clear();

				data->m_batch.reset();
				data.reset();
			}

			m_data.clear();
			m_root.clear();
			m_properties.clear();
			m_tile_layers.clear();
			m_object_layers.clear();
			m_image_layers.clear();
			m_tilesets.clear();
		}

		const bool Map::load(std::string_view map)
		{
			const auto json = json::parse_from_disk(map);
			if (json == std::nullopt)
			{
				GALAXY_LOG(GALAXY_ERROR, "Failed to load json tiled map: {0}.", map);
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
						m_tilesets.emplace_back(tileset);
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

				if (m_infinite)
				{
					GALAXY_LOG(GALAXY_ERROR, "Does not support infinite maps.");
					return false;
				}
				else
				{
					if (m_render_order == "right-down")
					{
						parse_tilesets();
						parse_layers(m_root, 0);

						return true;
					}
					else
					{
						GALAXY_LOG(GALAXY_ERROR, "Can only parse right-down maps.");
						return false;
					}
				}
			}
			else
			{
				GALAXY_LOG(GALAXY_ERROR, "Tried to parse map that was not loaded.");
				return false;
			}
		}

		void Map::create(core::World& world)
		{
			for (const auto& imagelayer : m_image_layers)
			{
				create_image_layer(imagelayer);
			}

			for (const auto& objectlayer : m_object_layers)
			{
				create_object_layer(objectlayer, world);
			}

			for (const auto& tilelayer : m_tile_layers)
			{
				create_tile_layer(tilelayer, world);
			}

			update_batches();
		}

		void Map::update_batches()
		{
			for (const auto& data : m_data)
			{
				data->m_batch->calculate();
			}
		}

		void Map::render(graphics::Camera& camera)
		{
			for (const auto& data : m_data)
			{
				graphics::Renderer2D::draw_batch(data->m_batch.get(), camera);
			}
		}

		void Map::enable_objects(core::World& world) noexcept
		{
			for (const auto entity : m_object_entities)
			{
				world.enable(entity);
			}
		}

		void Map::disable_objects(core::World& world) noexcept
		{
			for (const auto entity : m_object_entities)
			{
				world.disable(entity);
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

		const std::vector<ImageLayer>& Map::get_image_layers() const noexcept
		{
			return m_image_layers;
		}

		const std::vector<ObjectLayer>& Map::get_object_layers() const noexcept
		{
			return m_object_layers;
		}

		const std::vector<TileLayer>& Map::get_tile_layers() const noexcept
		{
			return m_tile_layers;
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

		const std::vector<Tileset>& Map::get_tile_sets() const noexcept
		{
			return m_tilesets;
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

		void Map::parse_tilesets()
		{
			for (auto& tileset : m_tilesets)
			{
				const auto tileset_name    = std::filesystem::path(tileset.get_image()).stem().string();
				const auto& tileset_region = SL_HANDLE.atlas()->get_region(tileset_name);

				for (const auto& tile : tileset.get_tiles())
				{
					const int rows   = tileset.get_tile_count() / tileset.get_columns();
					const int column = tile.get_id() % tileset.get_columns();
					const int row    = tile.get_id() / tileset.get_columns();

					auto u = (column != 0) ? (column * tileset.get_tile_width()) : (0 * tileset.get_tile_width());
					auto v = (row < rows - 1) ? (row * tileset.get_tile_height()) : ((rows - 1) * tileset.get_tile_height());

					u += ((column * tileset.get_spacing()) + tileset.get_margin());
					v += ((row * tileset.get_spacing()) + tileset.get_margin());

					u += tileset_region.m_x;
					v += tileset_region.m_y;

					const auto name = tileset_name + std::to_string(tile.get_id());
					graphics::fRect rect;
					rect.m_x      = u;
					rect.m_y      = v;
					rect.m_width  = tileset.get_tile_width();
					rect.m_height = tileset.get_tile_height();

					SL_HANDLE.atlas()->add_custom_region(name, rect);
				}
			}
		}

		const int Map::parse_layers(const nlohmann::json& json, int level)
		{
			if (json.count("layers") > 0)
			{
				const auto& layer_array = json.at("layers");
				for (const auto& layer : layer_array)
				{
					if (layer.count("type") > 0)
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

		void Map::create_image_layer(const ImageLayer& layer)
		{
			if (layer.is_visible())
			{
				const auto level = layer.get_z_level();
				if (level >= m_data.size())
				{
					m_data.resize(level + 1);
					m_data[level]          = std::make_unique<RenderData>();
					m_data[level]->m_batch = std::make_unique<graphics::SpriteBatch>((4096 / m_tile_width) * (4096 / m_tile_height));
					m_data[level]->m_batch->set_texture(SL_HANDLE.atlas()->get_atlas());
				}

				auto& data = m_data[level]->m_batch_data.emplace_back(std::make_unique<BatchData>());

				data->first  = std::make_unique<components::BatchSprite>();
				data->second = std::make_unique<components::Transform>();

				data->first->create(std::filesystem::path(layer.get_image()).stem().string(), layer.get_opacity());
				data->second->set_pos(layer.get_offset_x(), layer.get_offset_y());

				auto& end = m_data[level]->m_batch_data.back();
				m_data[level]->m_batch->add(end->first.get(), end->second.get(), level);
			}
		}

		void Map::create_object_layer(const ObjectLayer& layer, core::World& world)
		{
			if (layer.get_draworder() == "topdown")
			{
				const auto& objects = layer.get_objects();
				for (const auto& object : objects)
				{
					const auto type = object.get_type_enum();

					if (type == Object::Type::TEXT)
					{
						GALAXY_LOG(GALAXY_WARNING, "Text objects are not supported.");
					}
					else
					{
						const auto entity     = world.create();
						auto* transform       = world.create_component<components::Transform>(entity);
						auto* shaderid        = world.create_component<components::ShaderID>(entity);
						auto* primitive2d     = world.create_component<components::Primitive2D>(entity);
						auto* renderable      = world.create_component<components::Renderable>(entity);
						renderable->m_z_level = layer.get_z_level();

						switch (type)
						{
							case Object::Type::ELLIPSE:
							{
								components::Primitive2D::PrimitiveData data;
								data.m_colour    = layer.get_colour();
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
								data.m_colour    = layer.get_colour();
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
								data.m_colour = layer.get_colour();
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
								data.m_colour = layer.get_colour();
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
								data.m_colour = layer.get_colour();
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

						m_object_entities.push_back(entity);
					}
				}
			}
			else
			{
				GALAXY_LOG(GALAXY_WARNING, "Only topdown object layers are supported.");
			}
		}

		void Map::create_tile_layer(const TileLayer& layer, core::World& world)
		{
			if (layer.is_visible())
			{
				const auto level = layer.get_z_level();
				if (level >= m_data.size())
				{
					m_data.resize(level + 1);
					m_data[level]          = std::make_unique<RenderData>();
					m_data[level]->m_batch = std::make_unique<graphics::SpriteBatch>((4096 / m_tile_width) * (4096 / m_tile_height));
					m_data[level]->m_batch->set_texture(SL_HANDLE.atlas()->get_atlas());
				}

				const auto& data = layer.get_data();
				for (int i = 0; i < m_height; i++)
				{
					for (int j = 0; j < m_width; j++)
					{
						auto flagged_gid = data[(i * m_width) + j];
						if (flagged_gid != 0)
						{
							// Have to clear flips first.
							const auto gid = unset_tile_flags(flagged_gid);

							auto [tile, tileset]    = get_tile_with_set(gid);
							const auto tileset_name = std::filesystem::path(tileset->get_image()).stem().string();
							const auto tile_name    = tileset_name + std::to_string(tile->get_id());

							if (!tile->get_animations().empty())
							{
								const auto entity = world.create();
								auto* renderable  = world.create_component<components::Renderable>(entity);
								auto* batch       = world.create_component<components::BatchSprite>(entity);
								auto* animated    = world.create_component<components::Animated>(entity);
								auto* transform   = world.create_component<components::Transform>(entity);

								renderable->m_z_level = layer.get_z_level();
								renderable->m_type    = graphics::Renderables::BATCHED;

								batch->create(tile_name, layer.get_opacity());

								std::vector<graphics::Frame> frames;
								frames.reserve(tile->get_animations().size());

								for (const auto& tile_anim : tile->get_animations())
								{
									graphics::Frame frame;
									frame.m_time_per_frame = tile_anim.get_duration();

									const auto tile_anim_name = tileset_name + std::to_string(tile_anim.get_tile_id());
									frame.set_region(tile_anim_name);

									frames.emplace_back(frame);
								}

								animated->add_animation("TileAnimation", "TileAnimation", true, 1.0, frames);
								animated->set_animation("TileAnimation");
								animated->play();

								transform->set_pos(layer.get_offset_x() + (j * tileset->get_tile_width()), layer.get_offset_y() + (i * tileset->get_tile_height()));

								world.enable(entity);
							}
							else
							{
								auto& batch_data   = m_data[level]->m_batch_data.emplace_back(std::make_unique<BatchData>());
								batch_data->first  = std::make_unique<components::BatchSprite>();
								batch_data->second = std::make_unique<components::Transform>();
								batch_data->first->create(tile_name, layer.get_opacity());
								batch_data->second->set_pos(layer.get_offset_x() + (j * tileset->get_tile_width()), layer.get_offset_y() + (i * tileset->get_tile_height()));

								auto& end = m_data[level]->m_batch_data.back();
								m_data[level]->m_batch->add(end->first.get(), end->second.get(), level);
							}
						}
					}
				}
			}
		}

		TileWithSet Map::get_tile_with_set(const int gid)
		{
			for (auto& tileset : m_tilesets)
			{
				auto& tiles         = tileset.get_tiles();
				const auto local_id = gid - tileset.get_first_gid();

				auto res = std::find_if(std::execution::par, tiles.begin(), tiles.end(), [&](const Tile& tile) {
					return tile.get_id() == local_id;
				});

				if (res != tiles.end())
				{
					return std::make_tuple(&(*res), &tileset);
				}
			}

			return std::make_tuple(nullptr, nullptr);
		}
	} // namespace map
} // namespace galaxy