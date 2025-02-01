///
/// Clipboard.hpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#ifndef GALAXY_INPUT_CLIPBOARD_HPP_
#define GALAXY_INPUT_CLIPBOARD_HPP_

#include "galaxy/graphics/Image.hpp"

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
			void set(const std::string& text) noexcept;

			///
			/// Get clipboard text content.
			///
			/// \return Data from clipboard in a string.
			///
			[[nodiscard]]
			std::string get() noexcept;

			///
			/// Get clipboard content as an image.
			///
			/// \return Image object.
			///
			[[nodiscard]]
			graphics::Image get_image() noexcept;
		} // namespace clipboard
	} // namespace input
} // namespace galaxy

#endif
