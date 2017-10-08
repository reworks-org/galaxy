//
//  Serialization.hpp
//  rework
//
//  Created by reworks on 20/10/2016.
//  Copyright (c) 2017 reworks. All rights reserved.
//

#ifndef REWORK_SERIALIZATION_HPP_
#define REWORK_SERIALIZATION_HPP_

#include <fstream>

#include <boost/archive/binary_iarchive.hpp>
#include <boost/archive/binary_oarchive.hpp>

#include "re/core/World.hpp"
#include "re/services/ServiceLocator.hpp"
#include "re/components/PhysicsComponent.hpp"
#include "re/components/TransformComponent.hpp"

// http://www.boost.org/doc/libs/1_62_0/libs/serialization/doc/index.html

namespace re
{
	namespace Serialization
	{
		/*
		* IMPORTS: none
		* EXPORTS: none
		* PURPOSE: Serialize the engine.
		*/
		inline void save(World* world, const std::string& fileName)
		{
			std::ofstream out(fileName);

			boost::archive::binary_oarchive oa(out);

			for (auto& it : world->getAliveEntitys())
			{
				oa << it.second;
			
				if (it.second.has<TransformComponent>())
				{
					oa << *(it.second.get<TransformComponent>());
				}

				if (it.second.has<PhysicsComponent>())
				{
					oa << *(it.second.get<PhysicsComponent>());
				}
			}

			for (auto& it : world->getDeadEntitys())
			{
				oa << it.second;

				if (it.second.has<TransformComponent>())
				{
					oa << *(it.second.get<TransformComponent>());
				}

				if (it.second.has<PhysicsComponent>())
				{
					oa << *(it.second.get<PhysicsComponent>());
				}
			}

			out.close();

			RE_LOG_PRINTPRETTY(LogLevel::INFO, "Saved game!");
		}

		/*
		* IMPORTS: none
		* EXPORTS: none
		* PURPOSE: Deserialize the engine.
		*/
		inline void load(World* world, const std::string& fileName)
		{
			std::ifstream in(fileName);

			boost::archive::binary_iarchive ia(in);

			for (auto& it : world->getAliveEntitys())
			{
				ia >> it.second;

				if (it.second.has<TransformComponent>())
				{
					ia >> *(it.second.get<TransformComponent>());
				}
				
				if (it.second.has<PhysicsComponent>())
				{
					ia >> *(it.second.get<PhysicsComponent>());
				}
			}

			for (auto& it : world->getDeadEntitys())
			{
				ia >> it.second;

				if (it.second.has<TransformComponent>())
				{
					ia >> *(it.second.get<TransformComponent>());
				}

				if (it.second.has<PhysicsComponent>())
				{
					ia >> *(it.second.get<PhysicsComponent>());
				}
			}

			in.close();

			RE_LOG_PRINTPRETTY(LogLevel::INFO, "Loaded game!");
		}
	}
}

#endif
