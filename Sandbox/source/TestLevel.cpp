//
//  TestLevel.cpp
//  rework
//
//  Created by reworks on 01/12/2017.
//  Copyright (c) 2017 reworks. All rights reserved.
//

#include <re/core/World.hpp>
#include <re/physics/Box2DHelper.hpp>
#include <re/systems/RenderSystem.hpp>
#include <re/services/ServiceLocator.hpp>

#include "TestLevel.hpp"

TestLevel::TestLevel(const std::string& script)
:Level(script)
{
	// Ensure all layers are preallocated and set to nullptr.
	m_mapBitmaps.resize(re::Locator::get<re::World>()->m_systemManager.system<re::RenderSystem>()->getRenderingLayers());
	for (auto& bitmap : m_mapBitmaps)
	{
		bitmap = nullptr;
	}

	re::tmx::process_all_layer_types(m_map, m_map->ly_head,
		[this](tmx_map* map, tmx_layer* layer)
		{
			if (layer->name == "collision")
			{
				tmx_object_group* objgr = layer->content.objgr;
				tmx_object *head = objgr->head;
				while (head)
				{
					b2BodyDef bodyDef;
					bodyDef.position.Set(re::b2::pixelsToMeters<float32>(head->x), re::b2::pixelsToMeters<float32>(head->y));
					bodyDef.type = b2BodyType::b2_staticBody;

					b2Body* body = re::Locator::get<re::Box2DManager>()->world()->CreateBody(&bodyDef);

					b2PolygonShape b2shape;
					b2shape.SetAsBox(re::b2::pixelsToMeters<float32>(head->width / 2.0f), re::b2::pixelsToMeters<float32>(head->height / 2.0f), b2Vec2(re::b2::pixelsToMeters<float32>(head->width / 2.0f), re::b2::pixelsToMeters<float32>(head->height / 2.0f)), 0.0f);

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
					al_clear_to_color(re::tmx::int_to_al_color_opacity(m_map->backgroundcolor, layer->opacity));
					
					re::tmx::draw_objects(objgr);
					
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
			al_clear_to_color(re::tmx::int_to_al_color_opacity(m_map->backgroundcolor, layer->opacity));

			re::tmx::draw_image_layer(layer);

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
		re::Locator::get<re::Box2DManager>()->world()->DestroyBody(body);
	}
}

void TestLevel::update()
{

}