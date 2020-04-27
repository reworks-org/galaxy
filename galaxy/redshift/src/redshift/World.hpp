///
/// World.hpp
/// redshift
///
/// Refer to LICENSE.txt for more details.
///

#ifndef REDSHIFT_WORLD_HPP_
#define REDSHIFT_WORLD_HPP_

#include <mutex>
#include <vector>

#include "redshift/body/Rigid.hpp"
#include "redshift/body/Kinetic.hpp"

///
/// Core namespace.
///
namespace rs
{
	///
	/// Physics "world" i.e. main class.
	///
	class World
	{
	public:
		///
		/// Default constructor.
		///
		World() noexcept;

		///
		/// Argument constructor.
		///
		World(const double gravity) noexcept;

		///
		///
		///
		~World() noexcept;

		///
		///
		///
		void process(const double deltaTime) noexcept;

		///
		///
		///
		void setGravity(const double gravity) noexcept;

	private:
		///
		/// \brief Gravity value.
		///
		/// Defaults to 9.81.
		///
		protostar::Vector2<double> m_gravity;

		///
		/// To protect resources from race conditions and access.
		///
		std::mutex m_mutex;

		///
		/// Holds async futures.
		///
		std::vector<std::future<void>> m_asyncResults;
		
		///
		/// Kinetic bodies.
		///
		std::vector<rs::Kinetic> m_kinetics;

		///
		/// Rigid bodies.
		///
		std::vector<rs::Rigid> m_rigids;
	};
}

#endif