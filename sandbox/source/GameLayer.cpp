///
/// GameLayer.cpp
/// sandbox
///
/// Refer to LICENSE.txt for more details.
///


#include <galaxy/core/ServiceLocator.hpp>

#include "GameLayer.hpp"

GameLayer::GameLayer()
	:Layer("GameLayer"), shape(100.0f)
{
	// a layer can have multiple states.

	shape.setFillColor(sf::Color(255, 0, 0));
	window = galaxy::ServiceLocator::i().window();
}

void GameLayer::event(const sf::Event& event)
{
}

void GameLayer::update(sf::Time& dt)
{
}

void GameLayer::render() noexcept
{
	// render must pass pointer to window...
	window->draw(shape);
}