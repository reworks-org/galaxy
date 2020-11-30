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
		Transform();

		///
		/// Copy constructor.
		///
		Transform(const Transform&);

		///
		/// Move constructor.
		///
		Transform(Transform&&);

		///
		/// Copy assignment operator.
		///
		Transform& operator=(const Transform&);

		///
		/// Move assignment operator.
		///
		Transform& operator=(Transform&&);

		///
		/// Default virtual destructor.
		///
		virtual ~Transform() = default;

		///
		/// \brief Translate (move) position.
		///
		/// Does not set the position but moves it.
		/// By adding the parameters to the existing transformation matrix.
		///
		/// \param x How far to translate on x axis.
		/// \param y How far to translate on x axis.
		///
		void move(const float x, const float y);

		///
		/// Rotate transformation in degrees.
		///
		/// \param degrees Max 360, min -360.
		///
		void rotate(float degrees);

		///
		/// Scale transformation on each axis.
		///
		/// \param scale Scale. Multiplier.
		///
		void scale(const float scale);

		///
		/// Recalculates the model view matrix.
		///
		void recalculate();

		///
		/// Sets position without moving the object.
		///
		/// \param x X position to set object to.
		/// \param y Y position to set object to.
		///
		virtual void set_pos(const float x, const float y);

		///
		/// Set the rotation point.
		///
		/// \param x X position to set origin to.
		/// \param y Y position to set origin to.
		///
		void set_rotation_origin(const float x, const float y);

		///
		/// Get flag indicating if transform needs to be applied before rendering.
		///
		/// \return Const boolean.
		///
		[[nodiscard]] const bool is_dirty() const;

		///
		/// Retrieve internal transformation matrix.
		///
		/// \return Reference to internal glm::mat4.
		///
		[[nodiscard]] glm::mat4& get_transform();

		///
		/// Get stored rotation cache.
		///
		/// \return Const float.
		///
		[[nodiscard]] const float get_rotation() const;

		///
		/// Get stored scale cache.
		///
		/// \return Const float.
		///
		[[nodiscard]] const float get_scale() const;

		///
		/// Get stored pos cache.
		///
		/// \return Const glm::vec2.
		///
		[[nodiscard]] const glm::vec2& get_pos() const;

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

		///
		/// Cached for easiy retrieval.
		/// Rotation.
		///
		float m_rotate;

		///
		/// Cached for easiy retrieval.
		/// Scale.
		///
		float m_scale;

		///
		/// Cached for easiy retrieval.
		/// Pos.
		///
		glm::vec2 m_pos;

	private:
		///
		/// Combined transformation matrix.
		///
		glm::mat4 m_model;
	};
} // namespace qs

#endif