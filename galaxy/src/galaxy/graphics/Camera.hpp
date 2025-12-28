///
/// Camera.hpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#ifndef GALAXY_GRAPHICS_CAMERA_HPP_
#define GALAXY_GRAPHICS_CAMERA_HPP_

#include <glm/mat4x4.hpp>
#include <glm/vec2.hpp>

namespace galaxy
{
	///
	/// Orthographic 2D camera.
	///
	class Camera final
	{
	public:
		///
		/// Camera data.
		///
		struct alignas(16) Data final
		{
			///
			/// Combined transformation matrix.
			///
			glm::mat4 m_model_view = glm::mat4 {1.0f};

			///
			/// Camera projection matrix.
			///
			glm::mat4 m_projection = glm::mat4 {1.0f};
		};

		///
		/// Constructor.
		///
		Camera() noexcept;

		///
		/// Move constructor.
		///
		Camera(Camera&&) noexcept;

		///
		/// Move assignment operator.
		///
		Camera& operator=(Camera&&) noexcept;

		///
		/// Copy constructor.
		///
		Camera(const Camera&) noexcept;

		///
		/// Copy assignment operator.
		///
		Camera& operator=(const Camera&) noexcept;

		///
		/// Destructor.
		///
		~Camera() noexcept;

		///
		/// \brief Set camera projection.
		///
		/// By default sets origin to center of right, bottom.
		///
		/// \param left Left point of ortho perspective.
		/// \param right Right point of ortho perspective.
		/// \param bottom Bottom point of ortho perspective.
		/// \param top Top point of ortho perspective.
		///
		void set_projection(const float left, const float right, const float bottom, const float top) noexcept;

		///
		/// Move position.
		///
		/// \param x How far to translate on x axis.
		/// \param y How far to translate on y axis.
		///
		void move(const float x, const float y) noexcept;

		///
		/// Move on x axis.
		///
		/// \param x How far to translate on x axis.
		///
		void move_x(const float x) noexcept;

		///
		/// Move on y axis.
		///
		/// \param y How far to translate on y axis.
		///
		void move_y(const float y) noexcept;

		///
		/// Rotate entity.
		///
		/// \param degrees Additive. Min 0, max 360.
		///
		void rotate(const float degrees) noexcept;

		///
		/// Set entity scale.
		///
		/// \param scale Value to scale by.
		///
		void scale(const float scale) noexcept;

		///
		/// Set entity scale.
		///
		/// \param x Value to scale horizontal axis by.
		///
		void set_scale_horizontal(const float x) noexcept;

		///
		/// Set entity scale.
		///
		/// \param y Value to scale vertical axis by.
		///
		void set_scale_vertical(const float y) noexcept;

		///
		/// Set camera rotation.
		///
		/// \param degrees Min 0, max 360.
		///
		void set_rotation(const float degrees) noexcept;

		///
		/// Set postion of camera.
		///
		/// \param x X axis.
		/// \param y Y axis.
		///
		void set_positon(const float x, const float y) noexcept;

		///
		/// Set postion of camera.
		///
		/// \param x X axis.
		///
		void set_positon_horizontal(const float x) noexcept;

		///
		/// Set postion of camera.
		///
		/// \param y Y axis.
		///
		void set_positon_vertical(const float y) noexcept;

		///
		/// \brief Set the origin point.
		///
		/// Only affects rotation and scale.
		///
		/// \param x X position to set origin to.
		/// \param y Y position to set origin to.
		///
		void set_origin(const float x, const float y) noexcept;

		///
		/// Reset transform.
		///
		void reset() noexcept;

		///
		/// Get stored position.
		///
		/// \return Const glm::vec2 reference.
		///
		[[nodiscard]]
		const glm::vec2& get_pos() const noexcept;

		///
		/// Get stored rotation.
		///
		/// \return Float.
		///
		[[nodiscard]]
		float get_rotation() const noexcept;

		///
		/// Get stored scale.
		///
		/// \return Const glm::vec2 reference.
		///
		[[nodiscard]]
		const glm::vec2& get_scale() const noexcept;

		///
		/// Get origin point.
		///
		/// \return Const glm::vec2 reference.
		///
		[[nodiscard]]
		const glm::vec2& get_origin() const noexcept;

		///
		/// Retrieve internal transformation matrix.
		///
		/// \return glm::mat4 reference.
		///
		[[nodiscard]]
		glm::mat4& get_transform() noexcept;

		///
		/// Retrieve internal transformation matrix.
		///
		/// \return Reference to internal glm::mat4.
		///
		[[nodiscard]]
		const glm::mat4& get_model_view() noexcept;

		///
		/// Get the Camera projection.
		///
		/// \return Const glm::mat4 reference.
		///
		[[nodiscard]]
		const glm::mat4& get_proj() noexcept;

		///
		/// Get camera view and proj.
		///
		/// \return Reference to camera data.
		///
		[[nodiscard]]
		Data& get_data() noexcept;

	private:
		///
		/// Recalculates the model view matrix.
		///
		void recalculate() noexcept;

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
		/// Cached for easy retrieval.
		/// Pos.
		///
		glm::vec2 m_pos;

		///
		/// Cached for easy retrieval.
		/// Rotation.
		///
		float m_rotation;

		///
		/// Cached for easy retrieval.
		/// Scale.
		///
		glm::vec2 m_scale;

		///
		/// Transform origin point.
		///
		glm::vec2 m_origin;

		///
		/// Flag to see if transform needs to be recalculated.
		///
		bool m_dirty;

		///
		/// Combined transform.
		///
		glm::mat4 m_transform;
	};
} // namespace galaxy

#endif
