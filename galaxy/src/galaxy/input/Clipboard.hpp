///
/// Clipboard.hpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#ifndef GALAXY_INPUT_CLIPBOARD_HPP_
#define GALAXY_INPUT_CLIPBOARD_HPP_

#include <string>

#include "galaxy/input/InputDevice.hpp"

namespace galaxy
{
	namespace input
	{
		///
		/// Access PC clipboard and set/get contents.
		///
		class Clipboard final : public InputDevice
		{
			friend class core::Window;

		public:
			///
			/// Destructor.
			///
			virtual ~Clipboard() noexcept = default;

			///
			/// Set clipboard contents.
			///
			/// \param contents UTF-8 string.
			///
			void set(const char* contents) const noexcept;

			///
			/// Get clipboard contents.
			///
			/// \return UTF-8 string.
			///
			[[nodiscard]] std::string get() const noexcept;

		private:
			///
			/// Constructor.
			///
			Clipboard() noexcept;

			///
			/// Move constructor.
			///
			Clipboard(Clipboard&&) = delete;

			///
			/// Move assignment operator.
			///
			Clipboard& operator=(Clipboard&&) = delete;

			///
			/// Copy constructor.
			///
			Clipboard(const Clipboard&) = delete;

			///
			/// Copy assignment operator.
			///
			Clipboard& operator=(const Clipboard&) = delete;
		};
	} // namespace input
} // namespace galaxy

#endif