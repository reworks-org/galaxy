///
/// StaticBody.hpp
/// redshift
///
/// Refer to LICENSE.txt for more details.
///

#ifndef REDSHIFT_STATICBODY_HPP_
#define REDSHIFT_STATICBODY_HPP_

#include "redshift/body/Body.hpp"

///
/// Core namespace.
///
namespace rs
{
	///
	/// A specialized body that cannot move.
	///
	class StaticBody final : public rs::Body
	{
	public:
		///
		/// Argument constructor.
		///
		/// \param density Density of this object.
		/// \param size Size of this object.
		///
		StaticBody(const float density, const glm::vec2& size);

		///
		/// Argument constructor.
		///
		/// \param density Density of this object.
		/// \param width Width of the object.
		/// \param height Height of the object.
		///
		StaticBody(const float density, const float width, const float height);

		///
		/// Virtual destructor.
		///
		virtual ~StaticBody() = default;

		///
		/// Get current velocity.
		///
		/// \return Const glm::vec2 reference.
		///
		[[nodiscard]] const glm::vec2& get_vel() const override;

		///
		/// Check if this body is rigid or can move.
		///
		/// \return Const bool.
		///
		const bool is_rigid() const noexcept override;

	private:
		///
		/// Deleted default constructor.
		///
		StaticBody() = delete;
	};
} // namespace rs

#endif