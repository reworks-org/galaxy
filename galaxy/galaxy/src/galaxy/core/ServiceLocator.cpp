///
/// ServiceLocator.cpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#include "ServiceLocator.hpp"

///
/// Core namespace.
///
namespace galaxy
{
	galaxy::ServiceLocator& ServiceLocator::get() noexcept
	{
		static galaxy::ServiceLocator s_serviceLocator;
		return s_serviceLocator;
	}

	galaxy::Config* ServiceLocator::config() const noexcept
	{
		return m_config;
	}

	protostar::ThreadPool* ServiceLocator::pool() const noexcept
	{
		return m_threadPool;
	}

	qs::Window* ServiceLocator::window() const noexcept
	{
		return m_window;
	}

	qs::Renderer* ServiceLocator::renderer() const noexcept
	{
		return m_renderer;
	}

	sol::state* ServiceLocator::lua() const noexcept
	{
		return m_lua;
	}

	protostar::StateMachine* ServiceLocator::gamestate() const noexcept
	{
		return m_state;
	}

	starlight::Dispatcher* ServiceLocator::dispatcher() const noexcept
	{
		return m_dispatcher;
	}

	galaxy::World* ServiceLocator::world() const noexcept
	{
		return m_world;
	}

	galaxy::Serializer* ServiceLocator::serializer() const noexcept
	{
		return m_serializer;
	}

	galaxy::FontBook* ServiceLocator::fontbook() const noexcept
	{
		return m_fontbook;
	}

	galaxy::ShaderBook* ServiceLocator::shaderbook() const noexcept
	{
		return m_shaderbook;
	}

	galaxy::AudioBook* ServiceLocator::audiobook() const noexcept
	{
		return m_audiobook;
	}

	ServiceLocator::ServiceLocator() noexcept
		:m_restart(false), m_config(nullptr), m_threadPool(nullptr), m_window(nullptr), m_renderer(nullptr), m_lua(nullptr), m_state(nullptr), m_dispatcher(nullptr), m_world(nullptr), m_serializer(nullptr), m_fontbook(nullptr), m_shaderbook(nullptr), m_audiobook(nullptr)
	{
	}
}