//
//  TestLevel.cpp
//  REngine3
//
//  Created by reworks on 25/10/2016.
//  Copyright (c) 2016 reworks. All rights reserved.
//

#include <SFML/System/Time.hpp>
#include <re/graphics/Window.hpp>

#include "TestLevel.hpp"

TestLevel::TestLevel(const std::string& script)
	:Level(script)
{
}

void TestLevel::Event(sf::Event & e)
{

}

void TestLevel::Update(sf::Time dt)
{

}

void TestLevel::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	m_window->setView(m_camera);

	target.draw()

	m_window->setView(m_window->getDefaultView());
}