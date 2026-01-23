///
/// KeyInput.cpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#include "KeyInput.hpp"

namespace galaxy
{
	KeyInput::KeyInput(std::string&& text) noexcept
	{
		m_text = std::move(text);
	}
} // namespace galaxy
