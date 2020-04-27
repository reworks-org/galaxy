///
/// Layer.cpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#include "Layer.hpp"

namespace galaxy
{
	void Layer::setName(const std::string& name) noexcept
	{
		m_name = name;
	}

	const std::string& Layer::getName() noexcept
	{
		return m_name;
	}

	Layer::Layer() noexcept
		:m_name("default")
	{
		/* https://www.sfml-dev.org/tutorials/2.5/audio-spatialization.php */
	}

	Layer::Layer(const std::string& name) noexcept
		:m_name(name)
	{
	}
}