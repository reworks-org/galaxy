///
/// Layer.cpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#include "Layer.hpp"

namespace galaxy
{
	void Layer::set_name(std::string_view name) noexcept
	{
		m_name = static_cast<std::string>(name);
	}

	const std::string& Layer::get_name() noexcept
	{
		return m_name;
	}

	Layer::Layer() noexcept
	    : m_name {"default"}
	{
	}

	Layer::Layer(std::string_view name) noexcept
	{
		set_name(name);
	}
} // namespace galaxy