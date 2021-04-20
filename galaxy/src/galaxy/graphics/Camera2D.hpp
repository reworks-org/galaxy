///
/// Camera2D.hpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#ifndef GALAXY_GRAPHICS_CAMERA2D_HPP_
#define GALAXY_GRAPHICS_CAMERA2D_HPP_

#include "galaxy/core/Window.hpp"
#include "galaxy/events/KeyUp.hpp"
#include "galaxy/events/KeyDown.hpp"
#include "galaxy/events/MouseWheel.hpp"
#include "galaxy/events/WindowResized.hpp"
#include "galaxy/fs/Serializable.hpp"

namespace galaxy
{
	namespace graphics
	{
		///
		/// Defines a Camera2D (view) of an OpenGL 2D space.
		///
		class Camera2D final : public fs::Serializable
		{
		public:
			///
			/// Constructor.
			///
			Camera2D() noexcept;

			///
			/// JSON constructor.
			///
			/// \param json JSON object.
			///
			Camera2D(const nlohmann::json& json) noexcept;

			///
			/// \brief Argument constructor.
			///
			/// Calls configure() and setSpeed().
			///
			/// \param left Left point of ortho perspective.
			/// \param right Right point of ortho perspective.
			/// \param bottom Bottom point of ortho perspective.
			/// \param top Top point of ortho perspective.
			/// \param speed Speed of the Camera2D. Multiplicative float.
			///
			Camera2D(const float left, const float right, const float bottom, const float top, const float speed) noexcept;

			///
			/// Destructor.
			///
			virtual ~Camera2D() noexcept = default;

			///
			/// Configures Camera2D for window model view projection.
			///
			/// \param left Left point of ortho perspective.
			/// \param right Right point of ortho perspective.
			/// \param bottom Bottom point of ortho perspective.
			/// \param top Top point of ortho perspective.
			///
			void create(const float left, const float right, const float bottom, const float top) noexcept;

			///
			///	Event processing method for key down for Camera2D.
			///
			/// \param e Takes in a shared galaxy event defining a key press down.
			///
			void on_event(const events::KeyDown& e) noexcept;

			///
			/// Event processing method for key up for Camera2D.
			///
			/// \param e Takes in a shared galaxy event defining a key release.
			///
			void on_event(const events::KeyUp& e) noexcept;

			///
			/// Event processing method for scroll event for Camera2D.
			///
			/// \param e Takes in a mouse wheel scroll event.
			///
			void on_event(const events::MouseWheel& e) noexcept;

			///
			/// Event window resizing.
			///
			/// \param e Window resized event.
			///
			void on_event(const events::WindowResized& e) noexcept;

			///
			/// Update method for Camera2D.
			///
			/// \param ts Time-step from update() loop.
			///
			void update(const double ts) noexcept;

			///
			/// \brief Translate (move) position.
			///
			/// Does not set the position but moves it.
			/// By adding the parameters to the existing transformation matrix.
			///
			/// \param x How far to translate on x axis.
			/// \param y How far to translate on y axis.
			///
			void move(const float x, const float y) noexcept;

			///
			/// \brief Translate (move) x pos.
			///
			/// Does not set the position but moves it.
			/// By adding the parameters to the existing transformation matrix.
			///
			/// \param x How far to translate on x axis.
			///
			void move_x(const float x) noexcept;

			///
			/// \brief Translate (move) y pos.
			///
			/// Does not set the position but moves it.
			/// By adding the parameters to the existing transformation matrix.
			///
			/// \param y How far to translate on y axis.
			///
			void move_y(const float y) noexcept;

			///
			/// Zoom Camera2D.
			///
			/// \param scale Scale. Multiplier. Min 0.2.
			///
			void zoom(float scale) noexcept;

			///
			/// Sets position without moving the object.
			///
			/// \param x X position to set object to.
			/// \param y Y position to set object to.
			///
			void set_pos(const float x, const float y) noexcept;

			///
			/// Set x pos.
			///
			/// \param x X position to set object to.
			///
			void set_pos_x(const float x) noexcept;

			///
			/// Set y pos.
			///
			/// \param y Y position to set object to.
			///
			void set_pos_y(const float y) noexcept;

			///
			/// Set the speed of the Camera2D.
			///
			/// \param speed Speed of the Camera2D. Multiplicative float.
			///
			void set_speed(const float speed) noexcept;

			///
			/// Set width of projection.
			///
			/// \param width New width.
			///
			void set_width(const float width) noexcept;

			///
			/// Set height of projection.
			///
			/// \param height New height.
			///
			void set_height(const float height) noexcept;

			///
			/// Get Camera2D speed.
			///
			/// \return Const float.
			///
			[[nodiscard]] const float get_speed() const noexcept;

			///
			/// Get Camera2D width.
			///
			/// \return Const float.
			///
			[[nodiscard]] const float get_width() const noexcept;

			///
			/// Get Camera2D height.
			///
			/// \return Const float.
			///
			[[nodiscard]] const float get_height() const noexcept;

			///
			/// Get flag indicating if transform needs to be applied before rendering.
			///
			/// \return Const boolean.
			///
			[[nodiscard]] const bool is_dirty() const noexcept;

			///
			/// Retrieve internal transformation matrix.
			///
			/// \return Reference to internal glm::mat4.
			///
			[[nodiscard]] const glm::mat4& get_view() noexcept;

			///
			/// Get stored scale cache.
			///
			/// \return Const float.
			///
			[[nodiscard]] const float get_scale() const noexcept;

			///
			/// Get stored pos cache.
			///
			/// \return Const glm::vec2.
			///
			[[nodiscard]] const glm::vec2& get_pos() const noexcept;

			///
			/// Get the Camera2D projection.
			///
			/// \return Const glm::mat4 reference.
			///
			[[nodiscard]] const glm::mat4& get_proj() noexcept;

			///
			/// Serializes object.
			///
			/// \return JSON object containing data to write out.
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
			/// Recalculates the model view matrix.
			///
			void recalculate() noexcept;

		public:
			///
			/// Forward key binding.
			///
			input::Keys m_forward_key = input::Keys::W;

			///
			/// Backward key binding.
			///
			input::Keys m_back_key = input::Keys::S;

			///
			/// Left key binding.
			///
			input::Keys m_left_key = input::Keys::A;

			///
			/// Right key binding.
			///
			input::Keys m_right_key = input::Keys::D;

		private:
			///
			/// Update flag.
			///
			bool m_dirty;

			///
			/// Scaled matrix.
			///
			glm::mat4 m_scaling;

			///
			/// Translation matrix.
			///
			glm::mat4 m_translation;

			///
			/// Identity matrix.
			///
			glm::mat4 m_identity_matrix;

			///
			/// Cached for easiy retrieval.
			/// Scale.
			///
			float m_scale;

			///
			/// Cached for easiy retrieval.
			/// Pos.
			///
			glm::vec2 m_pos;

			///
			/// Combined transformation matrix.
			///
			glm::mat4 m_model;

			///
			/// Camera2D move up flag.
			///
			bool m_move_up;

			///
			/// Camera2D move down flag.
			///
			bool m_move_down;

			///
			/// Camera2D move left flag.
			///
			bool m_move_left;

			///
			/// Camera2D move right flag.
			///
			bool m_move_right;

			///
			/// Speed of Camera2D.
			///
			float m_speed;

			///
			/// Width of Camera2D bounds.
			///
			float m_width;

			///
			/// Height of Camera2D bounds.
			///
			float m_height;

			///
			/// Camera2D projection matrix.
			///
			glm::mat4 m_projection;
		};
	} // namespace graphics
} // namespace galaxy

#endif