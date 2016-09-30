//
//  State.cpp
//  REngine3
//
//  Created by reworks on 25/07/2016.
//  Copyright (c) 2016 reworks. All rights reserved.
//

#include "re/app/World.hpp"
#include "re/services/ServiceLocator.hpp"
#include "re/systems/state/StateSystem.hpp"

namespace re
{
	State::~State()
	{
	}

	void State::RequestStackPush(int stateID)
	{
		Locator::Get<World>()->Get<StateSystem>()->PushState(stateID);
	}

	void State::RequestStackPop()
	{
		Locator::Get<World>()->Get<StateSystem>()->PopState();
	}

	void State::RequestStateClear()
	{
		Locator::Get<World>()->Get<StateSystem>()->ClearStates();
	}
}