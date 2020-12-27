///
/// Camera.hpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#ifndef GALAXY_GRAPHICS_CAMERA_HPP_
#define GALAXY_GRAPHICS_CAMERA_HPP_

#include "galaxy/events/KeyUp.hpp"
#include "galaxy/events/KeyDown.hpp"
#include "galaxy/events/MouseWheel.hpp"
#include "galaxy/graphics/Transform.hpp"

namespace galaxy
{
	namespace graphics
	{
		///
		/// Defines a Camera (view) of an OpenGL 2D space.
		///
		class Camera final : public Transform
		{
		public:
			///
			/// Default constructor.
			///
			Camera();

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
			explicit Camera(const float left, const float right, const float bottom, const float top, const float speed);

			///
			/// Default destructor.
			///
			virtual ~Camera() = default;

			///
			/// Configures camera for window model view projection.
			///
			/// \param left Left point of ortho perspective.
			/// \param right Right point of ortho perspective.
			/// \param bottom Bottom point of ortho perspective.
			/// \param top Top point of ortho perspective.
			///
			void create(const float left, const float right, const float bottom, const float top);

			///
			///	Event processing method for key down for camera.
			///
			/// \param e Takes in a shared galaxy event defining a key press down.
			///
			void on_key_down(const events::KeyDown& e);

			///
			/// Event processing method for key up for camera.
			///
			/// \param e Takes in a shared galaxy event defining a key release.
			///
			void on_key_up(const events::KeyUp& e);

			///
			/// Event processing method for scroll event for camera.
			///
			/// \param e Takes in a mouse wheel scroll event.
			///
			void on_mouse_scroll(const events::MouseWheel& e);

			///
			/// Update method for camera.
			///
			/// \param ts Time-step from update() loop.
			///
			void update(const double ts);

			///
			/// Set the speed of the camera.
			///
			/// \param speed Speed of the camera. Multiplicative float.
			///
			void set_speed(const float speed);

			///
			/// Get camera width.
			///
			/// \return Const float.
			///
			[[nodiscard]] const float get_width() const;

			///
			/// Get camera height.
			///
			/// \return Const float.
			///
			[[nodiscard]] const float get_height() const;

			///
			/// Get the camera projection.
			///
			/// \return Const glm::mat4 reference.
			///
			[[nodiscard]] const glm::mat4& get_proj();

		private:
			///
			/// Mouse scale.
			///
			double m_scale;

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