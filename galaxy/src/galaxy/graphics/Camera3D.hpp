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
#include "galaxy/events/KeyUp.hpp"
#include "galaxy/events/MouseMoved.hpp"
#include "galaxy/events/MouseWheel.hpp"
#include "galaxy/events/WindowResized.hpp"
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
			/// Type of camera.
			///
			enum class Mode : short
			{
				///
				/// Perform calculations expecting this is a first person view.
				///
				FIRST,

				///
				/// Perform calculations expecting this is a third person view.
				///
				THIRD,

				///
				/// Perform calculations expecting this is a free (fly/maya) style view.
				///
				FREE
			};

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
			virtual ~Camera3D() noexcept = default;

			///
			/// Set camera mode.
			///
			/// \param mode New mode of camera.
			///
			void set_mode(const Mode mode) noexcept;

			///
			/// Set position of camera.
			///
			/// \param pos New position of camera.
			///
			void set_position(const glm::vec3& pos) noexcept;

			///
			/// Set focus point of camera.
			///
			/// \param look_at Set point the camera is looking at.
			///
			void set_focal(const glm::vec3& look_at) noexcept;

			///
			/// Set field of view of camera.
			///
			/// \param fov New FOV of camera.
			///
			void set_fov(const float fov) noexcept;

			///
			/// Set near clipping distance.
			///
			/// \param near_clip Near clip.
			///
			void set_near(const float near_clip) noexcept;

			///
			/// Set far clipping distance.
			///
			/// \param far_clip Far clip.
			///
			void set_far(const float far_clip) noexcept;

			///
			/// Set camera speed.
			///
			/// \param speed New camera speed. Multiplier.
			///
			void set_speed(const float speed) noexcept;

			///
			/// Set pitch (up/down) of camera.
			///
			/// \param degrees New pitch in degrees.
			///
			void pitch(float degrees) noexcept;

			///
			/// Set heading (left/right) of camera.
			///
			/// \param degrees New heading in degrees.
			///
			void heading(float degrees) noexcept;

			///
			///	On key down function.
			///
			/// \param e Key down event.
			///
			void on_event(const events::KeyDown& e) noexcept;

			///
			///	On key up function.
			///
			/// \param e Key up event.
			///
			void on_event(const events::KeyUp& e) noexcept;

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
			/// Event window resizing.
			///
			/// \param e Window resized event.
			///
			void on_event(const events::WindowResized& e) noexcept;

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
			[[nodiscard]] const glm::mat4& get_view() const noexcept;

			///
			/// Get camera projection.
			///
			/// \return Const reference to a glm::mat4.
			///
			[[nodiscard]] const glm::mat4& get_proj() const noexcept;

			///
			/// Get camera position.
			///
			/// \return Const reference to a glm::vec3.
			///
			[[nodiscard]] const glm::vec3& get_pos() const noexcept;

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

		public:
			input::Keys m_forward_key = input::Keys::W;
			input::Keys m_back_key    = input::Keys::S;
			input::Keys m_left_key    = input::Keys::A;
			input::Keys m_right_key   = input::Keys::D;
			input::Keys m_up_key      = input::Keys::Q;
			input::Keys m_down_key    = input::Keys::E;

		private:
			Mode m_mode = Mode::FREE;

			bool m_moving_fwd   = false;
			bool m_moving_back  = false;
			bool m_moving_left  = false;
			bool m_moving_right = false;

			float m_aspect_ratio;
			float m_fov  = 45.0f;
			float m_near = 0.1f;
			float m_far  = 100.0f;

			float m_heading          = 0.0f;
			float m_pitch            = 0.0f;
			float m_max_pitch_rate   = 5.0f;
			float m_max_heading_rate = 5.0f;

			glm::vec3 m_pos;
			glm::vec3 m_delta;
			glm::vec3 m_focal;
			glm::vec3 m_dir;
			glm::vec3 m_up;
			glm::vec3 m_mouse_pos;

			float m_speed = 1.0f;
			float m_sensitivity;

			glm::mat4 m_view;
			glm::mat4 m_proj;
		};
	} // namespace graphics
} // namespace galaxy

#endif