///
/// Clipboard.hpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#ifndef GALAXY_INPUT_CLIPBOARD_HPP_
#define GALAXY_INPUT_CLIPBOARD_HPP_

#include <string>

#include <raylib.h>

namespace galaxy
{
	namespace input
	{
		///
		/// Access to PC clipboard.
		///
		namespace clipboard
		{
			///
			//// Set clipboard text content.
			///
			/// \param text Text content to set.
			///
			void set(const std::string& text);

			///
			/// Get clipboard text content.
			///
			/// \return Data from clipboard in a string.
			///
			[[nodiscard]]
			std::string get();

			///
			/// Get clipboard content as an image.
			///
			/// \return Image object.
			///
			[[nodiscard]]
			Image get_image();
		} // namespace clipboard
	} // namespace input
} // namespace galaxy

#endif
