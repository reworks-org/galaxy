///
/// Camera3D.hpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#ifndef GALAXY_GRAPHICS_CAMERA3D_HPP_
#define GALAXY_GRAPHICS_CAMERA3D_HPP_

#include <glm/vec3.hpp>
#include <glm/matrix.hpp>
#include <nlohmann/json_fwd.hpp>

#include "galaxy/events/KeyDown.hpp"
#include "galaxy/events/MouseMoved.hpp"
#include "galaxy/events/MouseWheel.hpp"
#include "galaxy/fs/Serializable.hpp"

namespace galaxy
{
	namespace graphics
	{
		///
		/// 3D view of an OpenGL scene.
		///
		class Camera3D final : public fs::Serializable
		{
		public:
			///
			/// Constructor.
			///
			Camera3D();

			///
			/// JSON constructor.
			///
			/// \param json JSON object with camera data.
			///
			Camera3D(const nlohmann::json& json);

			///
			/// Virtual destructor.
			///
			virtual ~Camera3D() noexcept;

			///
			///	On key down function.
			///
			/// \param e Key down event.
			///
			void on_event(const events::KeyDown& e) noexcept;

			///
			///	Mouse moved function.
			///
			/// \param e Mouse moved event.
			///
			void on_event(const events::MouseMoved& e) noexcept;

			///
			///	Mouse wheel function.
			///
			/// \param e Mouse wheel event.
			///
			void on_event(const events::MouseWheel& e) noexcept;

			///
			/// Update camera based on event input.
			///
			/// \param dt Delta-Time from gameloop.
			///
			void update(const double dt) noexcept;

			///
			/// Reset camera data.
			///
			void reset() noexcept;

			///
			/// Get camera view.
			///
			/// \return Const reference to a glm::mat4.
			///
			[[nodiscard]] const glm::mat4& get_view_matrix() noexcept;

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
			/// Recalculate camera vectors.
			///
			void recalculate() noexcept;

		public:
			input::Keys m_forward;
			input::Keys m_back;
			input::Keys m_left;
			input::Keys m_right;

			float m_speed;

		private:
			bool m_dirty_vectors;
			bool m_dirty_view;

			bool m_moving_fwd;
			bool m_moving_back;
			bool m_moving_left;
			bool m_moving_right;

			glm::vec3 m_pos;
			glm::vec3 m_front;
			glm::vec3 m_up;
			glm::vec3 m_right_axis;
			glm::vec3 m_world_up;

			float m_yaw;
			float m_pitch;
			float m_zoom;

			float m_sensitivity;
			glm::mat4 m_view;
		};
	} // namespace graphics
} // namespace galaxy

#endif