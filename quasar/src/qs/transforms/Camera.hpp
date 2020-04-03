///
/// Camera.hpp
/// quasar
///
/// Apache 2.0 LICENSE.
/// Refer to LICENSE.txt for more details.
///

#ifndef QUASAR_CAMERA_HPP_
#define QUASAR_CAMERA_HPP_

#include <protostar/events/KeyDownEvent.hpp>

#include "qs/transforms/Transform.hpp"

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
		explicit Camera(const float left, const float right, const float bottom, const float top, const float speed) noexcept;

		///
		/// Default destructor.
		///
		~Camera() noexcept override = default;

		///
		/// Configures camera for window model view projection.
		///
		/// \param left Left point of ortho perspective.
		/// \param right Right point of ortho perspective.
		/// \param bottom Bottom point of ortho perspective.
		/// \param top Top point of ortho perspective.
		///
		void configure(const float left, const float right, const float bottom, const float top) noexcept;

		///
		///	Event processing method for camera.
		///
		/// \param e Takes in a shared protostar event defining a key press down.
		///
		void onKeyDown(const protostar::KeyDownEvent& e);

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
		void setSpeed(const float speed) noexcept;

		///
		/// Get the camera view-projection.
		///
		/// \return Const glm::mat4 reference.
		///
		const glm::mat4& get() const noexcept;

	private:
		///
		/// Current keydown event.
		///
		protostar::KeyDownEvent m_curKeyDownEvent;

		///
		/// Speed of camera.
		///
		float m_speed;

		///
		/// Camera projection matrix.
		///
		glm::mat4 m_projection;

		///
		/// Camera view-projection matrix.
		///
		glm::mat4 m_vp;

	};
}

#endif