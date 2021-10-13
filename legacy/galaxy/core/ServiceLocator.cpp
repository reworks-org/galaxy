///
/// ServiceLocator.cpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#include "ServiceLocator.hpp"

namespace galaxy
{
	namespace core
	{
		ServiceLocator::ServiceLocator() noexcept
			: m_restart {false}
			, m_app_start_time_point {}
			, m_config {nullptr}
			, m_window {nullptr}
			, m_lua {nullptr}
			, m_fontbook {nullptr}
			, m_shaderbook {nullptr}
			, m_soundbook {nullptr}
			, m_musicbook {nullptr}
			, m_texturebook {nullptr}
			, m_vfs {nullptr}
			, m_openal {nullptr}
			, m_scriptbook {nullptr}
			, m_language {nullptr}
			, m_pool {nullptr}
		{
		}

		ServiceLocator& ServiceLocator::get() noexcept
		{
			static ServiceLocator s_service_locator;
			return s_service_locator;
		}

		fs::Config* ServiceLocator::config() const noexcept
		{
			return m_config;
		}

		core::Window* ServiceLocator::window() const noexcept
		{
			return m_window;
		}

		sol::state* ServiceLocator::lua() const noexcept
		{
			return m_lua;
		}

		res::FontBook* ServiceLocator::fontbook() const noexcept
		{
			return m_fontbook;
		}

		res::ShaderBook* ServiceLocator::shaderbook() const noexcept
		{
			return m_shaderbook;
		}

		res::SoundBook* ServiceLocator::soundbook() const noexcept
		{
			return m_soundbook;
		}

		res::MusicBook* ServiceLocator::musicbook() const noexcept
		{
			return m_musicbook;
		}

		res::TextureBook* ServiceLocator::texturebook() const noexcept
		{
			return m_texturebook;
		}

		fs::Virtual* ServiceLocator::vfs() const noexcept
		{
			return m_vfs;
		}

		audio::Context* ServiceLocator::openal() const noexcept
		{
			return m_openal;
		}

		res::ScriptBook* ServiceLocator::scriptbook() const noexcept
		{
			return m_scriptbook;
		}

		res::Language* ServiceLocator::lang() const noexcept
		{
			return m_language;
		}

		async::ThreadPool* ServiceLocator::pool() const noexcept
		{
			return m_pool;
		}
	} // namespace core
} // namespace galaxy