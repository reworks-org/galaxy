///
/// Rigid.hpp
/// redshift
///
/// Refer to LICENSE.txt for more details.
///

#ifndef REDSHIFT_RIGID_HPP_
#define REDSHIFT_RIGID_HPP_

#include "redshift/body/Body.hpp"

///
/// Core namespace.
///
namespace rs
{
	///
	/// Represents a fixed point body.
	///
	class Rigid final : public rs::Body
	{
	public:
		///
		/// Constructor.
		///
		Rigid() noexcept;

		///
		/// Virtual destructor.
		///
		~Rigid() noexcept override;

		///
		/// Is the body fixed or dynamic.
		///
		const bool isFixed() const noexcept override;

	private:
	};
}

#endif