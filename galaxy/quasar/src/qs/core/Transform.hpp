///
/// Transform.hpp
/// quasar
///
/// Refer to LICENSE.txt for more details.
///

#ifndef QUASAR_TRANSFORM_HPP_
#define QUASAR_TRANSFORM_HPP_

#include <glm/gtc/matrix_transform.hpp>
#include <protostar/system/Concepts.hpp>

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
		/// Rotate transformation in degrees.
		///
		/// \param degrees Max 360, min -360.
		///
		void rotate(float degrees) noexcept;

		///
		/// Scale transformation on each axis.
		///
		/// \param scale Scale. Multiplier.
		///
		void scale(const pr::positive_float auto scale) noexcept;

		///
		/// Recalculates the model view matrix.
		///
		virtual void recalculate() noexcept;

		///
		/// Sets position without moving the object.
		///
		/// \param x X position to set object to.
		/// \param y Y position to set object to.
		///
		void set_pos(const float x, const float y) noexcept;

		///
		/// Set the rotation point.
		///
		/// \param x X position to set origin to.
		/// \param y Y position to set origin to.
		///
		void set_rotation_origin(const float x, const float y) noexcept;

		///
		/// Get flag indicating if transform needs to be applied before rendering.
		///
		/// \return Const boolean.
		///
		[[nodiscard]] const bool is_dirty() const noexcept;

		///
		/// Retrieve internal transformation matrix.
		///
		/// \return Reference to internal glm::mat4.
		///
		[[nodiscard]] glm::mat4& get_transform() noexcept;

	protected:
		///
		/// Update flag.
		///
		bool m_dirty;

		///
		/// Rotation origin point.
		///
		glm::vec3 m_origin;

		///
		/// Rotational matrix.
		///
		glm::mat4 m_rotation;

		///
		/// Scaled matrix.
		///
		glm::mat4 m_scaling;

		///
		/// Translation matrix.
		///
		glm::mat4 m_translation;

	private:
		///
		/// Combined transformation matrix.
		///
		glm::mat4 m_model;
	};
} // namespace qs

#endif