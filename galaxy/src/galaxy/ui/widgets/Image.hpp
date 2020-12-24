///
/// Image.hpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#ifndef GALAXY_IMAGE_HPP_
#define GALAXY_IMAGE_HPP_

#include <galaxy/events/MouseMovedEvent.hpp>
#include <qs/sprite/SpriteBatch.hpp>

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
		/// \brief Simple image widget.
		///
		/// ANY FUNCTIONS YOU BIND TO AN EVENT ARE NOT CALLED ON THE MAIN THREAD.
		/// DO NOT CALL GL CODE IN EVENT FUNCTIONS.
		///
		class Image final : public galaxy::Widget, public qs::BatchedSprite
		{
		public:
			///
			/// Constructor.
			///
			Image() = default;

			///
			/// Copy constructor.
			///
			Image(const Image&) = delete;

			///
			/// Move constructor.
			///
			Image(Image&&) = default;

			///
			/// Copy assignment operator.
			///
			Image& operator=(const Image&) = delete;

			///
			/// Move assignment operator.
			///
			Image& operator=(Image&&) = default;

			///
			/// Destructor.
			///
			virtual ~Image() = default;

			///
			/// Use theme texture from atlas.
			///
			/// \param name Name of the texture to use.
			///
			using qs::BatchedSprite::create;
			void create(std::string_view name);

			///
			/// Sets position without moving the object.
			///
			/// \param x X position to set object to.
			/// \param y Y position to set object to.
			///
			void set_pos(const float x, const float y) override;

			///
			/// Triggered when mouse moves.
			///
			/// \param mme Mouse Moved Event.
			///
			void on_event(const pr::MouseMovedEvent& mme);

			///
			/// \brief Update widget logic.
			///
			/// YOU MUST NOT CALL ANY GL CODE FROM THIS FUNCTION. THIS FUNCTION IS CALLED FROM A SEPERATE THREAD.
			///
			/// \param dt Delta Time from gameloop.
			///
			void update(const double dt) override;

			///
			/// \brief Render the widget.
			///
			/// THIS FUNCTION IS CALLED ON THE MAIN THREAD. PUT YOUR GL CODE HERE.
			///
			/// \param camera View camera to use when rendering.
			///
			void render(qs::Camera& camera) override;
		};
	} // namespace widget
} // namespace galaxy

#endif