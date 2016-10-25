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
	/*
	namespace Serialization
	{
		inline void Save(World* world)
		{
		}

		inline void Load(World* world)
		{
		}
	}
	*/

	class Serialization
	{
	public:
		/*
		* IMPORTS: Name of save file.
		* EXPORTS: none
		* PURPOSE: Set the save file to save to.
		*/
		Serialization(const std::string& fileName);

		/*
		* IMPORTS: none
		* EXPORTS: none
		* PURPOSE: Serialize the engine.
		*/
		void Save();

		/*
		* IMPORTS: none
		* EXPORTS: none
		* PURPOSE: Deserialize the engine.
		*/
		void Load();

	private:
		std::ofstream m_out;
		std::ifstream m_in;

		std::string m_fileName;
	};
}

#endif

// http://www.boost.org/doc/libs/1_62_0/libs/serialization/doc/index.html