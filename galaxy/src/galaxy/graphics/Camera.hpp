///
/// Camera.hpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#ifndef GALAXY_GRAPHICS_CAMERA_HPP_
#define GALAXY_GRAPHICS_CAMERA_HPP_

#include "galaxy/fs/Serializable.hpp"
#include "galaxy/graphics/Transform.hpp"
#include "galaxy/utils/Globals.hpp"

namespace galaxy
{
	namespace input
	{
		class CameraController;
	} // namespace input

	namespace graphics
	{
		///
		/// Orthographic 2D camera.
		///
		class Camera final : public Transform, fs::Serializable
		{
			friend class input::CameraController;

		  public:
			///
			/// Camera data.
			///
			struct alignas(16) Data final
			{
				///
				/// Combined transformation matrix.
				///
				glm::mat4 m_model_view = glm::mat4 {GALAXY_IDENTITY_MATRIX};

				///
				/// Camera projection matrix.
				///
				glm::mat4 m_projection = glm::mat4 {GALAXY_IDENTITY_MATRIX};
			};

			///
			/// Constructor.
			///
			Camera();

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
			/// \brief Set Viewport.
			///
			/// Calls set_projection().
			///
			/// \param width Width of viewport, independant of window size.
			/// \param height Height of viewport, independant of window size.
			///
			void set_viewport(const float width, const float height);

			///
			/// Translate (move) position.
			///
			/// \param x How far to translate on x axis.
			/// \param y How far to translate on y axis.
			///
			void translate(const float x, const float y) override;

			///
			/// Rotate entity.
			///
			/// \param degrees Additive. Min -360, max 360.
			///
			void rotate(const float degrees) override;

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
			[[nodiscard]] const glm::mat4& get_model_view();

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
			bool m_allow_rotation;

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
			/// Camera data.
			///
			Data m_data;

			///
			/// Camera viewport size.
			///
			glm::vec2 m_viewport;
		};
	} // namespace graphics
} // namespace galaxy

#endif
