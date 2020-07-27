///
/// Camera.hpp
/// quasar
///
/// Refer to LICENSE.txt for more details.
///

#ifndef QUASAR_CAMERA_HPP_
#define QUASAR_CAMERA_HPP_

#include <protostar/events/KeyUpEvent.hpp>
#include <protostar/events/KeyDownEvent.hpp>
#include <protostar/events/MouseWheelEvent.hpp>

#include "qs/core/Transform.hpp"

///
/// Core namespace.
///
namespace qs
{
	///
	/// Defines a Camera (view) of an OpenGL 2D space.
	///
	class Camera final : public qs::Transform
	{
	public:
		///
		/// Default constructor.
		///
		Camera() noexcept;

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
		explicit Camera(const float left, const float right, const float bottom, const float top, const pr::positive_float auto speed) noexcept;

		///
		/// Default destructor.
		///
		~Camera() noexcept = default;

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
		/// \param e Takes in a shared protostar event defining a key press down.
		///
		void on_key_down(const pr::KeyDownEvent& e) noexcept;

		///
		/// Event processing method for key up for camera.
		///
		/// \param e Takes in a shared protostar event defining a key release.
		///
		void on_key_up(const pr::KeyUpEvent& e) noexcept;

		///
		/// Event processing method for scroll event for camera.
		///
		/// \param e Takes in a mouse wheel scroll event.
		///
		void on_mouse_scroll(const pr::MouseWheelEvent& e) noexcept;

		///
		/// Update method for camera.
		///
		/// \param ts Time-step from update() loop.
		///
		void update(const double ts) noexcept;

		///
		/// Set the speed of the camera.
		///
		/// \param speed Speed of the camera. Multiplicative float.
		///
		void set_speed(const pr::positive_float auto speed) noexcept;

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
		/// Get the camera projection.
		///
		/// \return Const glm::mat4 reference.
		///
		[[nodiscard]] const glm::mat4& get_proj() noexcept;

	private:
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
} // namespace qs

#endif