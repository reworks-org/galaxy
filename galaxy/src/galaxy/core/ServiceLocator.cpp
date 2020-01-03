///
/// ServiceLocator.cpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#include "ServiceLocator.hpp"

namespace galaxy
{
	const galaxy::ServiceLocator& ServiceLocator::i() noexcept
	{
		static galaxy::ServiceLocator s_serviceLocator;
		return s_serviceLocator;
	}

	const auto ServiceLocator::lua() const noexcept
	{
		return m_lua;
	}

	const auto ServiceLocator::config() const noexcept
	{
		return m_config;
	}

	const auto ServiceLocator::window() const noexcept
	{
		return m_window;
	}

	ServiceLocator::ServiceLocator()
		:m_lua(nullptr), m_window(nullptr)
	{
	}
}