///
/// Popup.hpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#ifndef GALAXY_UI_WIDGETS_POPUP_HPP_
#define GALAXY_UI_WIDGETS_POPUP_HPP_

#include <functional>
#include <string_view>
#include <string>

namespace galaxy
{
	///
	/// Cleaner way of rendering an ImGui popup.
	///
	class Popup final
	{
	public:
		///
		/// Constructor.
		///
		/// \param id Unique ID for this popup.
		///
		Popup(std::string_view id) noexcept;

		///
		/// Destructor.
		///
		~Popup() noexcept;

		///
		/// Draw the button to show the popup.
		///
		/// \param text Text to show on the button.
		///
		void draw_button(const char* text) noexcept;

		///
		/// Draw a menu button to show the popup.
		///
		/// \param text Text to show on the menubar.
		///
		void draw_menuitem(const char* text) noexcept;

		///
		/// Show popup.
		///
		/// \param func ImGui controls to show on popup.
		///
		void draw(std::move_only_function<void(void)>&& func);

	private:
		///
		/// Default constructor (deleted).
		///
		Popup() = delete;

	private:
		///
		/// Unique id (name) of the popup.
		///
		std::string m_id;

		///
		/// Controls state of popup.
		///
		bool m_show;
	};
} // namespace galaxy

#endif
