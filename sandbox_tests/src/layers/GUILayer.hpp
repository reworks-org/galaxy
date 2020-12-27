///
/// GUILayer.hpp
/// sandbox_tests
///
/// Refer to LICENSE.txt for more details.
///

#ifndef SANDBOXTESTS_GUILAYER_HPP_
#define SANDBOXTESTS_GUILAYER_HPP_

#include <galaxy/state/Layer.hpp>
#include <galaxy/ui/GUI.hpp>

namespace sb
{
	class GUILayer final : public pr::Layer
	{
	public:
		GUILayer();
		virtual ~GUILayer();

		void events() override;
		void update(const double dt) override;
		void pre_render() override;
		void render() override;

	private:
		graphics::Camera m_gui_camera;
		GUI m_gui;
		UITheme m_theme;
	};
} // namespace sb

#endif