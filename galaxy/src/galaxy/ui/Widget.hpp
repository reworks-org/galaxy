///
/// Widget.hpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#ifndef GALAXY_UI_WIDGET_HPP_
#define GALAXY_UI_WIDGET_HPP_

#include "galaxy/audio/Sound.hpp"
#include "galaxy/components/Transform.hpp"
#include "galaxy/ui/Tooltip.hpp"

namespace galaxy
{
	namespace ui
	{
		///
		/// \brief Interactable UI object.
		///
		/// ANY FUNCTIONS YOU BIND TO AN EVENT ARE NOT CALLED ON THE MAIN THREAD.
		/// DO NOT CALL GL CODE IN EVENT FUNCTIONS.
		///
		class Widget
		{
			friend class GUI;

		public:
			///
			/// Move constructor.
			///
			Widget(Widget&&) noexcept;

			///
			/// Move assignment operator.
			///
			Widget& operator=(Widget&&) noexcept;

			///
			/// Destructor.
			///
			virtual ~Widget();

			///
			/// \brief Update widget logic.
			///
			/// YOU MUST NOT CALL ANY GL CODE FROM THIS FUNCTION. THIS FUNCTION IS CALLED FROM A SEPERATE THREAD.
			///
			/// \param dt Delta Time from gameloop.
			///
			virtual void update(const double dt) = 0;

			///
			/// \brief Render the widget.
			///
			/// THIS FUNCTION IS CALLED ON THE MAIN THREAD. PUT YOUR GL CODE HERE.
			///
			virtual void render() = 0;

			///
			/// Add a sound effect to this widget when clicked.
			///
			/// \param sound Sound to load from the soundbook resource manager.
			///
			void add_sfx(std::string_view sound);

			///
			/// \brief Get ID of this widget.
			///
			/// 0 is reserved for NULL.
			///
			/// \return const unsigned int ID value.
			///
			[[nodiscard]] const unsigned int id() const noexcept;

		protected:
			///
			/// Constructor.
			///
			Widget() noexcept;

		private:
			///
			/// Copy constructor.
			///
			Widget(const Widget&) = delete;

			///
			/// Copy assignment operator.
			///
			Widget& operator=(const Widget&) = delete;

		protected:
			///
			/// Pointer to GUI theme.
			///
			Theme* m_theme;

			///
			/// Tooltip. Optional.
			///
			std::unique_ptr<Tooltip> m_tooltip;

			///
			/// Bounds of widget.
			///
			graphics::fRect m_bounds;

			///
			/// Sound effect to play on clicking of UI. Optional.
			///
			audio::Sound* m_sound;

		private:
			///
			/// Widget ID.
			///
			unsigned int m_id;
		};

		///
		/// Shorthand.
		///
		using WidgetPtr = std::unique_ptr<Widget>;
	} // namespace ui
} // namespace galaxy

#endif