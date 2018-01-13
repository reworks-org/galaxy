//
//  Serializer.hpp
//  rework
//
//  Created by reworks on 9/11/2017.
//  Copyright (c) 2017 reworks. All rights reserved.
//

#ifndef REWORK_SERIALIZE_HPP_
#define REWORK_SERIALIZE_HPP_

namespace sl
{
	class Serializer
	{
	public:
		Serializer(const std::string& savePath);
		~Serializer();
		
		void saveEntityData(const std::string& saveFile);
		void loadEntityData(const std::string& saveFile);

	public:
		std::string m_savePath;
	};
}

#endif