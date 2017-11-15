//
//  MoveSystem.hpp
//  Sandbox
//
//  Created by reworks on 21/11/2016.
//  Copyright (c) 2017 reworks. All rights reserved.
//

#ifndef SANDBOX_MOVESYSTEM_HPP_
#define SANDBOX_MOVESYSTEM_HPP_

#include <entityx/System.h>

/// BASIC MOVE SYSTEM EXAMPLE SYSTEM
class MoveSystem : public entityx::System<MoveSystem>
{
public:
	MoveSystem(const std::string& playerName);
	~MoveSystem() override;

	void move(double velocity);
	void jump(double velocity);
	void update(entityx::EntityManager& es, entityx::EventManager& events, entityx::TimeDelta dt) override;

private:
	std::string m_player;
};

#endif