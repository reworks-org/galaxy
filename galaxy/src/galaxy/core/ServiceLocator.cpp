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

		async::ThreadPool<4>* ServiceLocator::pool() const noexcept
		{
			return m_threadpool;
		}

		graphics::Window* ServiceLocator::window() const noexcept
		{
			return m_window;
		}

		graphics::Renderer* ServiceLocator::renderer() const noexcept
		{
			return m_renderer;
		}

		sol::state* ServiceLocator::lua() const noexcept
		{
			return m_lua;
		}

		core::StateMachine* ServiceLocator::gamestate() const noexcept
		{
			return m_state;
		}

		events::Dispatcher* ServiceLocator::dispatcher() const noexcept
		{
			return m_dispatcher;
		}

		fs::Serializer* ServiceLocator::serializer() const noexcept
		{
			return m_serializer;
		}

		res::FontBook* ServiceLocator::fontbook() const noexcept
		{
			return m_fontbook;
		}

		res::ShaderBook* ServiceLocator::shaderbook() const noexcept
		{
			return m_shaderbook;
		}

		res::AudioBook* ServiceLocator::audiobook() const noexcept
		{
			return m_audiobook;
		}

		graphics::TextureAtlas* ServiceLocator::atlas() const noexcept
		{
			return m_texture_atlas;
		}

		fs::Virtual* ServiceLocator::vfs() const noexcept
		{
			return m_vfs;
		}

		ServiceLocator::ServiceLocator() noexcept
		    : m_restart {false}, m_config {nullptr}, m_threadpool {nullptr}, m_window {nullptr}, m_renderer {nullptr}, m_lua {nullptr}, m_state {nullptr}, m_dispatcher {nullptr}, m_serializer {nullptr}, m_fontbook {nullptr}, m_shaderbook {nullptr}, m_audiobook {nullptr}, m_texture_atlas {nullptr}, m_vfs {nullptr}
		{
		}
	} // namespace core
} // namespace galaxy