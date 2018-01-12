//
//  TestLevel.hpp
//  starlight
//
//  Created by reworks on 01/12/2017.
//  Copyright (c) 2017 reworks. All rights reserved.
//

#ifndef STARLIGHT_TESTLEVEL_HPP_
#define STARLIGHT_TESTLEVEL_HPP_

#include <sl/mapping/Level.hpp>
#include <sl/managers/Box2DManager.hpp>

class TestLevel : public sl::Level
{
public:
	TestLevel(const std::string& script);
	~TestLevel() override;

	void update() override;

private:
	std::vector<b2Body*> m_mapCollisions;
	std::vector<ALLEGRO_BITMAP*> m_mapBitmaps;
};

#endif