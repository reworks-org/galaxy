///
/// GameLayer.hpp
/// sandbox
///
/// Refer to LICENSE.txt for more details.
///

#ifndef SANDBOX_GAMELAYER_HPP_
#define SANDBOX_GAMELAYER_HPP_

#include <SFML/Graphics.hpp>
#include <galaxy/types/Layer.hpp>

class GameLayer : public galaxy::Layer
{
public:
	GameLayer();

	void event(const sf::Event& event)override;
	void update(sf::Time& dt)override;
	void render() noexcept override;

	sf::CircleShape shape;
	sf::RenderWindow* window;
};

#endif