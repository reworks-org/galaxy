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
			/// Constructor.
			///
			RMLEvents();

			///
			/// Destructor.
			///
			~RMLEvents();

			///
			/// Set RML context.
			///
			/// \param context Pointer to RML context.
			///
			void set_context(Rml::Context* context);

			///
			/// On mouse moved event handler.
			///
			/// \param e Event to process.
			///
			void on_mouse_move(events::MouseMoved& e);

			///
			/// On mouse pressed event handler.
			///
			/// \param e Event to process.
			///
			void on_mouse_pressed(events::MousePressed& e);

			///
			/// On mouse released event handler.
			///
			/// \param e Event to process.
			///
			void on_mouse_released(events::MouseReleased& e);

			///
			/// On mouse wheel event handler.
			///
			/// \param e Event to process.
			///
			void on_mouse_wheel(events::MouseWheel& e);

			///
			/// On mouse enter event handler.
			///
			/// \param e Event to process.
			///
			void on_mouse_enter(events::MouseEnter& e);

			///
			/// On key down event handler.
			///
			/// \param e Event to process.
			///
			void on_key_down(events::KeyDown& e);

			///
			/// On key repeat event handler.
			///
			/// \param e Event to process.
			///
			void on_key_repeat(events::KeyRepeat& e);

			///
			/// On key up event handler.
			///
			/// \param e Event to process.
			///
			void on_key_up(events::KeyUp& e);

			///
			/// On typing event handler.
			///
			/// \param e Event to process.
			///
			void on_key_char(events::KeyChar& e);

			///
			/// On window resized event handler.
			///
			/// \param e Event to process.
			///
			void on_window_resize(const events::WindowResized& e);

			///
			/// On content scale event handler.
			///
			/// \param e Event to process.
			///
			void on_content_scale(const events::ContentScale& e);

			///
			/// Converts the GLFW key modifiers to RmlUi key modifiers.
			///
			/// \param glfw_mods GLFW modifier.
			///
			/// \return RML modifier.
			///
			[[nodiscard]] int convert_key_modifier(int glfw_mods);

		private:
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