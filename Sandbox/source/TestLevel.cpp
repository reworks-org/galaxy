//
//  TestLevel.cpp
//  starlight
//
//  Created by reworks on 01/12/2017.
//  Copyright (c) 2017 reworks. All rights reserved.
//

#include <sl/cosl/World.hpp>
#include <sl/physics/Box2DHelper.hpp>
#include <sl/systems/RenderSystem.hpp>
#include <sl/services/ServiceLocator.hpp>

#include "TestLevel.hpp"

TestLevel::TestLevel(const std::string& script)
:Level(script)
{
	// Ensure all layers are preallocated and set to nullptr.
	m_mapBitmaps.resize(sl::Locator::get<sl::World>()->m_systemManager.system<sl::RenderSystem>()->getRenderingLayers());
	for (auto& bitmap : m_mapBitmaps)
	{
		bitmap = nullptr;
	}

	sl::tmx::process_all_layer_types(m_map, m_map->ly_head,
		[this](tmx_map* map, tmx_layer* layer)
		{
			if (layer->name == "collision")
			{
				tmx_object_group* objgr = layer->content.objgr;
				tmx_object *head = objgr->head;
				while (head)
				{
					b2BodyDef bodyDef;
					bodyDef.position.Set(sl::b2::pixelsToMeters<float32>(head->x), sl::b2::pixelsToMeters<float32>(head->y));
					bodyDef.type = b2BodyType::b2_staticBody;

					b2Body* body = sl::Locator::get<sl::Box2DManager>()->world()->CreateBody(&bodyDef);

					b2PolygonShape b2shape;
					b2shape.SetAsBox(sl::b2::pixelsToMeters<float32>(head->width / 2.0f), sl::b2::pixelsToMeters<float32>(head->height / 2.0f), b2Vec2(sl::b2::pixelsToMeters<float32>(head->width / 2.0f), sl::b2::pixelsToMeters<float32>(head->height / 2.0f)), 0.0f);

					b2FixtureDef fixtureDef;
					fixtureDef.density = 1.0f;
					fixtureDef.friction = 0.5f;
					fixtureDef.restitution = 0.0f;
					fixtureDef.shape = &b2shape;

					body->CreateFixture(&fixtureDef);
					body->SetFixedRotation(true);

					m_mapCollisions.push_back(body);

					ALLEGRO_BITMAP* obj_layer = al_create_bitmap(m_map->width, m_map->height);
					al_set_target_bitmap(obj_layer);
					al_clear_to_color(sl::tmx::int_to_al_color_opacity(m_map->backgroundcolor, layer->opacity));
					
					sl::tmx::draw_objects(objgr);
					
					al_flip_display();
					al_set_target_backbuffer(al_get_current_display());
					m_mapBitmaps[tmx_get_property(layer->properties, "layer")->value.integer] = obj_layer;
				}
			}
		},

		[this](tmx_map* map, tmx_layer* layer) 
		{
			ALLEGRO_BITMAP* img_layer = al_create_bitmap(layer->content.image->width, layer->content.image->height);
			al_set_target_bitmap(img_layer);
			al_clear_to_color(sl::tmx::int_to_al_color_opacity(m_map->backgroundcolor, layer->opacity));

			sl::tmx::draw_image_layer(layer);

			al_flip_display();
			al_set_target_backbuffer(al_get_current_display());
			m_mapBitmaps[tmx_get_property(layer->properties, "layer")->value.integer] = img_layer;
		},
		
		[this](tmx_map* map, tmx_layer* layer)
		{
		}
	);
}

TestLevel::~TestLevel()
{
	for (b2Body* body : m_mapCollisions)
	{
		sl::Locator::get<sl::Box2DManager>()->world()->DestroyBody(body);
	}
}

void TestLevel::update()
{

}