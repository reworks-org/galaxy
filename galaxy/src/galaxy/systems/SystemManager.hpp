///
/// SystemManager.hpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#ifndef GALAXY_SYSTEMS_SYSTEMMANAGER_HPP_
#define GALAXY_SYSTEMS_SYSTEMMANAGER_HPP_

#include "galaxy/meta/SystemFactory.hpp"

namespace galaxy
{
	class Scene;

	///
	/// \brief Manages the systems assigned to it.
	///
	/// A system belongs to a scene and operates on it and the shared entities.
	///
	class SystemManager final
	{
	public:
		///
		/// Constructor.
		///
		SystemManager() noexcept;

		///
		/// Move constructor.
		///
		SystemManager(SystemManager&&) noexcept;

		///
		/// Move assignment operator.
		///
		SystemManager& operator=(SystemManager&&) noexcept;

		///
		/// Destructor.
		///
		~SystemManager() noexcept;

		///
		/// \brief Add a system to operate on entities in this scene.
		///
		/// Scene is called in order of adding. So i.e. if you add anim then render, systems are called in that order.
		///
		/// \param system Name of system to add to this scene.
		///
		void add_system(const std::string& system);

		///
		/// Process all systems.
		///
		/// \param em Entities to process.
		/// \param scene Scene this system belongs to.
		///
		void update(EntityManager& em, Scene* scene);

		///
		/// Suspend all running systems.
		///
		void suspend() noexcept;

		///
		/// Resume all systems.
		///
		void resume() noexcept;

		///
		/// Remove all systems.
		///
		void clear();

		///
		/// Get list of systems.
		///
		/// \return System stack.
		///
		[[nodiscard]]
		SystemStack& stack() noexcept;

	private:
		///
		/// Copy assignment operator.
		///
		SystemManager& operator=(const SystemManager&) = delete;
		///
		/// Copy constructor.
		///
		SystemManager(const SystemManager&) = delete;

	private:
		///
		/// List of systems to process.
		///
		SystemStack m_systems;

		///
		/// Are systems allowed to run.
		///
		bool m_suspended;
	};
} // namespace galaxy

#endif
