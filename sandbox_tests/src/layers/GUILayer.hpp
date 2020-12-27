///
/// GUILayer.hpp
/// sandbox_tests
///
/// Refer to LICENSE.txt for more details.
///

#ifndef SANDBOXTESTS_GUILAYER_HPP_
#define SANDBOXTESTS_GUILAYER_HPP_

#include <galaxy/core/Layer.hpp>
#include <galaxy/ui/GUI.hpp>

namespace sb
{
	class GUILayer final : public galaxy::core::Layer
	{
	public:
		GUILayer();
		virtual ~GUILayer();

		void events() override;
		void update(const double dt) override;
		void pre_render() override;
		void render() override;

	private:
		galaxy::graphics::Camera m_gui_camera;
		galaxy::ui::GUI m_gui;
		galaxy::ui::UITheme m_theme;
	};
} // namespace sb

#endif