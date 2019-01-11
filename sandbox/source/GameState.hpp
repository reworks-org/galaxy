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

#include <sl/ui/UI.hpp>
#include <sl/core/StateMachine.hpp>
#include <sl/ui/widgets/Label.hpp>
#include <sl/ui/widgets/Slider.hpp>

class GameState : public sl::State
{
public:
	GameState();
	~GameState() override;

	void load() override;
	void unload() override;

	void event(ALLEGRO_EVENT* event) override;
	void update(const double dt) override;
	void render() override;

private:
	sl::UI m_ui;
	sl::UITheme m_theme;
	sl::Panel* m_panel;
	sl::Slider* slider;
	sl::Label* m_sliderLabel;
	std::string sliderLabelStr;
};

#endif