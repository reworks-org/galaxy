//
//  MoveSystem.cpp
//  REngine3
//
//  Created by reworks on 07/10/2016.
//  Copyright (c) 2016 reworks. All rights reserved.
//

#include <sl/cosl/World.hpp>
#include <sl/services/ServiceLocator.hpp>
#include <sl/components/PhysicsComponent.hpp>
#include <sl/components/TransformComponent.hpp>

#include "MoveSystem.hpp"

using namespace sl;

MoveSystem::MoveSystem(const std::string& playerName)
	:m_player(playerName)
{
}

MoveSystem::~MoveSystem()
{
}

void MoveSystem::move(double velocity)
{
	auto phys = Locator::get<World>()->m_entitys[m_player].component<PhysicsComponent>();
	auto vel = phys->m_body->GetLinearVelocity();

	vel.x = velocity;
	phys->m_body->SetLinearVelocity(vel);
}

void MoveSystem::jump(double velocity)
{
	velocity *= -1;

	auto phys = Locator::get<World>()->m_entitys[m_player].component<PhysicsComponent>();
	auto vel = phys->m_body->GetLinearVelocity();

	vel.y = velocity;
	phys->m_body->SetLinearVelocity(vel);
}

void MoveSystem::update(entityx::EntityManager & es, entityx::EventManager & events, entityx::TimeDelta dt)
{
}