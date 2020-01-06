///
/// ServiceLocator.hpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#ifndef GALAXY_SERVICELOCATOR_HPP_
#define GALAXY_SERVICELOCATOR_HPP_

#include <sol/forward.hpp>

// Forward decs.
namespace sf { class RenderWindow; }

///
/// Core namespace.
///
namespace galaxy
{
	// Forward decs.
	// This avoids mass includes.
	class Config;
	class FileSystem;
	class World;

	///
	/// Provides pointer access to services in the framework.
	/// Cannot be created, copied or moved.
	///
	class ServiceLocator final
	{
		///
		/// Allows application, and only application, to set a service's pointer.
		///
		friend class Application;

	public:
		///
		/// Default Destructor.
		///
		~ServiceLocator() = default;

		///
		/// Get singleton instance.
		///
		/// \return Returns a reference to the internal singleton of this class.
		///
		static galaxy::ServiceLocator& i() noexcept;

		///
		/// Get Lua service.
		///
		/// \return Return const pointer to Lua service.
		///
		const auto lua() const noexcept;

		///
		/// Get Config service.
		///
		/// \return Return const pointer to Config service.
		///
		const auto config() const noexcept;

		///
		/// Get FileSystem service.
		///
		/// \return Return const pointer to FileSystem service.
		///
		const auto fs() const noexcept;

		///
		/// Get Window service.
		///
		/// \return Return const pointer to Window service.
		///
		const auto window() const noexcept;

		///
		/// Get World service.
		///
		/// \return Return const pointer to World service.
		///
		const auto world() const noexcept;

	private:
		///
		/// Default constructor.
		///
		ServiceLocator();

		///
		/// Copy Constructor.
		/// Deleted.
		///
		ServiceLocator(const ServiceLocator&) = delete;

		///
		/// Move Constructor.
		/// Deleted.
		///
		ServiceLocator(ServiceLocator&&) = delete;

		///
		/// Copy assignment operator.
		/// Deleted.
		///
		ServiceLocator& operator=(const ServiceLocator &) = delete;

		///
		/// Move assignment operator.
		/// Deleted.
		///
		ServiceLocator& operator=(ServiceLocator &&) = delete;

	private:
		///
		/// Lua service. Main instance of Lua.
		///
		sol::state* m_lua;

		///
		/// Config service.
		///
		galaxy::Config* m_config;

		///
		/// FileSystem service.
		///
		galaxy::FileSystem* m_fs;

		///
		/// Window service.
		///
		sf::RenderWindow* m_window;

		///
		/// World service.
		///
		galaxy::World* m_world;
	};
}

#endif