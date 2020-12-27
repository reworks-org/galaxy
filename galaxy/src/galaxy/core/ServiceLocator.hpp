///
/// ServiceLocator.hpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#ifndef GALAXY_CORE_SERVICELOCATOR_HPP_
#define GALAXY_CORE_SERVICELOCATOR_HPP_

#include <sol/forward.hpp>

///
/// Shortcut macro.
///
#define SL_HANDLE galaxy::core::ServiceLocator::get()

namespace galaxy
{
	namespace async
	{
		template<std::size_t max_threads>
		class ThreadPool;
	} // namespace async

	namespace core
	{
		class StateMachine;
		class World;
	} // namespace core

	namespace events
	{
		class Dispatcher;
	} // namespace events

	namespace fs
	{
		class Config;
		class Serializer;
	} // namespace fs

	namespace graphics
	{
		class Window;
		class Renderer;
		class TextureAtlas;
	} // namespace graphics

	namespace res
	{
		class FontBook;
		class ShaderBook;
		class AudioBook;

	} // namespace res

	namespace core
	{
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
			ServiceLocator(const ServiceLocator&) = delete;

			///
			/// Move constructor.
			///
			ServiceLocator(ServiceLocator&&) = delete;

			///
			/// Copy assignment operator.
			///
			ServiceLocator& operator=(const ServiceLocator&) = delete;

			///
			/// Move assignment operator.
			///
			ServiceLocator& operator=(ServiceLocator&&) = delete;

			///
			/// Default Destructor.
			///
			~ServiceLocator() = default;

			///
			/// Get singleton instance.
			///
			/// \return Returns a reference to the internal singleton of this class.
			///
			[[nodiscard]] static ServiceLocator& get();

			///
			/// Get config service.
			///
			/// \return Return pointer to config service.
			///
			[[maybe_unused]] fs::Config* config() const;

			///
			/// Get threadpool service.
			///
			/// \return Return pointer to threadpool.
			///
			[[maybe_unused]] async::ThreadPool<4>* pool() const;

			///
			/// Get window service.
			///
			/// \return Return pointer to window service.
			///
			[[maybe_unused]] graphics::Window* window() const;

			///
			/// Get renderer service.
			///
			/// \return Return pointer to renderer service.
			///
			[[maybe_unused]] graphics::Renderer* renderer() const;

			///
			/// Get Lua service.
			///
			/// \return Return pointer to Lua service.
			///
			[[maybe_unused]] sol::state* lua() const;

			///
			/// Get game state service.
			///
			[[maybe_unused]] core::StateMachine* gamestate() const;

			///
			/// Get Dispatcher service.
			///
			/// \return Return pointer to Dispatcher service.
			///
			[[maybe_unused]] events::Dispatcher* dispatcher() const;

			///
			/// Get World service.
			///
			/// \return Return pointer to World service.
			///
			[[maybe_unused]] core::World* world() const;

			///
			/// Get serializer service.
			///
			/// \return Return pointer to Serializer service.
			///
			[[maybe_unused]] fs::Serializer* serializer() const;

			///
			/// Get FontBook service.
			///
			/// \return Return pointer to FontBook service.
			///
			[[maybe_unused]] res::FontBook* fontbook() const;

			///
			/// Get ShaderBook service.
			///
			/// \return Return pointer to ShaderBook service.
			///
			[[maybe_unused]] res::ShaderBook* shaderbook() const;

			///
			/// Get AudioBook service.
			///
			/// \return Return pointer to AudioBook service.
			///
			[[maybe_unused]] res::AudioBook* audiobook() const;

			///
			/// Get TextureAtlas service.
			///
			/// \return Return pointer to TextureAtlas service.
			///
			[[maybe_unused]] graphics::TextureAtlas* atlas() const;

			///
			/// Restart flag.
			///
			bool m_restart;

		private:
			///
			/// Default constructor.
			///
			ServiceLocator();

			///
			/// Config service.
			///
			fs::Config* m_config;

			///
			/// Threadpool service.
			///
			async::ThreadPool<4>* m_threadpool;

			///
			/// Window service.
			///
			graphics::Window* m_window;

			///
			/// Renderer service.
			///
			graphics::Renderer* m_renderer;

			///
			/// Lua service. Main instance of Lua.
			///
			sol::state* m_lua;

			///
			/// Game state service.
			///
			core::StateMachine* m_state;

			///
			/// Dispatcher service.
			///
			events::Dispatcher* m_dispatcher;

			///
			/// World service.
			///
			core::World* m_world;

			///
			/// Serializer service.
			///
			fs::Serializer* m_serializer;

			///
			/// FontBook service.
			///
			res::FontBook* m_fontbook;

			///
			/// ShaderBook service.
			///
			res::ShaderBook* m_shaderbook;

			///
			/// AudioBook service.
			///
			res::AudioBook* m_audiobook;

			///
			/// AudioBook service.
			///
			graphics::TextureAtlas* m_texture_atlas;
		};
	} // namespace core
} // namespace galaxy

#endif