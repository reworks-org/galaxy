///
/// GameState.hpp
/// sandbox
///
/// Created by reworks on 20/02/2018.
/// MIT License.
/// Refer to LICENSE.txt for more details.
///

#ifndef SANDBOX_GAMESTATE_HPP_
#define SANDBOX_GAMESTATE_HPP_

#include <sl/core/StateMachine.hpp>

#include "GameLevel.hpp"

class GameState : public sl::State
{
public:
	GameState() = default;
	~GameState() override;

	void event(ALLEGRO_EVENT* event) override;
	void update(double dt) override;
	void render() override;
};

#endif