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
		ServiceLocator& ServiceLocator::get()
		{
			static ServiceLocator s_service_locator;
			return s_service_locator;
		}

		fs::Config* ServiceLocator::config() const
		{
			return m_config;
		}

		async::ThreadPool<4>* ServiceLocator::pool() const
		{
			return m_threadpool;
		}

		graphics::Window* ServiceLocator::window() const
		{
			return m_window;
		}

		graphics::Renderer* ServiceLocator::renderer() const
		{
			return m_renderer;
		}

		sol::state* ServiceLocator::lua() const
		{
			return m_lua;
		}

		core::StateMachine* ServiceLocator::gamestate() const
		{
			return m_state;
		}

		events::Dispatcher* ServiceLocator::dispatcher() const
		{
			return m_dispatcher;
		}

		core::World* ServiceLocator::world() const
		{
			return m_world;
		}

		fs::Serializer* ServiceLocator::serializer() const
		{
			return m_serializer;
		}

		res::FontBook* ServiceLocator::fontbook() const
		{
			return m_fontbook;
		}

		res::ShaderBook* ServiceLocator::shaderbook() const
		{
			return m_shaderbook;
		}

		res::AudioBook* ServiceLocator::audiobook() const
		{
			return m_audiobook;
		}

		graphics::TextureAtlas* ServiceLocator::atlas() const
		{
			return m_texture_atlas;
		}

		ServiceLocator::ServiceLocator()
		    : m_restart {false}, m_config {nullptr}, m_threadpool {nullptr}, m_window {nullptr}, m_renderer {nullptr}, m_lua {nullptr}, m_state {nullptr}, m_dispatcher {nullptr}, m_world {nullptr}, m_serializer {nullptr}, m_fontbook {nullptr}, m_shaderbook {nullptr}, m_audiobook {nullptr}, m_texture_atlas {nullptr}
		{
		}
	} // namespace core
} // namespace galaxy