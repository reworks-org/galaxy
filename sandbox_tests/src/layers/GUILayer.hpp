///
/// GUILayer.hpp
/// sandbox_tests
///
/// Refer to LICENSE.txt for more details.
///

#ifndef SANDBOXTESTS_GUILAYER_HPP_
#define SANDBOXTESTS_GUILAYER_HPP_

#include <galaxy/layer/Layer.hpp>
#include <galaxy/ui/GUI.hpp>

namespace sb
{
	class GUILayer final : public galaxy::Layer
	{
	public:
		GUILayer();
		virtual ~GUILayer();

		void events() override;
		void update(const double dt) override;
		void render(qs::Camera& camera) override;

	private:
		galaxy::GUI m_gui;
		galaxy::UITheme m_theme;
	};
} // namespace sb

#endif