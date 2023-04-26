///
/// Camera.hpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#ifndef GALAXY_GRAPHICS_CAMERA_HPP_
#define GALAXY_GRAPHICS_CAMERA_HPP_

#include <glm/mat4x4.hpp>

#include "galaxy/events/KeyDown.hpp"
#include "galaxy/events/MouseWheel.hpp"
#include "galaxy/events/WindowResized.hpp"
#include "galaxy/fs/Serializable.hpp"
#include "galaxy/utils/Globals.hpp"

namespace galaxy
{
	namespace graphics
	{
		///
		/// Orthographic 2D camera.
		///
		class Camera final : public fs::Serializable
		{
		public:
			///
			/// Camera data.
			///
			struct Data final
			{
				///
				/// Combined transformation matrix.
				///
				alignas(16) glm::mat4 m_model_view = glm::mat4 {GALAXY_IDENTITY_MATRIX};

				///
				/// Camera projection matrix.
				///
				alignas(16) glm::mat4 m_projection = glm::mat4 {GALAXY_IDENTITY_MATRIX};
			};

			///
			/// Argument constructor.
			///
			/// \param allow_rotate Can the camera rotate?
			///
			Camera(bool allow_rotate = false);

			///
			/// JSON constructor.
			///
			/// \param json JSON object.
			///
			Camera(const nlohmann::json& json);

			///
			/// Move constructor.
			///
			Camera(Camera&&);

			///
			/// Move assignment operator.
			///
			Camera& operator=(Camera&&);

			///
			/// Copy constructor.
			///
			Camera(const Camera&);

			///
			/// Copy assignment operator.
			///
			Camera& operator=(const Camera&);

			///
			/// Destructor.
			///
			virtual ~Camera();

			///
			/// Event processing method for input events.
			///
			/// \param e Takes in a key down event.
			///
			void on_key_down(events::KeyDown& e);

			///
			/// Event processing method for scroll events.
			///
			/// \param e Takes in a mouse wheel scroll event.
			///
			void on_mouse_wheel(events::MouseWheel& e);

			///
			/// \brief Set Viewport.
			///
			/// Calls set_projection().
			///
			/// \param width Width of viewport, independant of window size.
			/// \param height Height of viewport, independant of window size.
			///
			void set_viewport(const float width, const float height);

			///
			/// Sets position without moving the entity.
			///
			/// \param x X position to set.
			/// \param y Y position to set.
			///
			void set_pos(const float x, const float y);

			///
			/// Set the entity rotation.
			///
			/// \param degrees Min 0, max 360.
			///
			void set_rotation(const float degrees);

			///
			/// Zoom camera view.
			///
			/// \param offset Camera view scroll offset.
			///
			void set_zoom(const float offset);

			///
			/// Get camera x pos.
			///
			/// \return Float.
			///
			[[nodiscard]] float get_x() const;

			///
			/// Get camera y pos.
			///
			/// \return Float.
			///
			[[nodiscard]] float get_y() const;

			///
			/// Get camera rotation.
			///
			/// \return Float.
			///
			[[nodiscard]] float get_rotation() const;

			///
			/// Get camera zoom.
			///
			/// \return Float.
			///
			[[nodiscard]] float get_zoom() const;

			///
			/// Get camera viewport.
			///
			/// \return glm::vec2 reference.
			///
			[[nodiscard]] const glm::vec2& get_viewport() const;

			///
			/// Retrieve internal transformation matrix.
			///
			/// \return Reference to internal glm::mat4.
			///
			[[nodiscard]] const glm::mat4& get_view();

			///
			/// Get the Camera projection.
			///
			/// \return Const glm::mat4 reference.
			///
			[[nodiscard]] const glm::mat4& get_proj();

			///
			/// Get camera view and proj.
			///
			/// \return Reference to camera data.
			///
			[[nodiscard]] Data& get_data();

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
			/// Constructor.
			///
			Camera() = delete;

			///
			/// Set camera projection.
			///
			/// \param left Left point of ortho perspective.
			/// \param right Right point of ortho perspective.
			/// \param bottom Bottom point of ortho perspective.
			/// \param top Top point of ortho perspective.
			///
			void set_projection(const float left, const float right, const float bottom, const float top);

			///
			/// Recalculates the model view matrix.
			///
			void recalculate();

		public:
			///
			/// Allow camera to rotate.
			///
			bool m_allow_rotate;

			///
			/// Movement speed.
			///
			float m_translation_speed;

			///
			/// Rotational speed.
			///
			float m_rotation_speed;

		private:
			///
			/// Camera position.
			///
			glm::vec3 m_pos;

			///
			/// Camera rotation.
			///
			float m_rotation;

			///
			/// Camera zoom (scroll).
			///
			float m_zoom;

			///
			/// Camera data.
			///
			Data m_data;

			///
			/// Camera origin point.
			/// Cached for reference.
			///
			glm::vec3 m_origin;

			///
			/// Camera viewport size.
			///
			glm::vec2 m_viewport;
		};
	} // namespace graphics
} // namespace galaxy

#endif