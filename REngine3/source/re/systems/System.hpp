//
//  System.hpp
//  REngine3
//
//  Created by reworks on 10/07/2016.
//  Copyright (c) 2016 reworks. All rights reserved.
//

#ifndef RENGINE3_SYSTEM_HPP_
#define RENGINE3_SYSTEM_HPP_

#include "re/entity/Entity.hpp"

class System
{
public:
	virtual inline ~System() {}

protected:
	std::vector<std::shared_ptr<Entity>> m_attachedEntitys;
};

#endif