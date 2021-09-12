///
/// Image.hpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#ifndef GALAXY_UI_WIDGETS_IMAGE_HPP_
#define GALAXY_UI_WIDGETS_IMAGE_HPP_

#include "galaxy/components/BatchSprite.hpp"
#include "galaxy/events/MouseMoved.hpp"
#include "galaxy/ui/Widget.hpp"

namespace galaxy
{
	namespace ui
	{
		///
		/// \brief Simple image widget.
		///
		/// ANY FUNCTIONS YOU BIND TO AN EVENT ARE NOT CALLED ON THE MAIN THREAD.
		/// DO NOT CALL GL CODE IN EVENT FUNCTIONS.
		///
		class Image final : public Widget
		{
			friend class GUI;

		public:
			///
			/// Constructor.
			///
			Image() noexcept;

			///
			/// Destructor.
			///
			virtual ~Image() noexcept = default;

			///
			/// Use theme texture from atlas.
			///
			/// \param name Name of the texture to use.
			///
			void create(std::string_view name);

			///
			/// Sets position without moving the object.
			///
			/// \param x X position to set object to.
			/// \param y Y position to set object to.
			///
			void set_pos(const float x, const float y) noexcept;

			///
			/// Triggered when mouse moves.
			///
			/// \param mme Mouse Moved Event.
			///
			void on_event(const events::MouseMoved& mme) noexcept;

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
			void render() override;

			///
			/// Serializes object.
			///
			/// \return JSON object containing data to be serialized.
			///
			[[nodiscard]] nlohmann::json serialize() override;

			///
			/// Deserializes from object.
			///
			/// \param json Json object to retrieve data from.
			///
			void deserialize(const nlohmann::json& json) override;

		private:
			///
			/// Copy constructor.
			///
			Image(const Image&) = delete;

			///
			/// Move constructor.
			///
			Image(Image&&) = delete;

			///
			/// Copy assignment operator.
			///
			Image& operator=(const Image&) = delete;

			///
			/// Move assignment operator.
			///
			Image& operator=(Image&&) = delete;

		private:
			///
			/// Batched sprite.
			///
			components::BatchSprite m_sprite;

			///
			/// Batched Transform.
			///
			components::Transform2D m_transform;
		};
	} // namespace ui
} // namespace galaxy

#endif