//
//  Serializer.hpp
//  rework
//
//  Created by reworks on 9/11/2017.
//  Copyright (c) 2017 reworks. All rights reserved.
//

#ifndef REWORK_SERIALIZE_HPP_
#define REWORK_SERIALIZE_HPP_

namespace re
{
	class Serializer
	{
	public:
		Serializer(const std::string& savePath);
		~Serializer();
		
		void serializeEntitys();

	private:
		std::string m_savePath;
	}
}

#endif