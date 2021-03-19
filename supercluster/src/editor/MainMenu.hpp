///
/// MainMenu.hpp
/// supercluster
///
/// Refer to LICENSE.txt for more details.
///

#ifndef SUPERCLUSTER_EDITOR_MAINMENU_HPP_
#define SUPERCLUSTER_EDITOR_MAINMENU_HPP_

#include "Project.hpp"

namespace sc
{
	class MainMenu final
	{
	public:
		void render(Project* project);

		void enable() noexcept;
		void disable() noexcept;

		const bool active() const noexcept;

	private:
		bool main_menu_active = false;
	};
} // namespace sc

#endif