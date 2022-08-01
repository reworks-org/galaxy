///
/// Cursor.hpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#ifndef GALAXY_INPUT_CURSOR_HPP_
#define GALAXY_INPUT_CURSOR_HPP_

#include <span>
#include <string_view>

#include "galaxy/input/InputDevice.hpp"

struct GLFWcursor;

namespace galaxy
{
	namespace input
	{
		///
		/// Implementation of the mouse cursor and state management.
		///
		class Cursor final : public InputDevice
		{
			friend class core::Window;

		public:
			///
			/// Destructor.
			///
			virtual ~Cursor() noexcept;

			///
			/// Set cursor visibility.
			///
			/// \param visible True for cursor to be visible, otherwise false.
			///
			void toggle(const bool visible) noexcept;

			///
			/// Set cursor icon.
			///
			/// \param icon Icon to load.
			///
			void set_cursor_icon(std::string_view icon);

			///
			/// Set cursor icon.
			///
			/// \param buffer Memory buffer containing pixels.
			///
			void set_cursor_icon(std::span<unsigned char> buffer);

			///
			/// Checks if cursor is within window.
			///
			/// \return True if cursor is within the content area of the window.
			///
			[[nodiscard]] bool within_window() noexcept;

		private:
			///
			/// Constructor.
			///
			Cursor() noexcept;

			///
			/// Move constructor.
			///
			Cursor(Cursor&&) = delete;

			///
			/// Move assignment operator.
			///
			Cursor& operator=(Cursor&&) = delete;

			///
			/// Copy constructor.
			///
			Cursor(const Cursor&) = delete;

			///
			/// Copy assignment operator.
			///
			Cursor& operator=(const Cursor&) = delete;

		public:
			///
			/// Allows for custom cursor properties.
			///
			GLFWcursor* m_data;
		};
	} // namespace input
} // namespace galaxy

#endif