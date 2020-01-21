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
namespace starlight { class Dispatcher; }

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
	class Editor;
	class Serializer;

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
		sol::state* lua() const noexcept;

		///
		/// Get Config service.
		///
		/// \return Return const pointer to Config service.
		///
		galaxy::Config* config() const noexcept;

		///
		/// Get FileSystem service.
		///
		/// \return Return const pointer to FileSystem service.
		///
		galaxy::FileSystem* fs() const noexcept;

		///
		/// Get Window service.
		///
		/// \return Return const pointer to Window service.
		///
		sf::RenderWindow* window() const noexcept;

		///
		/// Get World service.
		///
		/// \return Return const pointer to World service.
		///
		galaxy::World* world() const noexcept;

		///
		/// Get Editor service.
		///
		/// \return Return const pointer to Editor service.
		///
		galaxy::Editor* editor() const noexcept;

		///
		/// Get Dispatcher service.
		///
		/// \return Return const pointer to Dispatcher service.
		///
		starlight::Dispatcher* dispatcher() const noexcept;

		///
		/// Get Serializer service.
		///
		/// \return Return const pointer to Dispatcher service.
		///
		galaxy::Serializer* serializer() const noexcept;

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

		///
		/// Editor service.
		///
		galaxy::Editor* m_editor;

		///
		/// Dispatcher service.
		///
		starlight::Dispatcher* m_dispatcher;

		///
		/// Serializer service.
		///
		galaxy::Serializer* m_serializer;
	};
}

#endif