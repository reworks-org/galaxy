//
//  Serialization.hpp
//  REngine3
//
//  Created by reworks on 20/10/2016.
//  Copyright (c) 2016 reworks. All rights reserved.
//

#ifndef RENGINE3_SERIALIZATION_HPP_
#define RENGINE3_SERIALIZATION_HPP_

#include <fstream>

namespace re
{
	class Serialization
	{
	public:
		Serialization(const std::string& fileName);

		void Save();

		void Load();

	private:
		std::ofstream m_out;
		std::ifstream m_in;
	};
}

#endif

// http://www.boost.org/doc/libs/1_62_0/libs/serialization/doc/index.html