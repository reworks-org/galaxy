//
//  State.cpp
//  REngine3
//
//  Created by reworks on 25/07/2016.
//  Copyright (c) 2016 reworks. All rights reserved.
//

#include "re/services/ServiceLocator.hpp"
#include "re/systems/state/StateSystem.hpp"
#include "re/entity/World.hpp"

namespace re
{
	State::~State()
	{
	}

	void State::RequestStackPush(int stateID)
	{
		Locator::Retrieve<World>()->GetSystem<StateSystem>()->PushState(stateID);
	}

	void State::RequestStackPop()
	{
		Locator::Retrieve<World>()->GetSystem<StateSystem>()->PopState();
	}

	void State::RequestStateClear()
	{
		Locator::Retrieve<World>()->GetSystem<StateSystem>()->ClearStates();
	}
}