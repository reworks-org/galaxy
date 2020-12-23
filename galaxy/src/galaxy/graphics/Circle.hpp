///
/// Circle.hpp
/// protostar
///
/// Refer to LICENSE.txt for more details.
///

#ifndef PROTOSTAR_CIRCLE_HPP_
#define PROTOSTAR_CIRCLE_HPP_

#include <compare>

#include <glm/vec2.hpp>

///
/// Core namespace.
///
namespace pr
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
		Circle() noexcept;

		///
		/// Argument constructor.
		///
		/// \param x X axis coord.
		/// \param y Y axis coord.
		/// \param radius Radius of circle.
		///
		Circle(const float x, const float y, const float radius) noexcept;

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
		void create(const float x, const float y, const float radius) noexcept;

		///
		/// Get x coord of circle.
		///
		/// \return Const float.
		///
		[[nodiscard]] const float get_x() const noexcept;

		///
		/// Get y coord of circle.
		///
		/// \return Const float.
		///
		[[nodiscard]] const float get_y() const noexcept;

		///
		/// Get xy as a vector.
		///
		/// \return Const reference to glm::vec2.
		///
		[[nodiscard]] const glm::vec2& get_xy() const noexcept;

		///
		/// Get radius of circle.
		///
		/// \return Const float.
		///
		[[nodiscard]] const float radius() const noexcept;

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
} // namespace pr

#endif