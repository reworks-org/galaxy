///
/// World.hpp
/// redshift
///
/// Refer to LICENSE.txt for more details.
///

#ifndef REDSHIFT_WORLD_HPP_
#define REDSHIFT_WORLD_HPP_

#include <mutex>

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
		World();
		~World();

		void changeGravity(const double gravity) noexcept;

	private:
		///
		/// To protect resources from race conditions and access.
		///
		std::mutex m_mutex;
		
		///
		/// Gravity value.
		/// Defaults to 9.81.
		///
		double m_gravity;
	};
}

#endif