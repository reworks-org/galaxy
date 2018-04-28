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

#include <sl/types/State.hpp>
#include <sl/core/StateManager.hpp>

#include "GameLevel.hpp"

class GameState : public sl::State<GameState>
{
public:
	GameState() = default;
	~GameState() override;

	void load() override;
	void unload() override;

	void event(ALLEGRO_EVENT* event) override;
	void update(double dt) override;
	void render() override;

private:
	std::unique_ptr<GameLevel> m_gameLevel;
};

#endif