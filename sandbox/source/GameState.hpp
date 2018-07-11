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

#include "Serialize.hpp"

#include <sl/mapping/TMXMap.hpp>
#include <sl/core/StateMachine.hpp>

class GameState : public sl::State
{
public:
	GameState();
	~GameState() override;

	void event(ALLEGRO_EVENT* event) override;
	void update(double dt) override;
	void render() override;

private:
	std::unique_ptr<sl::TMXMap> map;
	Serialize serialize;
};

#endif