///
/// World.hpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#ifndef GALAXY_WORLD_HPP_
#define GALAXY_WORLD_HPP_

#include <solar/entity/Manager.hpp>

///
/// Core namespace.
///
namespace galaxy
{
	///
	/// The World class. Contains the entities and systems and other library stuff, like the main lua state.
	///
	class World final
	{
	public: 
		///
		/// Constructor.
		///
		World() noexcept;

		///
		/// Destructor.
		///
		~World() noexcept;

	private:
		///
		/// ECS manager and main class.
		///
		sr::Manager m_manager;
	};
}

#endif