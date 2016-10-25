//
//  Serialization.cpp
//  REngine3
//
//  Created by reworks on 20/10/2016.
//  Copyright (c) 2016 reworks. All rights reserved.
//

#include <boost/archive/binary_iarchive.hpp>
#include <boost/archive/binary_oarchive.hpp>

#include "re/app/World.hpp"
#include "re/services/ServiceLocator.hpp"
#include "re/component/PositionComponent.hpp"

#include "Serialization.hpp"

namespace re
{
	Serialization::Serialization(const std::string& fileName)
	{
		m_fileName = fileName;
	}

	void Serialization::Save()
	{
		m_out.open(m_fileName);

		boost::archive::binary_oarchive oa(m_out);
		
		for (auto& it : Locator::Get<World>()->GetAlive())
		{
			oa << *(it.second);
			oa << *(it.second->Get<PositionComponent>());
		}

		for (auto& it : Locator::Get<World>()->GetDead())
		{
			oa << *(it.second);
			oa << *(it.second->Get<PositionComponent>());
		}

		m_out.close();
	}

	void Serialization::Load()
	{
		m_in.open(m_fileName);

		boost::archive::binary_iarchive ia(m_in);

		for (auto& it : Locator::Get<World>()->GetAlive())
		{
			ia >> *(it.second);
			ia >> *(it.second->Get<PositionComponent>());
		}

		for (auto& it : Locator::Get<World>()->GetDead())
		{
			ia >> *(it.second);
			ia >> *(it.second->Get<PositionComponent>());
		}

		m_in.close();
	}
}