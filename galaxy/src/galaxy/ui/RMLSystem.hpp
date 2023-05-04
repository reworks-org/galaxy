///
/// RMLSystem.hpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#ifndef GALAXY_UI_RMLSYSTEM_HPP_
#define GALAXY_UI_RMLSYSTEM_HPP_

#include <RmlUi/Core/SystemInterface.h>

namespace galaxy
{
	namespace ui
	{
		///
		/// Custom RML system interface.
		///
		class RMLSystem final : public Rml::SystemInterface
		{
		public:
			///
			/// Constructor.
			///
			RMLSystem();

			///
			/// Destructor.
			///
			virtual ~RMLSystem();

			///
			/// Get the number of seconds elapsed since the start of the application.
			///
			/// \return Elapsed time, in seconds.
			///
			[[nodiscard]] double GetElapsedTime() override;

			///
			/// Log the specified message.
			///
			/// \param type Type of log message, ERROR, WARNING, etc.
			/// \param message Message to log.
			///
			/// \return True to continue execution, false to break into the debugger.
			///
			[[nodiscard]] bool LogMessage(Rml::Log::Type type, const Rml::String& message) override;

			///
			/// Set mouse cursor.
			///
			/// \param cursor_name Cursor name to activate.
			///
			[[nodiscard]] void SetMouseCursor(const Rml::String& cursor_name) override;

			///
			/// Set clipboard text.
			///
			/// \param text Text to apply to clipboard.
			///
			void SetClipboardText(const Rml::String& text) override;

			///
			/// Get clipboard text.
			///
			/// \param text Retrieved text from clipboard.
			///
			void GetClipboardText(Rml::String& text) override;
		};
	} // namespace ui
} // namespace galaxy

#endif