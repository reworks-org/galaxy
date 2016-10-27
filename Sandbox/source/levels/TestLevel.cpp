//
//  TestLevel.cpp
//  REngine3
//
//  Created by reworks on 25/10/2016.
//  Copyright (c) 2016 reworks. All rights reserved.
//

#include <SFML/System/Time.hpp>
#include <SFML/Graphics/RenderTarget.hpp>

#include <re/graphics/Window.hpp>

#include "TestLevel.hpp"

TestLevel::TestLevel(const std::string& script, re::Window* window)
	:Level(script, sf::View(sf::Vector2f(1280, 720) / 2.f, sf::Vector2f(1280, 720)))
{
	m_currentMap->ShowObjects();
	m_window = window;
}

void TestLevel::Event(sf::Event& e)
{

}

void TestLevel::Update(sf::Time dt)
{
	m_window->setView(m_camera);
}

void TestLevel::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	target.draw(*(m_currentMap));
}