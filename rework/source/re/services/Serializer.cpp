//
//  Serializer.cpp
//  rework
//
//  Created by reworks on 9/11/2017.
//  Copyright (c) 2017 reworks. All rights reserved.
//

#include "cereal/archives/json.hpp"

#include "re/core/World.hpp"
#include "re/services/ServiceLocator.hpp"

#include "re/components/SerializeComponent.hpp"
#include "re/components/TransformComponent.hpp"
#include "re/components/PhysicsComponent.hpp"
#include "re/components/AnimationComponent.hpp"

#include "Serializer.hpp"

// https://uscilab.github.io/cereal/serialization_archives.html

namespace re
{
	Serializer::Serializer(const std::string& savePath)
	:m_savePath(savePath)
	{
	}

	Serializer::~Serializer()
	{
	}

	void Serializer::serializeEntitys()
	{
		Locator::get<World>()->m_entityManager.each<SerializeComponent, TransformComponent>([](entityx::Entity entity, SerializeComponent& sc, TransformComponent& tc)
		{
			{
				std::ofstream save(m_savePath+"")
			}
		});
	}
}