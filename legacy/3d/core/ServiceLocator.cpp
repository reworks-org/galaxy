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

		res::TextureAtlas* ServiceLocator::atlas() const noexcept
		{
			return m_texture_atlas;
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

		res::MaterialBook* ServiceLocator::materialbook() const noexcept
		{
			return m_materialbook;
		}

		res::Language* ServiceLocator::lang() const noexcept
		{
			return m_language;
		}

		async::ThreadPool* ServiceLocator::pool() const noexcept
		{
			return m_pool;
		}

		ServiceLocator::ServiceLocator() noexcept
		    : m_restart {false}, m_config {nullptr}, m_window {nullptr}, m_lua {nullptr}, m_fontbook {nullptr}, m_shaderbook {nullptr}, m_soundbook {nullptr}, m_musicbook {nullptr}, m_texture_atlas {nullptr}, m_vfs {nullptr}, m_openal {nullptr}, m_scriptbook {nullptr}, m_materialbook {nullptr}, m_language {nullptr}, m_pool {nullptr}
		{
		}
	} // namespace core
} // namespace galaxy