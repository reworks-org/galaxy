///
/// Progressbar.hpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#ifndef GALAXY_PROGRESSBAR_HPP_
#define GALAXY_PROGRESSBAR_HPP_

#include <protostar/events/MouseMovedEvent.hpp>
#include <qs/sprite/BatchedSprite.hpp>

#include "galaxy/ui/Widget.hpp"

///
/// Core namespace.
///
namespace galaxy
{
	///
	/// Widget specific namespace.
	///
	namespace widget
	{
		///
		/// UI bar that can be moved to increment and decrement a value.
		///
		class Progressbar final : public galaxy::Widget
		{
			friend class GUI;

		public:
			///
			/// Constructor.
			///
			Progressbar();

			///
			/// Copy constructor.
			///
			Progressbar(const Progressbar&) = delete;

			///
			/// Move constructor.
			///
			Progressbar(Progressbar&&) = default;

			///
			/// Copy assignment operator.
			///
			Progressbar& operator=(const Progressbar&) = delete;

			///
			/// Move assignment operator.
			///
			Progressbar& operator=(Progressbar&&) = default;

			///
			/// Destructor.
			///
			virtual ~Progressbar() = default;

			///
			/// Create container.
			///
			/// \param container Texture of the container in theme to use.
			/// \param bar Texture of the bar in theme to use.
			///
			void create(std::string_view container, std::string_view bar);

			///
			/// Triggered upon mouse movement.
			///
			/// \param mme Mouse Moved Event.
			///
			void on_event(const pr::MouseMovedEvent& mme);

			///
			/// \brief Update widget logic.
			///
			/// YOU MUST NOT CALL ANY GL CODE FROM THIS FUNCTION. THIS FUNCTION IS CALLED FROM A SEPERATE THREAD.
			///
			void update() override;

			///
			/// \brief Render the widget.
			///
			/// THIS FUNCTION IS CALLED ON THE MAIN THREAD. PUT YOUR GL CODE HERE.
			///
			/// \param camera View camera to use when rendering.
			///
			void render(qs::Camera& camera) override;

			///
			/// Sets position without moving the object.
			///
			/// \param x X position to set object to.
			/// \param y Y position to set object to.
			///
			void set_pos(const float x, const float y);

			///
			/// Set current progress of bar.
			///
			/// \param progress Clamped between 0.0f and 1.0f.
			///
			void set_progress(const float progress);

			///
			/// Get current progress of container.
			///
			/// \return const float of progress. 0.0f - 1.0f. I.e. 0.78f is 78%.
			///
			const float progress() const;

			///
			/// Returns current progress of bar in a percentage.
			///
			/// \return const float progress as pertentage.
			///
			const float percentage() const;

		private:
			///
			/// Current progress of bar.
			///
			float m_progress;

			///
			/// Previous progress of bar.
			///
			float m_prev_progress;

			///
			/// Bounds of bar on bar.
			///
			pr::Rect<float> m_bar_pos;

			///
			/// Progressbar texture.
			///
			qs::BatchedSprite m_container;

			///
			/// Bar texture.
			///
			qs::BatchedSprite m_bar;
		};
	} // namespace widget
} // namespace galaxy

#endif