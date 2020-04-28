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
		/// Retrieve internal transformation matrix.
		///
		/// \return Reference to internal glm::mat4.
		///
		glm::mat4& getTransformation() noexcept;

		///
		/// Gets OpenGL compatible pointer to matrix.
		///
		/// \return OpenGL compatible mat4* pointer.
		///
		decltype(auto) getMatrixPtr() noexcept;

	protected:
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