//
//  TestLevel.hpp
//  REngine3
//
//  Created by reworks on 25/10/2016.
//  Copyright (c) 2016 reworks. All rights reserved.
//

#ifndef RENGINE3_TESTLEVEL_HPP_
#define RENGINE3_TESTLEVEL_HPP_

#include <re/mapping/Level.hpp>

class TestLevel : public re::Level
{
public:
	TestLevel(const std::string& script);

	/*
	* IMPORTS: none
	* EXPORTS: none
	* PURPOSE: Process level events.
	*/
	void Event(sf::Event& e) override;

	/*
	* IMPORTS: delta time.
	* EXPORTS: none
	* PURPOSE: Update state.
	*/
	void Update(sf::Time dt) override;

	/*
	* IMPORTS: none
	* EXPORTS: none
	* PURPOSE: Render level.
	*/
	void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

private:

};

#endif