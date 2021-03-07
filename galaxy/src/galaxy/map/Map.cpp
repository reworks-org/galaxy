///
/// Map.cpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#include <span>

#define CUTE_TILED_IMPLEMENTATION
#include <cute_tiled.h>

#include "galaxy/components/Primitive2D.hpp"
#include "galaxy/core/ServiceLocator.hpp"
#include "galaxy/error/Log.hpp"
#include "galaxy/fs/FileSystem.hpp"
#include "galaxy/graphics/Renderer2D.hpp"
#include "galaxy/res/TextureAtlas.hpp"

#include "Map.hpp"

namespace galaxy
{
	namespace map
	{
		Map::Map() noexcept
		    : m_map {nullptr}
		{
		}

		Map::Map(std::string_view map) noexcept
		    : m_map {nullptr}
		{
			if (!load(map))
			{
				GALAXY_LOG(GALAXY_ERROR, "Failed to load map from constructor: {0}.", map);
			}
		}

		Map::Map(Map&& m) noexcept
		{
			this->m_data = std::move(m.m_data);
			this->m_map  = m.m_map;

			m.m_map = nullptr;
		}

		Map& Map::operator=(Map&& m) noexcept
		{
			if (this != &m)
			{
				this->m_data = std::move(m.m_data);
				this->m_map  = m.m_map;

				m.m_map = nullptr;
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

			if (m_map != nullptr)
			{
				cute_tiled_free_map(m_map);
			}
		}

		const bool Map::load(std::string_view map)
		{
			const auto path = SL_HANDLE.vfs()->absolute(map);
			if (path != std::nullopt)
			{
				m_map = cute_tiled_load_map_from_file(path.value().c_str(), nullptr);
				if (!m_map)
				{
					GALAXY_LOG(GALAXY_ERROR, "Failed to load map using cute_tiled: {0}.", map);
					return false;
				}
				else
				{
					if (m_map->infinite)
					{
						GALAXY_LOG(GALAXY_ERROR, "Cannot parse infinite maps.");
						return false;
					}
					else
					{
						parse_tilesets();
						parse_layers(m_map->layers, 0);
						update_batches();

						return true;
					}
				}
			}
			else
			{
				GALAXY_LOG(GALAXY_ERROR, "Failed to load map from disk: {0}.", map);
				return false;
			}
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

		void Map::parse_tilesets()
		{
			auto* tileset = m_map->tilesets;
			while (tileset)
			{
				const auto& tileset_name   = std::filesystem::path(tileset->image.ptr).stem().string();
				const auto& tileset_region = SL_HANDLE.atlas()->get_region(tileset_name);

				auto* tile = tileset->tiles;
				while (tile)
				{
					const int rows   = tileset->tilecount / tileset->columns;
					const int column = tile->tile_index % tileset->columns;
					const int row    = tile->tile_index / tileset->columns;

					auto u = (column != 0) ? (column * tileset->tilewidth) : (0 * tileset->tilewidth);
					auto v = (row < rows - 1) ? (row * tileset->tileheight) : ((rows - 1) * tileset->tileheight);

					u += ((column * tileset->spacing) + tileset->margin);
					v += ((row * tileset->spacing) + tileset->margin);

					u += tileset_region.m_x;
					v += tileset_region.m_y;

					const auto name = tileset_name + std::to_string(tile->tile_index);
					graphics::fRect rect;
					rect.m_x      = u;
					rect.m_y      = v;
					rect.m_width  = tileset->tilewidth;
					rect.m_height = tileset->tileheight;

					SL_HANDLE.atlas()->add_custom_region(name, rect);
					tile = tile->next;
				}

				tileset = tileset->next;
			}
		}

		const int Map::parse_layers(cute_tiled_layer_t* layer, int level)
		{
			while (layer)
			{
				if (std::strcmp(layer->type.ptr, "imagelayer") == 0)
				{
					parse_image_layer(layer, level);
					level++;
				}
				else if (std::strcmp(layer->type.ptr, "objectgroup") == 0)
				{
					parse_object_layer(layer, level);
					level++;
				}
				else if (std::strcmp(layer->type.ptr, "tilelayer") == 0)
				{
					parse_tile_layer(layer, level);
					level++;
				}
				else if (std::strcmp(layer->type.ptr, "group") == 0)
				{
					level = parse_layers(layer->layers, level);
				}

				layer = layer->next;
			}

			return level;
		}

		void Map::parse_image_layer(cute_tiled_layer_t* layer, int level)
		{
			if (layer->visible)
			{
				if (level >= m_data.size())
				{
					m_data.resize(level + 1);
					m_data[level]          = std::make_unique<RenderData>();
					m_data[level]->m_batch = std::make_unique<graphics::SpriteBatch>((4096 / m_map->tilewidth) * (4096 / m_map->tileheight));
					m_data[level]->m_batch->set_texture(SL_HANDLE.atlas()->get_atlas());
				}

				auto& data   = m_data[level]->m_batch_data.emplace_back(std::make_unique<BatchData>());
				data->first  = std::make_unique<components::BatchSprite>();
				data->second = std::make_unique<components::Transform>();
				data->first->create(std::filesystem::path(layer->image.ptr).stem().string(), layer->opacity);
				data->second->set_pos(layer->x + layer->offsetx, layer->y + layer->offsety);

				auto& end = m_data[level]->m_batch_data.back();
				m_data[level]->m_batch->add(end->first.get(), end->second.get(), level);
			}
		}

		void Map::parse_object_layer(cute_tiled_layer_t* layer, int level)
		{
			if (layer->visible)
			{
				auto* object = layer->objects;
				while (object)
				{
					/*
					const auto entity     = world.create();
					auto* transform       = world.create_component<components::Transform>(entity);
					auto* shaderid        = world.create_component<components::ShaderID>(entity);
					auto* primitive2d     = world.create_component<components::Primitive2D>(entity);
					auto* renderable      = world.create_component<components::Renderable>(entity);
					renderable->m_z_level = object_layer.get_z_level();

					if (object->ellipse)
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
					else if (object->point)
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

					if (type != Object::Type::POLYLINE && type != Object::Type::TEXT)
					{
						switch (type)
						{
							case Object::Type::ELLIPSE:
							{
							}
							break;

							case Object::Type::POINT:
							{
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
					*/

					object = object->next;
				}
			}
		}

		void Map::parse_tile_layer(cute_tiled_layer_t* layer, int level)
		{
			if (layer->visible)
			{
				if (level >= m_data.size())
				{
					m_data.resize(level + 1);
					m_data[level]          = std::make_unique<RenderData>();
					m_data[level]->m_batch = std::make_unique<graphics::SpriteBatch>((4096 / m_map->tilewidth) * (4096 / m_map->tileheight));
					m_data[level]->m_batch->set_texture(SL_HANDLE.atlas()->get_atlas());
				}

				std::span<int> data = {layer->data, static_cast<std::size_t>(layer->data_count)};
				for (int i = 0; i < m_map->height; i++)
				{
					for (int j = 0; j < m_map->width; j++)
					{
						auto flagged_gid = data[(i * m_map->width) + j];
						if (flagged_gid != 0)
						{
							// Have to clear flips first.
							int h_flip = 0, v_flip = 0, d_flip = 0;

							cute_tiled_get_flags(flagged_gid, &h_flip, &v_flip, &d_flip);
							const auto gid = cute_tiled_unset_flags(flagged_gid);

							auto [tile, tileset] = get_tile_with_set(gid);

							if (tile->animation)
							{
							}
							else
							{
								const auto& tileset_name = std::filesystem::path(tileset->image.ptr).stem().string();
								const auto tile_name     = tileset_name + std::to_string(tile->tile_index);

								auto& data   = m_data[level]->m_batch_data.emplace_back(std::make_unique<BatchData>());
								data->first  = std::make_unique<components::BatchSprite>();
								data->second = std::make_unique<components::Transform>();
								data->first->create(tile_name, layer->opacity);
								data->second->set_pos(layer->x + layer->offsetx + (j * tileset->tilewidth), layer->y + layer->offsety + (i * tileset->tileheight));

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
			auto* tileset = m_map->tilesets;
			while (tileset)
			{
				const auto local_id = gid - tileset->firstgid;

				auto* tile = tileset->tiles;
				while (tile)
				{
					if (tile->tile_index == local_id)
					{
						return std::make_tuple(tile, tileset);
					}

					tile = tile->next;
				}

				tileset = tileset->next;
			}

			return std::make_tuple(nullptr, nullptr);
		}
	} // namespace map
} // namespace galaxy