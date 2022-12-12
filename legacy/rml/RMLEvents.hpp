///
/// RMLEvents.hpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#ifndef GALAXY_UI_RMLEVENTS_HPP_
#define GALAXY_UI_RMLEVENTS_HPP_

#include "galaxy/events/EventQueue.hpp"
#include "galaxy/ui/RMLInput.hpp"

namespace Rml
{
	class Context;
} // namespace Rml

namespace galaxy
{
	namespace ui
	{
		///
		/// Class to assist in handling events between galaxy and Rml.
		///
		class RMLEvents final
		{
		public:
			///
			/// Argument constructor.
			///
			/// \param context Pointer to RML context.
			///
			RMLEvents(Rml::Context* context) noexcept;

			///
			/// Destructor.
			///
			~RMLEvents() noexcept;

			///
			/// On mouse moved event handler.
			///
			/// \param e Event to process.
			///
			void on_mouse_move(events::MouseMoved& e) noexcept;

			///
			/// On mouse pressed event handler.
			///
			/// \param e Event to process.
			///
			void on_mouse_pressed(events::MousePressed& e) noexcept;

			///
			/// On mouse released event handler.
			///
			/// \param e Event to process.
			///
			void on_mouse_released(events::MouseReleased& e) noexcept;

			///
			/// On mouse wheel event handler.
			///
			/// \param e Event to process.
			///
			void on_mouse_wheel(events::MouseWheel& e) noexcept;

			///
			/// On key down event handler.
			///
			/// \param e Event to process.
			///
			void on_key_down(events::KeyDown& e) noexcept;

			///
			/// On key repeat event handler.
			///
			/// \param e Event to process.
			///
			void on_key_repeat(events::KeyRepeat& e) noexcept;

			///
			/// On key up event handler.
			///
			/// \param e Event to process.
			///
			void on_key_up(events::KeyUp& e) noexcept;

			///
			/// On typing event handler.
			///
			/// \param e Event to process.
			///
			void on_key_char(events::KeyChar& e) noexcept;

			///
			/// On window resized event handler.
			///
			/// \param e Event to process.
			///
			void on_window_resize(const events::WindowResized& e) noexcept;

			///
			/// On content scale event handler.
			///
			/// \param e Event to process.
			///
			void on_content_scale(const events::ContentScale& e) noexcept;

			///
			/// Converts the GLFW key modifiers to RmlUi key modifiers.
			///
			/// \param glfw_mods GLFW modifier.
			///
			/// \return RML modifier.
			///
			[[nodiscard]] int convert_key_modifier(int glfw_mods) noexcept;

		private:
			///
			/// Constructor.
			///
			RMLEvents() = delete;

			///
			/// Move constructor.
			///
			RMLEvents(RMLEvents&&) = delete;

			///
			/// Move assignment operator.
			///
			RMLEvents& operator=(RMLEvents&&) = delete;

			///
			/// Copy constructor.
			///
			RMLEvents(const RMLEvents&) = delete;

			///
			/// Copy assignment operator.
			///
			RMLEvents& operator=(const RMLEvents&) = delete;

		private:
			///
			/// RML context.
			///
			Rml::Context* m_context;
		};
	} // namespace ui
} // namespace galaxy

#endif