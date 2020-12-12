///
/// AABB.hpp
/// redshift
///
/// Refer to LICENSE.txt for more details.
///

#include <glm/vec2.hpp>

///
/// Core namespace.
///
namespace rs
{
	///
	/// AABB bounding box for seperating axis theorm.
	/// See:
	/// https://www.azurefromthetrenches.com/introductory-guide-to-aabb-tree-collision-detection/
	/// https://realtimecollisiondetection.net/
	///
	struct AABB final
	{
		///
		/// Constructor.
		///
		AABB();

		///
		/// Argument constructor.
		///
		/// \param min Minimum bounds.
		/// \param max Maximum bounds.
		///
		AABB(const glm::vec2& min, const glm::vec2& max);

		///
		/// Destructor.
		///
		~AABB() = default;

		///
		/// Test for an overlap.
		///
		/// \param b AABB to test.
		///
		/// \return Const bool. True if overlap.
		///
		[[nodiscard]] const bool overlaps(const AABB& b) const;

		///
		/// Check if this AABB contains another AABB.
		///
		/// \param b AABB to check.
		///
		/// \return Const bool. True if contains.
		///
		[[nodiscard]] const bool contains(const AABB& b) const;

		///
		/// Merge two AABBs.
		///
		/// \param b AABB to merge with this.
		///
		/// \return Merged AABB.
		///
		[[nodiscard]] AABB merge(const AABB& b) const;

		///
		/// Get intersection of two AABBs.
		///
		/// \param b AABB to test.
		///
		/// \return AABB intersection.
		///
		[[nodiscard]] AABB intersection(const AABB& b) const;

		///
		/// Get surface area of AABB.
		///
		/// \return Const float.
		///
		[[nodiscard]] const float surface_area() const;

		///
		/// Lower bounds.
		///
		glm::vec2 m_min;

		///
		/// Upper bounds.
		///
		glm::vec2 m_max;
	};
} // namespace rs