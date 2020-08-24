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
} // namespace qs

///
/// Forward dec.
///
namespace pr
{
	class ThreadPool;
	class StateMachine;
} // namespace pr

///
/// Forward dec.
///
namespace sl
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
	class FontBook;
	class ShaderBook;
	class AudioBook;

	///
	/// Provides pointer access to services in the framework.
	/// Cannot be created, copied or moved.
	///
	class ServiceLocator final
	{
		friend class Application;

	public:
		///
		/// Copy constructor.
		///
		ServiceLocator(const ServiceLocator&) noexcept = delete;

		///
		/// Move constructor.
		///
		ServiceLocator(ServiceLocator&&) noexcept = delete;

		///
		/// Copy assignment operator.
		///
		ServiceLocator& operator=(const ServiceLocator&) noexcept = delete;

		///
		/// Move assignment operator.
		///
		ServiceLocator& operator=(ServiceLocator&&) noexcept = delete;

		///
		/// Default Destructor.
		///
		~ServiceLocator() noexcept = default;

		///
		/// Get singleton instance.
		///
		/// \return Returns a reference to the internal singleton of this class.
		///
		[[nodiscard]] static galaxy::ServiceLocator& get() noexcept;

		///
		/// Get config service.
		///
		/// \return Return pointer to config service.
		///
		[[maybe_unused]] galaxy::Config* config() const noexcept;

		///
		/// Get threadpool service.
		///
		/// \return Return pointer to threadpool.
		///
		[[maybe_unused]] pr::ThreadPool* pool() const noexcept;

		///
		/// Get window service.
		///
		/// \return Return pointer to window service.
		///
		[[maybe_unused]] qs::Window* window() const noexcept;

		///
		/// Get renderer service.
		///
		/// \return Return pointer to renderer service.
		///
		[[maybe_unused]] qs::Renderer* renderer() const noexcept;

		///
		/// Get Lua service.
		///
		/// \return Return const pointer to Lua service.
		///
		[[maybe_unused]] sol::state* lua() const noexcept;

		///
		/// Get game state service.
		///
		[[maybe_unused]] pr::StateMachine* gamestate() const noexcept;

		///
		/// Get Dispatcher service.
		///
		/// \return Return const pointer to Dispatcher service.
		///
		[[maybe_unused]] sl::Dispatcher* dispatcher() const noexcept;

		///
		/// Get World service.
		///
		/// \return Return const pointer to World service.
		///
		[[maybe_unused]] galaxy::World* world() const noexcept;

		///
		/// Get serializer service.
		///
		/// \return Return const pointer to Serializer service.
		///
		[[maybe_unused]] galaxy::Serializer* serializer() const noexcept;

		///
		/// Get FontBook service.
		///
		/// \return Return const pointer to FontBook service.
		///
		[[maybe_unused]] galaxy::FontBook* fontbook() const noexcept;

		///
		/// Get ShaderBook service.
		///
		/// \return Return const pointer to ShaderBook service.
		///
		[[maybe_unused]] galaxy::ShaderBook* shaderbook() const noexcept;

		///
		/// Get AudioBook service.
		///
		/// \return Return const pointer to AudioBook service.
		///
		[[maybe_unused]] galaxy::AudioBook* audiobook() const noexcept;

		///
		/// Restart flag.
		///
		bool m_restart;

	private:
		///
		/// Default constructor.
		///
		ServiceLocator() noexcept;

		///
		/// Config service.
		///
		galaxy::Config* m_config;

		///
		/// Threadpool service.
		///
		pr::ThreadPool* m_threadpool;

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
		pr::StateMachine* m_state;

		///
		/// Dispatcher service.
		///
		sl::Dispatcher* m_dispatcher;

		///
		/// World service.
		///
		galaxy::World* m_world;

		///
		/// Serializer service.
		///
		galaxy::Serializer* m_serializer;

		///
		/// FontBook service.
		///
		galaxy::FontBook* m_fontbook;

		///
		/// ShaderBook service.
		///
		galaxy::ShaderBook* m_shaderbook;

		///
		/// AudioBook service.
		///
		galaxy::AudioBook* m_audiobook;
	};
} // namespace galaxy

#endif