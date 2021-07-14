///
/// NuklearUI.hpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#ifndef GALAXY_UI_NUKLEARUI_HPP_
#define GALAXY_UI_NUKLEARUI_HPP_

#include <nuklear_glfw_gl4.h>

namespace galaxy
{
	namespace ui
	{
		///
		/// Holds data relating to Nuklear.
		///
		struct NuklearUI final
		{
			///
			/// Pointer to Nuklear context.
			///
			nk_context* m_context = nullptr;

			///
			/// Nuklear anti-aliasing settings.
			///
			nk_anti_aliasing m_do_aa = nk_anti_aliasing::NK_ANTI_ALIASING_ON;
		};
	} // namespace ui
} // namespace galaxy

#endif