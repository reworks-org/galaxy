///
/// TMXMap.cpp
/// starlight
///
/// Created by reworks on 07/02/2018.
/// MIT License.
/// Refer to LICENSE.txt for more details.
///

#include <allegro5/bitmap_draw.h>
#include <allegro5/allegro_primitives.h>

#include "sl/core/World.hpp"
#include "sl/utils/Time.hpp"
#include "sl/utils/Utils.hpp"
#include "sl/fs/VirtualFS.hpp"
#include "sl/graphics/Window.hpp"
#include "sl/resources/FontBook.hpp"
#include "sl/core/ServiceLocator.hpp"
#include "sl/graphics/TextureAtlas.hpp"
#include "sl/components/RenderComponent.hpp"
#include "sl/components/PhysicsComponent.hpp"
#include "sl/components/AnimationComponent.hpp"
#include "sl/components/TransformComponent.hpp"
#include "sl/libs/boost/numeric/conversion/cast.hpp"

#include "TMXMap.hpp"

namespace sl
{
	TMXMap::TMXMap(const std::string& mapFile, float lineThickness)
	:m_lineThickness(lineThickness), m_internalMapData("")
	{
		// Load map data from vfs.
		m_internalMapData = Locator::virtualFS->openAsString(mapFile);

		// Then parse it with tmxlib.
		m_internalMap = tmx_load_buffer(m_internalMapData.c_str(), boost::numeric_cast<int>(m_internalMapData.size()));

		// Checking to make sure its valid.
		if (!m_internalMap)
		{
			LOG_S(FATAL) << "Failed to load map! File: " << mapFile << ". tmxlib Error: " << tmx_strerr();
		}

		// Process layers.
		processAllLayers(m_internalMap->ly_head);
	}

	TMXMap::~TMXMap()
	{
		// Free up all memory used. The usual.
		tmx_map_free(m_internalMap);
		m_internalMapData.clear();
	}

	void TMXMap::processAllLayers(tmx_layer* layers)
	{
		// The layer structure is a linked-list.
		// So for each layer...
		while (layers)
		{
			// Only if layer is actually visible, ofcourse.
			if (layers->visible)
			{
				// ...process based off of type but only if visible.
				if (layers->type == L_GROUP)
				{
					// If its a ordinary layer, process it.
					processAllLayers(layers->content.group_head);
				}
				else if (layers->type == L_OBJGR)
				{
					// Process all objects in object layer.
					processObjects(m_internalMap, layers);
				}
				else if (layers->type == L_IMAGE)
				{
					// Process all images in image layer.
					processImageLayer(layers);
				}
				else if (layers->type == L_LAYER)
				{
					// Process the map layer.
					processLayer(m_internalMap, layers);
				}
			}

			// Proceed to next layer in the list.
			layers = layers->next;
		}
	}

	ALLEGRO_COLOUR TMXMap::intToColour(int colour, unsigned char opacity)
	{
		unsigned char r = 0;
		unsigned char g = 0;
		unsigned char b = 0;

		// Extract rgb colour values using bit operations.
		r = (colour >> 16) & 0xFF;
		g = (colour >> 8) & 0xFF;
		b = (colour) & 0xFF;

		return al_map_rgba(r, g, b, opacity);
	}

	void TMXMap::drawPolyline(double** points, double x, double y, int pointsc, ALLEGRO_COLOUR colour)
	{
		// Iterate over the points to draw and draw them in a line.
		int i;
		for (i = 1; i<pointsc; i++)
		{
			al_draw_line(x + points[i - 1][0], y + points[i - 1][1], x + points[i][0], y + points[i][1], colour, m_lineThickness);
		}
	}

	void TMXMap::drawPolygon(double** points, double x, double y, int pointsc, ALLEGRO_COLOUR colour)
	{
		// Iterate over each point and draw the lines so the result is a polygon.
		drawPolyline(points, x, y, pointsc, colour);
		if (pointsc > 2)
		{
			al_draw_line(x + points[0][0], y + points[0][1], x + points[pointsc - 1][0], y + points[pointsc - 1][1], colour, m_lineThickness);
		}
	}

	int TMXMap::gidExtractFlags(unsigned int gid)
	{
		int res = 0;

		// Extract tile flip information flags.
		if (gid & TMX_FLIPPED_HORIZONTALLY) res |= ALLEGRO_FLIP_HORIZONTAL;
		if (gid & TMX_FLIPPED_VERTICALLY)   res |= ALLEGRO_FLIP_VERTICAL;
		/* FIXME allegro has no diagonal flip */

		return res;
	}

