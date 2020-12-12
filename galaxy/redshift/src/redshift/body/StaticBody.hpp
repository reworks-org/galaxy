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
	class StaticBody final : public rs::Body
	{
	public:
		///
		/// Argument constructor.
		///
		StaticBody(const float density);

		virtual ~StaticBody() = default;

		const bool is_rigid() const noexcept override;

	private:
		StaticBody() = delete;
	};
} // namespace rs

#endif