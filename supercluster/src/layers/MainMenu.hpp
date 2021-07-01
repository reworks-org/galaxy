///
/// MainMenu.hpp
/// supercluster
///
/// Refer to LICENSE.txt for more details.
///

#ifndef SUPERCLUSTER_EDITOR_MAINMENU_HPP_
#define SUPERCLUSTER_EDITOR_MAINMENU_HPP_

#include <galaxy/core/Application.hpp>

using namespace galaxy;

namespace sc
{
	class MainMenu final : public core::Layer
	{
	public:
		MainMenu(core::Application* app) noexcept;
		virtual ~MainMenu() noexcept;

		void events() override;
		void update(const double dt) override;
		void pre_render() override;
		void render() override;

	private:
		MainMenu() = delete;
	};
} // namespace sc

#endif