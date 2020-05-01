///
/// Transform.hpp
/// quasar
///
/// Apache 2.0 LICENSE.
/// Refer to LICENSE.txt for more details.
///

#ifndef QUASAR_TRANSFORM_HPP_
#define QUASAR_TRANSFORM_HPP_

#include <glm/gtc/matrix_transform.hpp>

///
/// Core namespace.
///
namespace qs
{
	///
	/// Defines a transformation of an OpenGL object.
	///
	class Transform
	{
	public:
		///
		/// Constructor.
		///
		Transform() noexcept;

		///
		/// Default virtual destructor.
		///
		virtual ~Transform() noexcept = default;

		///
		/// \brief Translate (move) position.
		///
		/// Does not set the position but moves it.
		/// By adding the parameters to the existing transformation matrix.
		///
		/// \param x How far to translate on x axis.
		/// \param y How far to translate on x axis.
		///
		void move(const float x, const float y) noexcept;

		///
		/// Sets position without moving the object.
		///
		/// \param x X position to set object to.
		/// \param y Y position to set object to.
		///
		void setPos(const float x, const float y) noexcept;

		///
		/// Set the rotation point.
		///
		void setRotationOrigin(const float x, const float y) noexcept;

		///
		/// Rotate transformation in degrees.
		///
		/// \param degrees Max 360, min -360.
		///
		void rotate(const float degrees) noexcept;

		///
		/// Scale transformation on each axis.
		///
		/// \param scale Scale. Multiplier.
		///
		void scale(const float scale) noexcept;

		///
		/// Recalculates the model view matrix.
		///
		virtual void recalculate() noexcept;

		///
		/// Set dirty flag.
		///
		/// \param dirty When rendering, do transforms need to be updated.
		///
		void setDirty(const bool isDirty) noexcept;

		///
		/// Set opacity.
		///
		/// Only affects the currently active quad.
		///
		/// \param opacity Opacity range is from 0.0f (transparent) to 1.0f (opaque).
		///
		void setOpacity(float opacity) noexcept;

		///
		/// \brief Sets the texture coords.
		///
		/// This sets, and thus overrides current position.
		///
		/// \param x New x position.
		/// \param y New y position.
		///
		void setTexels(const float x, const float y) noexcept;

		///
		/// \brief Moves the texture coords.
		///
		/// This is additive. This does not set.
		///
		/// \param x Amount to move on x axis.
		/// \param y Amount to move on y axis.
		///
		void moveTexels(const float x, const float y) noexcept;

		///
		/// Retrieve internal transformation matrix.
		///
		/// \return Reference to internal glm::mat4.
		///
		glm::mat4& getTransformation() noexcept;
		
		///
		/// Retrieve texture transform.
		///
		/// \return Reference to a glm::vec3. X/Y is U/V and Z is opacity.
		///
		glm::vec3& getTexelTransform() noexcept;

		///
		/// Get flag indicating if transform needs to be applied before rendering.
		///
		const bool isDirty() const noexcept;

	protected:
		///
		/// When rendering, do transforms need to be updated.
		///
		bool m_isDirty;

		///
		/// Texture transformation.
		///
		glm::vec3 m_textureTransform;

		///
		/// Rotation origin point.
		///
		glm::vec3 m_originPoint;

		///
		/// Rotational matrix.
		///
		glm::mat4 m_rotateMatrix;

		///
		/// Scaled matrix.
		///
		glm::mat4 m_scaledMatrix;

		///
		/// Translation matrix.
		///
		glm::mat4 m_translationMatrix;

		///
		/// Combined transformation matrix.
		///
		glm::mat4 m_modelMatrix;
	};
}

#endif