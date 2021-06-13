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
		class ThreadPool;
	} // namespace async

	namespace audio
	{
		class Context;
	} // namespace audio

	namespace core
	{
		class Window;
	} // namespace core

	namespace fs
	{
		class Config;
		class Virtual;
	} // namespace fs

	namespace res
	{
		class FontBook;
		class ShaderBook;
		class TextureAtlas;
		class SoundBook;
		class MusicBook;
		class ScriptBook;
		class MaterialBook;
		class Language;
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
			/// Default Destructor.
			///
			~ServiceLocator() noexcept = default;

			///
			/// Get singleton instance.
			///
			/// \return Returns a reference to the internal singleton of this class.
			///
			[[nodiscard]] static ServiceLocator& get() noexcept;

			///
			/// Get config service.
			///
			/// \return Return pointer to config service.
			///
			[[maybe_unused]] fs::Config* config() const noexcept;

			///
			/// Get window service.
			///
			/// \return Return pointer to window service.
			///
			[[maybe_unused]] core::Window* window() const noexcept;

			///
			/// Get Lua service.
			///
			/// \return Return pointer to Lua service.
			///
			[[maybe_unused]] sol::state* lua() const noexcept;

			///
			/// Get FontBook service.
			///
			/// \return Return pointer to FontBook service.
			///
			[[maybe_unused]] res::FontBook* fontbook() const noexcept;

			///
			/// Get ShaderBook service.
			///
			/// \return Return pointer to ShaderBook service.
			///
			[[maybe_unused]] res::ShaderBook* shaderbook() const noexcept;

			///
			/// Get SoundBook service.
			///
			/// \return Return pointer to SoundBook service.
			///
			[[maybe_unused]] res::SoundBook* soundbook() const noexcept;

			///
			/// Get MusicBook service.
			///
			/// \return Return pointer to MusicBook service.
			///
			[[maybe_unused]] res::MusicBook* musicbook() const noexcept;

			///
			/// Get TextureAtlas service.
			///
			/// \return Return pointer to TextureAtlas service.
			///
			[[maybe_unused]] res::TextureAtlas* atlas() const noexcept;

			///
			/// Get Virtual FileSystem service.
			///
			/// \return Return pointer to Virtual FileSystem service.
			///
			[[maybe_unused]] fs::Virtual* vfs() const noexcept;

			///
			/// Get OpenAL context service.
			///
			/// \return Return pointer to OpenAL context.
			///
			[[maybe_unused]] audio::Context* openal() const noexcept;

			///
			/// Get ScriptBook service.
			///
			/// \return Return pointer to ScriptBook service.
			///
			[[maybe_unused]] res::ScriptBook* scriptbook() const noexcept;

			///
			/// Get MaterialBook service.
			///
			/// \return Return pointer to ScriptBook service.
			///
			[[maybe_unused]] res::MaterialBook* materialbook() const noexcept;

			///
			/// Get Language service.
			///
			/// \return Return pointer to Language service.
			///
			[[maybe_unused]] res::Language* lang() const noexcept;

			///
			/// Get ThreadPool service.
			///
			/// \return Return pointer to ThreadPool service.
			///
			[[maybe_unused]] async::ThreadPool* pool() const noexcept;

		public:
			///
			/// Restart flag.
			///
			bool m_restart;

		private:
			///
			/// Constructor.
			///
			ServiceLocator() noexcept;

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

		private:
			///
			/// Config service.
			///
			fs::Config* m_config;

			///
			/// Window service.
			///
			core::Window* m_window;

			///
			/// Lua service. Main instance of Lua.
			///
			sol::state* m_lua;

			///
			/// FontBook service.
			///
			res::FontBook* m_fontbook;

			///
			/// ShaderBook service.
			///
			res::ShaderBook* m_shaderbook;

			///
			/// SoundBook service.
			///
			res::SoundBook* m_soundbook;

			///
			/// MusicBook service.
			///
			res::MusicBook* m_musicbook;

			///
			/// Texture Atlas service.
			///
			res::TextureAtlas* m_texture_atlas;

			///
			/// Virtual FileSystem service.
			///
			fs::Virtual* m_vfs;

			///
			/// Audio Context service.
			///
			audio::Context* m_openal;

			///
			/// ScriptBook service.
			///
			res::ScriptBook* m_scriptbook;

			///
			/// MaterialBook service.
			///
			res::MaterialBook* m_materialbook;

			///
			/// Language service.
			///
			res::Language* m_language;

			///
			/// ThreadPool service.
			///
			async::ThreadPool* m_pool;
		};
	} // namespace core
} // namespace galaxy

#endif