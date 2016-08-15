//
//  StateIdentifiers.hpp
//  REngine3
//
//  Created by reworks on 25/07/2016.
//  Copyright (c) 2016 reworks. All rights reserved.
//

#ifndef RENGINE3_STATEIDENTIFIERS_HPP_
#define RENGINE3_STATEIDENTIFIERS_HPP_

namespace re
{
	/*
		Inherit from this and add your own state ID's.
		Must not use values -1 to 3.
		Must be a const static int variable.
	*/
	struct StateID
	{
		const static int none = 0;
		const static int menu = 1;
		const static int game = 2;
		const static int loading = 3;
		const static int pause = 4;
		const static int gameover = 5;
	};
}

#endif