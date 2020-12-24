///
/// Circle.hpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#ifndef GALAXY_CIRCLE_HPP_
#define GALAXY_CIRCLE_HPP_

#include <compare>

#include <glm/vec2.hpp>

///
/// Core namespace.
///
namespace galaxy
{
	///
	/// Primitive geometric shape.
	///
	class Circle final
	{
	public:
		///
		/// Constructor.
		///
		Circle();

		///
		/// Argument constructor.
		///
		/// \param x X axis coord.
		/// \param y Y axis coord.
		/// \param radius Radius of circle.
		///
		Circle(const float x, const float y, const float radius);

		///
		/// Destructor.
		///
		~Circle() = default;

		///
		/// Create circle.
		///
		/// \param x X axis coord.
		/// \param y Y axis coord.
		/// \param radius Radius of circle.
		///
		void create(const float x, const float y, const float radius);

		///
		/// Get x coord of circle.
		///
		/// \return Const float.
		///
		[[nodiscard]] const float get_x() const;

		///
		/// Get y coord of circle.
		///
		/// \return Const float.
		///
		[[nodiscard]] const float get_y() const;

		///
		/// Get xy as a vector.
		///
		/// \return Const reference to glm::vec2.
		///
		[[nodiscard]] const glm::vec2& get_xy() const;

		///
		/// Get radius of circle.
		///
		/// \return Const float.
		///
		[[nodiscard]] const float radius() const;

		///
		/// Strongly ordered spaceship operator.
		///
		auto operator<=>(const Circle&) const = default;

	private:
		///
		/// Position of circle.
		///
		glm::vec2 m_pos;

		///
		/// Radius of circle.
		///
		float m_radius;
	};
} // namespace galaxy

#endif