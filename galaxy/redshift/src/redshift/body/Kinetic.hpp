///
/// Kinetic.hpp
/// redshift
///
/// Refer to LICENSE.txt for more details.
///

#ifndef REDSHIFT_KINETIC_HPP_
#define REDSHIFT_KINETIC_HPP_

#include <future>

#include "redshift/body/Body.hpp"

///
/// Core namespace.
///
namespace rs
{
	///
	/// Represents a dynamic body.
	///
	class Kinetic final : public rs::Body
	{
	public:
		///
		/// Constructor.
		///
		Kinetic() noexcept;

		///
		/// Virtual destructor.
		///
		~Kinetic() noexcept override;

		///
		/// Is the body fixed or dynamic.
		///
		const bool isFixed() const noexcept override;

		///
		/// Move the body over a distance with a velocity acting on the body.
		///
		/// \param acc External acting acceleration on the body.
		/// \param time Application delta time.
		/// \param cv Condition variable for multithreading.
		///
		void move(const pr::Vector2<double>& acc, const double time, std::condition_variable& cv) noexcept;

	private:
		///
		/// Current velocity of body.
		///
		pr::Vector2<double> m_velocity;
	};
}

#endif