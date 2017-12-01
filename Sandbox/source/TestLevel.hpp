//
//  TestLevel.hpp
//  rework
//
//  Created by reworks on 01/12/2017.
//  Copyright (c) 2017 reworks. All rights reserved.
//

#ifndef REWORK_TESTLEVEL_HPP_
#define REWORK_TESTLEVEL_HPP_

#include <re/mapping/Level.hpp>

class TestLevel : public re::Level
{
public:
	TestLevel(const std::string& script);
	~TestLevel() override;

private:

};

#endif