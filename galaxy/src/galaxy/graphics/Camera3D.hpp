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
			/// GLSL compatible data structure.
			///
			struct Data final
			{
				///
				/// Camera view matrix.
				///
				alignas(16) glm::mat4 m_view;

				///
				/// Camera projection matrix.
				///
				alignas(16) glm::mat4 m_proj;

				///
				/// Camera world position.
				///
				alignas(16) glm::vec3 m_pos;
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
			/// Move position of camera.
			///
			/// \param move Additional movement of camera.
			///
			void move_position(const glm::vec3& move) noexcept;

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
			/// Retrieve GLSL compatible data struct.
			///
			/// \return Properly aligned const reference to a struct.
			///
			[[nodiscard]] const Camera3D::Data& get_data() const noexcept;

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

			///
			/// "Free" camera up key.
			///
			input::Keys m_up_key = input::Keys::Q;

			///
			/// "Free" camera down key.
			///
			input::Keys m_down_key = input::Keys::E;

		private:
			///
			/// Camera mode. Affects calculations to simulate camera movement style.
			///
			Mode m_mode = Mode::FREE;

			///
			/// Moving forward flag.
			///
			bool m_moving_fwd = false;

			///
			/// Moving back flag.
			///
			bool m_moving_back = false;

			///
			/// Moving left flag.
			///
			bool m_moving_left = false;

			///
			/// Moving right flag.
			///
			bool m_moving_right = false;

			///
			/// Moving up flag.
			///
			bool m_moving_up = false;

			///
			/// Moving down flag.
			///
			bool m_moving_down = false;

			///
			/// Screen aspect ratio.
			///
			float m_aspect_ratio;

			///
			/// Camera FOV.
			///
			float m_fov = 45.0f;

			///
			/// Camera near plane.
			///
			float m_near = 0.1f;

			///
			/// Camera far plane.
			///
			float m_far = 100.0f;

			///
			/// Camera heading.
			///
			float m_heading = 0.0f;

			///
			/// Camera pitch.
			///
			float m_pitch = 0.0f;

			///
			/// Camera delta calculation.
			///
			glm::vec3 m_delta;

			///
			/// Camera focal point.
			///
			glm::vec3 m_focal;

			///
			/// Camera view direction.
			///
			glm::vec3 m_dir;

			///
			/// Camera up vector.
			///
			glm::vec3 m_up;

			///
			/// Camera mouse position.
			///
			glm::vec3 m_mouse_pos;

			///
			/// Camera movement speed.
			///
			float m_speed;

			///
			/// Camera mouse movement speed.
			///
			float m_sensitivity;

			///
			/// Camera GLSL data.
			///
			Data m_data;
		};
	} // namespace graphics
} // namespace galaxy

#endif