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
			virtual ~Cursor();

			///
			/// Initialize cursor data.
			///
			void init();

			///
			/// Set cursor visibility.
			///
			/// \param visible True for cursor to be visible, otherwise false.
			///
			void toggle(const bool visible);

			///
			/// Load custom cursor data.
			///
			/// \param file Cursor texture file to load.
			///
			void load_custom(std::string_view file);

			///
			/// Load custom cursor data.
			///
			/// \param buffer Memory buffer containing pixel data.
			///
			void load_custom_mem(std::span<unsigned char> buffer);

			///
			/// Set cursor to custom cursor if loaded.
			///
			void use_custom();

			///
			/// OS default pointer.
			///
			void use_pointer();

			///
			/// OS default hand pointer.
			///
			void use_hand();

			///
			/// OS default cross pointer.
			///
			void use_cross();

			///
			/// OS default I-Beam cursor.
			///
			void use_text();

			///
			/// Uses custom cursor if available, otherwise uses default OS pointer.
			///
			void use_custom_else_pointer();

			///
			/// Checks if cursor is within window.
			///
			/// \return True if cursor is within the content area of the window.
			///
			[[nodiscard]] bool within_window() const;

			///
			/// \brief Clean up custom cursor.
			///
			/// Does not destroy default cursors.
			///
			void destroy();

		  private:
			///
			/// Constructor.
			///
			Cursor();

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

			///
			/// Destroy system cursor data.
			///
			void destroy_system_cursors();

		  private:
			///
			/// Custom user cursor.
			///
			GLFWcursor* m_custom;

			///
			/// Default system pointer.
			///
			GLFWcursor* m_pointer;

			///
			/// Default system cross pointer.
			///
			GLFWcursor* m_cross;

			///
			/// Default system hand cursor.
			///
			GLFWcursor* m_hand;

			///
			/// Default system text pointer.
			///
			GLFWcursor* m_text;
		};
	} // namespace input
} // namespace galaxy

#endif
