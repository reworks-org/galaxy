//
//  World.hpp
//  REngine3
//
//  Created by reworks on 9/07/2016.
//  Copyright (c) 2016 reworks. All rights reserved.
//

#ifndef RENGINE3_WORLD_HPP_
#define RENGINE3_WORLD_HPP_

#include "re/entity/Entity.hpp"
#include "re/services/Service.hpp"

namespace re
{
	class World : public Service
	{
	public:
		~World() override;

		void AddEntity(std::shared_ptr<Entity> entity);
		void RemoveEntity();
		void DeleteEntity();
		void RestoreEntity();
		void DeleteEntitys();
		void GetEntity();

		void AddSystem();
		void DeleteSystem();
		void GetSystem();

	private:
		
	};
}

#endif