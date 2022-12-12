///
/// NuklearUI.hpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#ifndef GALAXY_UI_NUKLEARUI_HPP_
#define GALAXY_UI_NUKLEARUI_HPP_

#include <nuklear_glfw_gl4.h>
#include <robin_hood.h>

namespace galaxy
{
	namespace ui
	{
		class NuklearUI final
		{
		public:
			using FontCache = robin_hood::unordered_node_map<std::string, nk_font*>;

			NuklearUI();
			~NuklearUI() noexcept;

			void new_frame() noexcept;
			void render() noexcept;

			void set_font(const std::string& font) noexcept;

			nk_context* handle() noexcept;
			nk_glfw* data() noexcept;

		private:
			nk_context* m_context;
			FontCache m_fonts;
		};
	} // namespace ui
} // namespace galaxy

#endif