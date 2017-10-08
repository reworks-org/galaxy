//
//  Service.hpp
//  rework
//
//  Created by reworks on 9/07/2016.
//  Copyright (c) 2017 reworks. All rights reserved.
//

#ifndef REWORK_SERVICE_HPP_
#define REWORK_SERVICE_HPP_

namespace re
{
	class Service
	{
	public:
		///
		/// Virtual destructor to be inherited by derived classes.
		///
		virtual inline ~Service() {}
	};
}

#endif