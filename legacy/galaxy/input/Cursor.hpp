///
/// Cursor.hpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#ifndef GALAXY_INPUT_CURSOR_HPP_
#define GALAXY_INPUT_CURSOR_HPP_

#include <glm/vec2.hpp>

struct GLFWcursor;

namespace galaxy
{
	namespace input
	{
		///
		/// Implementation of the mouse cursor and state management.
		///
		class Cursor final
		{
		public:
			///
			/// Constructor.
			///
			Cursor() noexcept;

			///
			/// Move constructor.
			///
			Cursor(Cursor&&) noexcept;

			///
			/// Move assignment operator.
			///
			Cursor& operator=(Cursor&&) noexcept;

			///
			/// Destructor.
			///
			~Cursor() noexcept;

			///
			/// Destroy cursor.
			///
			void destroy();

		private:
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

			///
			/// Stores current position.
			///
			glm::dvec2 m_pos;

			///
			/// Stores previous position.
			///
			glm::dvec2 m_prev_pos;

			///
			/// Cursor size.
			///
			glm::vec2 m_cursor_size;
		};
	} // namespace input
} // namespace galaxy

#endif