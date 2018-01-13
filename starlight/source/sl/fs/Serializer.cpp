//
//  Serializer.cpp
//  rework
//
//  Created by reworks on 9/11/2017.
//  Copyright (c) 2017 reworks. All rights reserved.
//

#include <fstream>

#include "cereal/archives/xml.hpp"

#include "re/core/World.hpp"
#include "re/services/ServiceLocator.hpp"

#include "re/components/PhysicsComponent.hpp"
#include "re/components/SerializeComponent.hpp"
#include "re/components/TransformComponent.hpp"

#include "Serializer.hpp"

// https://uscilab.github.io/cereal/quickstart.html

namespace sl
{
	Serializer::Serializer(const std::string& savePath)
	:m_savePath(savePath)
	{
	}

	Serializer::~Serializer()
	{
	}

	void Serializer::saveEntityData(const std::string& saveFile)
	{
		std::ofstream saveStream(m_savePath + saveFile);
		cereal::XMLOutputArchive ar(saveStream);

		Locator::get<World>()->m_entityManager.each<SerializeComponent, TransformComponent>([&](entityx::Entity entity, SerializeComponent& sc, TransformComponent& tc)
		{
			ar(cereal::make_nvp(sc.m_id, tc));
		});

		Locator::get<World>()->m_entityManager.each<SerializeComponent, PhysicsComponent>([&](entityx::Entity entity, SerializeComponent& sc, PhysicsComponent& pc)
		{
			ar(cereal::make_nvp(sc.m_id, pc));
		});
	}

	void Serializer::loadEntityData(const std::string& saveFile)
	{
		std::ifstream loadStream(m_savePath + saveFile);
		cereal::XMLInputArchive ar(loadStream);

		Locator::get<World>()->m_entityManager.each<SerializeComponent, TransformComponent>([&](entityx::Entity entity, SerializeComponent& sc, TransformComponent& tc)
		{
			ar(cereal::make_nvp(sc.m_id, tc));
		});

		Locator::get<World>()->m_entityManager.each<SerializeComponent, PhysicsComponent>([&](entityx::Entity entity, SerializeComponent& sc, PhysicsComponent& pc)
		{
			ar(cereal::make_nvp(sc.m_id, pc));
		});
	}
}