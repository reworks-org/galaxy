///
/// ServiceLocator.hpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#ifndef GALAXY_SERVICELOCATOR_HPP_
#define GALAXY_SERVICELOCATOR_HPP_

#include <sol/forward.hpp>

///
/// Shortcut macro.
///
#define SL_HANDLE galaxy::ServiceLocator::get()

///
/// Forward dec.
///
namespace qs
{
	class Window;
	class Renderer;
}

///
/// Forward dec.
///
namespace protostar
{
	class ThreadPool;
	class StateMachine;
}

///
/// Forward dec.
///
namespace starlight
{
	class Dispatcher;
}

///
/// Core namespace.
///
namespace galaxy
{
	///
	/// Forward decs.
	/// This avoids mass includes.
	///
	class Config;
	class World;
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
		~ServiceLocator() noexcept = default;

		///
		/// Get singleton instance.
		///
		/// \return Returns a reference to the internal singleton of this class.
		///
		static galaxy::ServiceLocator& get() noexcept;

		///
		/// Get config service.
		///
		/// \return Return pointer to config service.
		///
		galaxy::Config* config() const noexcept;

		///
		/// Get threadpool service.
		///
		/// \return Return pointer to threadpool.
		///
		protostar::ThreadPool* pool() const noexcept;

		///
		/// Get window service.
		///
		/// \return Return pointer to window service.
		///
		qs::Window* window() const noexcept;

		///
		/// Get renderer service.
		///
		/// \return Return pointer to renderer service.
		///
		qs::Renderer* renderer() const noexcept;

		///
		/// Get Lua service.
		///
		/// \return Return const pointer to Lua service.
		///
		sol::state* lua() const noexcept;
		
		///
		/// Get game state service.
		///
		protostar::StateMachine* gamestate() const noexcept;

		///
		/// Get Dispatcher service.
		///
		/// \return Return const pointer to Dispatcher service.
		///
		starlight::Dispatcher* dispatcher() const noexcept;

		///
		/// Get World service.
		///
		/// \return Return const pointer to World service.
		///
		galaxy::World* world() const noexcept;

		///
		/// Get serializer service.
		///
		/// \return Return const pointer to Serializer service.
		///
		galaxy::Serializer* serializer() const noexcept;

	private:
		///
		/// Default constructor.
		///
		ServiceLocator() noexcept;

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

	public:
		///
		/// Restart flag.
		///
		bool m_restart;

	private:
		///
		/// Config service.
		///
		galaxy::Config* m_config;

		///
		/// Threadpool service.
		///
		protostar::ThreadPool* m_threadPool;

		///
		/// Window service.
		///
		qs::Window* m_window;

		///
		/// Renderer service.
		///
		qs::Renderer* m_renderer;

		///
		/// Lua service. Main instance of Lua.
		///
		sol::state* m_lua;

		///
		/// Game state service.
		///
		protostar::StateMachine* m_state;

		///
		/// Dispatcher service.
		///
		starlight::Dispatcher* m_dispatcher;

		///
		/// World service.
		///
		galaxy::World* m_world;

		///
		/// Serializer service.
		///
		galaxy::Serializer* m_serializer;
	};
}

#endif