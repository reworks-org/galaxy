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
		galaxy::ServiceLocator& ServiceLocator::get()
		{
			static galaxy::ServiceLocator s_service_locator;
			return s_service_locator;
		}

		galaxy::Config* ServiceLocator::config() const
		{
			return m_config;
		}

		pr::ThreadPool<4>* ServiceLocator::pool() const
		{
			return m_threadpool;
		}

		qs::Window* ServiceLocator::window() const
		{
			return m_window;
		}

		qs::Renderer* ServiceLocator::renderer() const
		{
			return m_renderer;
		}

		sol::state* ServiceLocator::lua() const
		{
			return m_lua;
		}

		pr::StateMachine* ServiceLocator::gamestate() const
		{
			return m_state;
		}

		sl::Dispatcher* ServiceLocator::dispatcher() const
		{
			return m_dispatcher;
		}

		galaxy::World* ServiceLocator::world() const
		{
			return m_world;
		}

		galaxy::Serializer* ServiceLocator::serializer() const
		{
			return m_serializer;
		}

		galaxy::FontBook* ServiceLocator::fontbook() const
		{
			return m_fontbook;
		}

		galaxy::ShaderBook* ServiceLocator::shaderbook() const
		{
			return m_shaderbook;
		}

		galaxy::AudioBook* ServiceLocator::audiobook() const
		{
			return m_audiobook;
		}

		qs::TextureAtlas* ServiceLocator::atlas() const
		{
			return m_texture_atlas;
		}

		ServiceLocator::ServiceLocator()
		    : m_restart {false}, m_config {nullptr}, m_threadpool {nullptr}, m_window {nullptr}, m_renderer {nullptr}, m_lua {nullptr}, m_state {nullptr}, m_dispatcher {nullptr}, m_world {nullptr}, m_serializer {nullptr}, m_fontbook {nullptr}, m_shaderbook {nullptr}, m_audiobook {nullptr}, m_texture_atlas {nullptr}
		{
		}
	} // namespace core
} // namespace galaxy