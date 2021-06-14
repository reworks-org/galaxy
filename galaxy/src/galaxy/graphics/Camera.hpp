///
/// Camera.hpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#ifndef GALAXY_GRAPHICS_CAMERA_CAMERA_HPP_
#define GALAXY_GRAPHICS_CAMERA_CAMERA_HPP_

#include <glm/matrix.hpp>
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>

#include "galaxy/events/KeyDown.hpp"
#include "galaxy/events/KeyUp.hpp"
#include "galaxy/events/MouseMoved.hpp"
#include "galaxy/events/MouseWheel.hpp"
#include "galaxy/events/WindowResized.hpp"

namespace galaxy
{
	namespace graphics
	{
		///
		/// Generic camera class.
		///
		class Camera
		{
		public:
			///
			/// Constructor.
			///
			Camera() noexcept;

			///
			/// Virtual destructor.
			///
			virtual ~Camera() noexcept = default;

			///
			///	On key down function.
			///
			/// \param e Key down event.
			///
			virtual void on_event(const events::KeyDown& e) noexcept = 0;

			///
			///	On key up function.
			///
			/// \param e Key up event.
			///
			virtual void on_event(const events::KeyUp& e) noexcept = 0;

			///
			///	Mouse moved function.
			///
			/// \param e Mouse moved event.
			///
			virtual void on_event(const events::MouseMoved& e) noexcept = 0;

			///
			///	Mouse wheel function.
			///
			/// \param e Mouse wheel event.
			///
			virtual void on_event(const events::MouseWheel& e) noexcept = 0;

			///
			/// Event window resizing.
			///
			/// \param e Window resized event.
			///
			virtual void on_event(const events::WindowResized& e) noexcept = 0;

			///
			/// Update camera based on event input.
			///
			/// \param dt Delta-Time from gameloop.
			///
			virtual void update(const double dt) noexcept = 0;

			///
			/// Get camera view.
			///
			/// \return Const reference to a glm::mat4.
			///
			[[nodiscard]] virtual const glm::mat4& get_view() noexcept = 0;

			///
			/// Get camera projection.
			///
			/// \return Const reference to a glm::mat4.
			///
			[[nodiscard]] virtual const glm::mat4& get_proj() noexcept = 0;

		public:
			///
			/// Forward key binding.
			///
			input::Keys m_forward_key;

			///
			/// Backward key binding.
			///
			input::Keys m_back_key;

			///
			/// Left key binding.
			///
			input::Keys m_left_key;

			///
			/// Right key binding.
			///
			input::Keys m_right_key;

		protected:
			///
			/// Moving forward flag.
			///
			bool m_moving_fwd;

			///
			/// Moving back flag.
			///
			bool m_moving_back;

			///
			/// Moving left flag.
			///
			bool m_moving_left;

			///
			/// Moving right flag.
			///
			bool m_moving_right;

			///
			/// Camera movement speed.
			///
			float m_speed;
		};
	} // namespace graphics
} // namespace galaxy

#endif