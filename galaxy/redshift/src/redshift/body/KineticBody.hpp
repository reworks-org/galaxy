///
/// KineticBody.hpp
/// redshift
///
/// Refer to LICENSE.txt for more details.
///

#ifndef REDSHIFT_KINETICBODY_HPP_
#define REDSHIFT_KINETICBODY_HPP_

#include "redshift/body/Body.hpp"

///
/// Core namespace.
///
namespace rs
{
	class KineticBody final : public rs::Body
	{
	public:
		KineticBody();
		~KineticBody();

		const bool is_rigid() const noexcept override;
	};
} // namespace rs

#endif