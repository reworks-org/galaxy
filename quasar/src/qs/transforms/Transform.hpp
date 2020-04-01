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
		/// \param z How far to translate on x axis. Defaults to 0.
		///
		void move(const float x, const float y, const float z = 0.0f);

		///
		/// Rotate transformation in degrees.
		///
		/// \param degrees Max 360, min -360.
		///
		void rotate(const float degrees) noexcept;
		
		///
		/// Scale transformation on each axis.
		///
		/// \param x X axis scale. Multiplier.
		/// \param y Y axis scale. Multiplier.
		/// \param z Optional z axis scaling. Defaults to 1.0f (no scale).
		///
		void scale(const float x, const float y, const float z = 1.0f) noexcept;

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

	private:
		///
		/// Constant identity matrix.
		///
		const glm::mat4 m_identityMatrix;

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