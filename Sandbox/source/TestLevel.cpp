//
//  TestLevel.cpp
//  rework
//
//  Created by reworks on 01/12/2017.
//  Copyright (c) 2017 reworks. All rights reserved.
//

#include <re/physics/Box2DHelper.hpp>
#include <re/services/ServiceLocator.hpp>

#include "TestLevel.hpp"

TestLevel::TestLevel(const std::string& script)
:Level(script)
{
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
				}
			}
		},

		[this](tmx_map* map, tmx_layer* layer) 
		{
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