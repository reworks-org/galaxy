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
		///
		StaticBody(const float density);

		///
		/// Virtual destructor.
		///
		virtual ~StaticBody() = default;

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