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
	galaxy::ServiceLocator& ServiceLocator::i() noexcept
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

	const auto ServiceLocator::fs() const noexcept
	{
		return m_fs;
	}

	const auto ServiceLocator::window() const noexcept
	{
		return m_window;
	}

	ServiceLocator::ServiceLocator()
		:m_lua(nullptr), m_config(nullptr), m_fs(nullptr), m_window(nullptr)
	{
	}
}