	unsigned int TMXMap::gidClearFlags(unsigned int gid)
	{
		// Bit operation to remove all flags on a tile.
		return gid & TMX_FLIP_BITS_REMOVAL;
	}

	void TMXMap::processImageLayer(tmx_layer* layer)
	{
		// On an image layer, we want to create an entity that represents the background to render.
		float op = layer->opacity;
		entt::DefaultRegistry::entity_type entity = Locator::world->m_registry.create();

		// Set up a render component and transform component using information from the layer.
		Locator::world->m_registry.assign<RenderComponent>(entity, op, Utils::removeExtension(layer->content.image->source));
		Locator::world->m_registry.assign<TransformComponent>(entity, tmx_get_property(layer->properties, "layer")->value.integer, 0.0f, Rect<float, int>{ static_cast<float>(layer->offsetx), static_cast<float>(layer->offsety), boost::numeric_cast<int>(layer->content.image->width), boost::numeric_cast<int>(layer->content.image->height) });
	}

	void TMXMap::processObjects(tmx_map* map, tmx_layer* layer)
	{
		// Retrieve layer data.
		static unsigned int poCounter = 0;
		unsigned int w = map->width * map->tile_width;
		unsigned int h = map->height * map->tile_height;
		tmx_object_group* objgr = layer->content.objgr;

		// Prep for layer contstruction.
		sol::state loader;
		tmx_object *head = objgr->head;
		ALLEGRO_COLOR color = intToColour(objgr->color, 255);

		// Create objects bitmap.
		ALLEGRO_BITMAP* objects = al_create_bitmap(w, h);
		al_set_target_bitmap(objects);

		// Assign object to an entity and create transform data.
		entt::DefaultRegistry::entity_type objLayerEntity = Locator::world->m_registry.create();
		Locator::world->m_registry.assign<TransformComponent>(objLayerEntity, tmx_get_property(layer->properties, "layer")->value.integer, 0.0f, Rect<float, int>{ 0.0f, 0.0f, boost::numeric_cast<int>(w), boost::numeric_cast<int>(h) }); // We use a super large layer height to ensure this component is always on top.

		// Prepare to draw texture to render.
		al_clear_to_color(al_map_rgba_f(1.0f, 1.0f, 1.0f, 0.0f));

		while (head)
		{
			if (head->visible)
			{
				if (head->obj_type == OT_SQUARE)
				{
					// If it is a square, check for a name and if so create an entity to process physics data in box2d with.
					if (Utils::nullToEmpty(head->name) != "")
					{
						// Get path to script.
						std::string physObjScriptPath = Locator::world->m_scriptFolderPath + std::string(head->name);

						// Prepare to create entity and load data.
						entt::DefaultRegistry::entity_type physObjEntity = Locator::world->m_registry.create();

						// Process script.
						loader.script(Locator::virtualFS->openAsString(physObjScriptPath));
						sol::table physicsObjectTable = loader.get<sol::table>("PhysicsObject");

						if (!physicsObjectTable.empty())
						{
							// Then assign lua tables to components to create them.
							Locator::world->m_registry.assign<PhysicsComponent>(physObjEntity, physicsObjectTable);

							// Then retrieve the registered component and ensure fixtures have correct collision entities.
							Locator::world->m_registry.get<PhysicsComponent>(physObjEntity).setFixtureEntity(physObjEntity);
						}
						else
						{
							LOG_S(WARNING) << "Physics Object table was empty! Script Name: " << head->name;
						}
					}

					// Then draw object to object layer.
					al_draw_rectangle(head->x, head->y, head->x + head->width, head->y + head->height, color, m_lineThickness);
				}
				else if (head->obj_type == OT_POLYGON)
				{
					// Draw polygon object to object layer.
					drawPolygon(head->content.shape->points, head->x, head->y, head->content.shape->points_len, color);
				}
				else if (head->obj_type == OT_POLYLINE)
				{
					// Draw polyline object to object layer.
					drawPolyline(head->content.shape->points, head->x, head->y, head->content.shape->points_len, color);
				}
				else if (head->obj_type == OT_ELLIPSE)
				{
					// Draw ellipse object to object layer.
					al_draw_ellipse(head->x + head->width / 2.0, head->y + head->height / 2.0, head->width / 2.0, head->height / 2.0, color, m_lineThickness);
				}
				else if (head->obj_type == OT_TEXT)
				{
					// First we stop drawing the current bitmap...
					al_flip_display();

					// Then get the information about the text.
					tmx_text* objText = head->content.text;
					std::string objTextID = "ObjectLayerText" + std::to_string(Time::getTimeSinceEpoch()) + std::string(objText->text);

					// Create text texture in atlas.
					std::string objTextFont = std::string(objText->fontfamily) + std::to_string(objText->pixelsize);
					objTextFont.erase(std::remove_if(objTextFont.begin(), objTextFont.end(), isspace), objTextFont.end());
					
					// Get text alignment.
					int alignment = 0;
					switch (objText->halign)
					{
					case tmx_horizontal_align::HA_LEFT:
						alignment = ALLEGRO_ALIGN_LEFT;
						break;

					case tmx_horizontal_align::HA_CENTER:
						alignment = ALLEGRO_ALIGN_CENTER;
						break;

					case tmx_horizontal_align::HA_RIGHT:
						alignment = ALLEGRO_ALIGN_RIGHT;
						break;

					case tmx_horizontal_align::HA_NONE:
						alignment = 0;
						break;

					default:
						alignment = 0;
						break;
					}

					// Add text to the texture atlas using retrieved values.
					Locator::textureAtlas->addText(objTextID, objText->text, Locator::fontBook->get(objTextFont.c_str()), intToColour(objText->color, 255), alignment);
					
					// Then create the entity for it.
					entt::DefaultRegistry::entity_type objTextEntity = Locator::world->m_registry.create();
					Locator::world->m_registry.assign<RenderComponent>(objTextEntity, 1.0f, objTextID);
					Locator::world->m_registry.assign<TransformComponent>(objTextEntity, tmx_get_property(layer->properties, "layer")->value.integer, static_cast<float>(head->rotation), Rect<float, int>{ static_cast<float>(head->x), static_cast<float>(head->y), boost::numeric_cast<int>(head->width), boost::numeric_cast<int>(head->height) });

					// then we continue drawing, but don't clear it.
					al_set_target_bitmap(objects);
				}
			}

			head = head->next;
		}

		// Finish drawing image.
		al_flip_display();
		al_set_target_backbuffer(Locator::window->getDisplay());
		
		// Then add it to the texture atlas ensuring a unique id.
		std::string id = "ObjectLayerNo" + std::to_string(Time::getTimeSinceEpoch()) + std::to_string(poCounter);
		Locator::textureAtlas->addTexture(id, objects);

		// Add the new texture to a render component for the entity.
		Locator::world->m_registry.assign<RenderComponent>(objLayerEntity, 1.0f, id);

		// Cleanup.
		al_destroy_bitmap(objects);
		++poCounter;
	}

