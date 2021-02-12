///
/// Camera.hpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#ifndef GALAXY_GRAPHICS_CAMERA_HPP_
#define GALAXY_GRAPHICS_CAMERA_HPP_

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
		/// Defines a Camera (view) of an OpenGL 2D space.
		///
		class Camera final : public fs::Serializable
		{
		public:
			///
			/// Constructor.
			///
			Camera() noexcept;

			///
			/// JSON constructor.
			///
			/// \param json JSON object.
			///
			Camera(const nlohmann::json& json) noexcept;

			///
			/// \brief Argument constructor.
			///
			/// Calls configure() and setSpeed().
			///
			/// \param left Left point of ortho perspective.
			/// \param right Right point of ortho perspective.
			/// \param bottom Bottom point of ortho perspective.
			/// \param top Top point of ortho perspective.
			/// \param speed Speed of the camera. Multiplicative float.
			///
			Camera(const float left, const float right, const float bottom, const float top, const float speed) noexcept;

			///
			/// Destructor.
			///
			virtual ~Camera() noexcept = default;

			///
			/// Configures camera for window model view projection.
			///
			/// \param left Left point of ortho perspective.
			/// \param right Right point of ortho perspective.
			/// \param bottom Bottom point of ortho perspective.
			/// \param top Top point of ortho perspective.
			///
			void create(const float left, const float right, const float bottom, const float top) noexcept;

			///
			///	Event processing method for key down for camera.
			///
			/// \param e Takes in a shared galaxy event defining a key press down.
			///
			void on_key_down(const events::KeyDown& e) noexcept;

			///
			/// Event processing method for key up for camera.
			///
			/// \param e Takes in a shared galaxy event defining a key release.
			///
			void on_key_up(const events::KeyUp& e) noexcept;

			///
			/// Event processing method for scroll event for camera.
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
			/// Update method for camera.
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
			/// \param y How far to translate on x axis.
			///
			void move(const float x, const float y) noexcept;

			///
			/// Zoom camera.
			///
			/// \param scale Scale. Multiplier.
			///
			void zoom(const float scale) noexcept;

			///
			/// Recalculates the model view matrix.
			///
			void recalculate() noexcept;

			///
			/// Sets position without moving the object.
			///
			/// \param x X position to set object to.
			/// \param y Y position to set object to.
			///
			void set_pos(const float x, const float y) noexcept;

			///
			/// Set the speed of the camera.
			///
			/// \param speed Speed of the camera. Multiplicative float.
			///
			void set_speed(const float speed) noexcept;

			///
			/// Get camera speed.
			///
			/// \return Const float.
			///
			[[nodiscard]] const float get_speed() const noexcept;

			///
			/// Get camera width.
			///
			/// \return Const float.
			///
			[[nodiscard]] const float get_width() const noexcept;

			///
			/// Get camera height.
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
			[[nodiscard]] const glm::mat4& get_transform() noexcept;

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
			/// Get the camera projection.
			///
			/// \return Const glm::mat4 reference.
			///
			[[nodiscard]] const glm::mat4& get_proj() noexcept;

			///
			/// Serializes object.
			///
			/// \param j Json object needing to be filled out.
			///
			[[nodiscard]] nlohmann::json serialize() override;

			///
			/// Deserializes from object.
			///
			/// \param j Json object to retrieve data from.
			///
			void deserialize(const nlohmann::json& json) override;

		private:
			///
			/// Update flag.
			///
			bool m_dirty;

			///
			/// Rotation origin point.
			///
			glm::vec3 m_origin;

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
			/// Camera move up flag.
			///
			bool m_move_up;

			///
			/// Camera move down flag.
			///
			bool m_move_down;

			///
			/// Camera move left flag.
			///
			bool m_move_left;

			///
			/// Camera move right flag.
			///
			bool m_move_right;

			///
			/// Speed of camera.
			///
			float m_speed;

			///
			/// Width of camera bounds.
			///
			float m_width;

			///
			/// Height of camera bounds.
			///
			float m_height;

			///
			/// Camera projection matrix.
			///
			glm::mat4 m_projection;
		};
	} // namespace graphics
} // namespace galaxy

#endif