	void TMXMap::processLayer(tmx_map* map, tmx_layer* layer)
	{
		// Set up variables for layer.
		unsigned long i = 0;
		unsigned long j = 0;
		unsigned int gid = 0;
		unsigned int x = 0;
		unsigned int y = 0;
		unsigned int w = 0;
		unsigned int h = 0;
		unsigned int flags = 0;

		tmx_tileset *ts = nullptr;
		tmx_image *im = nullptr;
		std::string identifier;
		ALLEGRO_BITMAP* tileset = nullptr;
		float op = layer->opacity;

		// Create entity for layer tilemap.
		entt::DefaultRegistry::entity_type entity = Locator::world->m_registry.create();
		Locator::world->m_registry.assign<TransformComponent>(entity, tmx_get_property(layer->properties, "layer")->value.integer, 0.0f, Rect<float, int>{ static_cast<float>(layer->offsetx), static_cast<float>(layer->offsety), boost::numeric_cast<int>(map->width * map->tile_width), boost::numeric_cast<int>(map->height * map->tile_height) });

		// Set up drawing for the tilemap.
		ALLEGRO_BITMAP* tileLayer = al_create_bitmap(map->width * map->tile_width, map->height * map->tile_height);
		al_set_target_bitmap(tileLayer);
		al_clear_to_color(intToColour(map->backgroundcolor, 0));
		al_hold_bitmap_drawing(true);

		// Iterate over each tile in the tilemap.
		for (i = 0; i<map->height; i++)
		{
			for (j = 0; j<map->width; j++)
			{
				// Retrieve the tile grid id, clearing the flags, making sure the tile at that gid is not NULL.
				gid = gidClearFlags(layer->content.gids[(i*map->width) + j]);

				tmx_tile* gidTile = map->tiles[gid];
				if (gidTile != nullptr)
				{
					// If the tile has no animation.
					if (!(gidTile->animation))
					{
						// Retrieve tileset, image and position information.
						ts = gidTile->tileset;
						im = gidTile->image;
						x = gidTile->ul_x;
						y = gidTile->ul_y;
						w = ts->tile_width;
						h = ts->tile_height;

						// The tile will have either an image or a tileset.
						if (im)
						{
							// Retrieve the image name and get the sub-bitmap for the tile image.
							identifier = Utils::removeExtension(im->source);
							tileset = Locator::textureAtlas->al_create_packed_sub_bitmap(identifier);
						}
						else
						{
							// Else retrieve tileset from texture atlas.
							identifier = Utils::removeExtension(ts->image->source);
							tileset = Locator::textureAtlas->al_create_packed_sub_bitmap(identifier);
						}

						// Retrieve any flags and pass to the allegro draw function, drawing the tile to the master tilemap layer image.
						flags = gidExtractFlags(layer->content.gids[(i*map->width) + j]);
						al_draw_tinted_bitmap_region(tileset, al_map_rgba_f(0.0f, 0.0f, 0.0f, op), x, y, w, h, j*ts->tile_width, i*ts->tile_height, flags);
						
						// Then destroy the used up tileset.
						al_destroy_bitmap(tileset);
					}
					else
					{
						// Else, if the tile HAS animation...

						// Retrieve information.
						ts = gidTile->tileset;
						im = gidTile->image;
						w = ts->tile_width;
						h = ts->tile_height;

						if (im)
						{
							// Retrieve the image name and get the sub-bitmap for the tile image.
							identifier = Utils::removeExtension(im->source);
						}
						else
						{
							// Else retrieve tileset from texture atlas.
							identifier = Utils::removeExtension(ts->image->source);
						}

						// We get that extact tile from the atlas by offsetting the postion of the tile texture in the atlas by
						// the position of the tile texture on the tilesheet.
						Rect<int> pr = Locator::textureAtlas->get(identifier.c_str());
						x = pr.m_x + gidTile->ul_x;
						y = pr.m_y + gidTile->ul_y;
						
						// Create unique atlas rectangle for that tile. No need to duplicate data.
						std::string id(layer->name);
						id += "AnimatedTile" + std::to_string(Time::getTimeSinceEpoch());
						Locator::textureAtlas->addRectToAtlas(id, { boost::numeric_cast<int>(x), boost::numeric_cast<int>(y), boost::numeric_cast<int>(w), boost::numeric_cast<int>(h) });

						// Then create that entity and assign all the data to it.
						entt::DefaultRegistry::entity_type animatedEntity = Locator::world->m_registry.create();
						Locator::world->m_registry.assign<TransformComponent>(animatedEntity, tmx_get_property(layer->properties, "layer")->value.integer, 0.0f, Rect<float, int>{boost::numeric_cast<float>(j*ts->tile_width), boost::numeric_cast<float>(i*ts->tile_height), boost::numeric_cast<int>(w), boost::numeric_cast<int>(h) });
						Locator::world->m_registry.assign<RenderComponent>(animatedEntity, op, id);
						Locator::world->m_registry.assign<AnimationComponent>(animatedEntity, map, map->tiles[gid], pr.m_x, pr.m_y, boost::numeric_cast<int>(w), boost::numeric_cast<int>(h), layer->name);
					}
				}
				else
				{
					LOG_S(ERROR) << "Map tiles are NULL! Internal Error: " << tmx_strerr();
				}
			}
		}

		// Then reset graphics.
		al_hold_bitmap_drawing(false);
		al_flip_display();
		al_set_target_backbuffer(Locator::window->getDisplay());

		// Add new tilemap to atlas.
		Locator::textureAtlas->addTexture(layer->name, tileLayer);
		al_destroy_bitmap(tileLayer);

		// And assign the tilemap render id to an entity so it can be rendered.
		Locator::world->m_registry.assign<RenderComponent>(entity, op, layer->name);
	}
}